/**
 * This file is part of Kafe.
 * https://github.com/libkafe/kafe/
 *
 * Copyright 2021 Matiss Treinis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include "kafe/make.hpp"
#include "kafe/io/file_system.hpp"
#include "kafe/local/local_api.hpp"
#include "kafe/pipeline/pipeline.hpp"

using namespace std;
using namespace kafe::io;
using namespace kafe::runtime;
using namespace kafe::pipeline;
using namespace kafe::local;

namespace kafe {
  static const char *const START_TOK = "{{";
  static const size_t STRLEN_START_TOK = strlen(START_TOK);

  static const char *const END_TOK = "}}";
  static const size_t STRLEN_END_TOK = strlen(END_TOK);

  string command_replace_values(const string &input, const map<const string, string> &key_value_set) {
      ostringstream out;
      size_t pos = 0;
      for (;;) {
          size_t subst_pos = input.find(START_TOK, pos);
          size_t end_pos = input.find(END_TOK, subst_pos);

          if (end_pos == string::npos) {
              break;
          }

          out.write(&*input.begin() + pos, subst_pos - pos);
          subst_pos += STRLEN_START_TOK;

          auto key = input.substr(subst_pos, end_pos - subst_pos);
          auto env_val = key_value_set.find(key);

          if (env_val == key_value_set.end()) {
              throw RuntimeException("Undefined variable key <%s> in command <%s>", key.c_str(), input.c_str());
          } else {
              out << env_val->second;
          }

          pos = end_pos + STRLEN_END_TOK;
      }

      out << input.substr(pos, string::npos);

      return out.str();
  }

  // TODO - allow no-op - just print commands and do nothing
  int Maker::make(
      const string &file,
      const map<const string, const string> &env_vals,
      const vector<string> &stages,
      ILogEventListener *logger,
      const vector<string> &extra_args
  ) {
      auto build_timer = logger->emit_debug_wt("Build start");
      auto cwd = std_fs::current_path();

      map<const string, string> pipeline_vars;
      pipeline_vars.insert(env_vals.begin(), env_vals.end());

      for (const auto &extra_arg : extra_args) {
          auto assignPos = extra_arg.find_first_of('=');
          if (string::npos == assignPos) {
              throw RuntimeException(
                  "Invalid value argument <%s>, expected key value pair (key=value)",
                  extra_arg.c_str()
              );
          }

          pipeline_vars[extra_arg.substr(0, assignPos)] = extra_arg.substr(assignPos + 1, string::npos);
      }

      const auto build_dir = FileSystem::tmp_dir("kafe-build-");
      pipeline_vars["KAFE_PROJECT_DIR"] = cwd;
      pipeline_vars["KAFE_BUILD_DIR"] = build_dir;

      try {
          auto main_file = FileSystem::absolute(file, std_fs::current_path()).string();

          if (!FileSystem::is_file_or_symlink(main_file)) {
              throw MakeFileException("make file <%s> does not exist", main_file.c_str());
          }

          auto pipeline = PipelineParser::from_file(main_file);

          for (const string &stage : stages) {
              if (!pipeline.has_stage(stage)) {
                  throw MakeFileException(
                      "make file <%s> does not have stage <%s>",
                      main_file.c_str(),
                      stage.c_str()
                  );
              }
          }

          const auto local_api = new LocalApi(logger);

          logger->emit_debug("KAFE_BUILD_DIR is <%s>", build_dir.c_str());

          for (const string &stage_name : stages) {
              LoggingTimer stage_timer = logger->emit_info_wt("Entering stage <%s>", stage_name.c_str());
              logger->context_push(stage_name);

              const auto stage = pipeline.get_stage(stage_name);

              const auto stage_dir = build_dir / stage_name;
              FileSystem::mkdirs(stage_dir);
              pipeline_vars["KAFE_STAGE_DIR"] = stage_dir;

              logger->emit_debug("KAFE_STAGE_DIR is <%s>", stage_dir.c_str());

              for (const auto &task: stage.get_tasks()) {
                  logger->emit_debug("Executing stage command from declaration <%s>", task.c_str());

                  try {
                      const string command = command_replace_values(task, pipeline_vars);
                      if (':' == command[0]) {
                          const auto internal_command = command.substr(1, string::npos);

                          if (internal_command.empty()) {
                              throw RuntimeException("Empty make command in stage <%s>", stage_name.c_str());
                          }

                          call_internal(internal_command, logger);
                      } else {
                          const auto result = local_api->local_popen(command, true);

                          if (result.get_code() != 0) {
                              throw RuntimeException("Non-zero command exit code <%d>", result.get_code());
                          }
                      }
                  } catch (RuntimeException &e) {
                      logger->emit_error("Stage execution failed - %s", e.what());
                      return 1;
                  }
              }

              logger->emit_success(&stage_timer, "Stage complete");
              logger->context_pop();

              // Move back to cwd once stage is complete
              std_fs::current_path(cwd);
          }
      } catch (exception &e) {
          std_fs::current_path(cwd);
          FileSystem::try_rm_r(build_dir);
          logger->emit_error(&build_timer, "Pipeline execution failed - %s", e.what());
          return 1;
      }

      std_fs::current_path(cwd);
      FileSystem::try_rm_r(build_dir);
      logger->emit_success(&build_timer, "Pipeline complete");

      return 0;
  }

  void Maker::call_internal(const string &command, ILogEventListener *logger) {
      auto cmd_arg_delim_pos = command.find(' ');
      auto cmd = command.substr(0, cmd_arg_delim_pos);
      string arg;
      if (cmd_arg_delim_pos != string::npos) {
          arg = command.substr(cmd_arg_delim_pos + 1);
      }

      if (cmd == "cwd") {
          if (arg.find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
              throw RuntimeException("cwd expects a single argument - path");
          }

          auto new_cwd = FileSystem::normalize(arg, std_fs::current_path());

          if (!FileSystem::is_directory(new_cwd)) {
              throw RuntimeException("Can not cwd to directory <%s>, not a directory", arg.c_str());
          }

          logger->emit_info("Changing working directory to <%s>", new_cwd.c_str());
          std_fs::current_path(new_cwd);
          return;
      }

      throw RuntimeException("Unknown command <%s>", command.c_str());
  }
}
