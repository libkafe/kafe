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

  string parser_replace_values(const string &input, const map<const string, const string> &keyValueSet) {
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

          const char *key = input.substr(subst_pos, end_pos - subst_pos).c_str();
          auto env_val = keyValueSet.find(key);

          if (env_val == keyValueSet.end()) {
              throw RuntimeException("Undefined variable key <%s> in command <%s>", key, input.c_str());
          } else {
              out << env_val->second;
          }

          pos = end_pos + STRLEN_END_TOK;
      }

      out << input.substr(pos, string::npos);

      return out.str();
  }

  int Maker::make(
      const string& file,
      const map<const string, const string> &env_vals,
      const vector<string>& stages,
      ILogEventListener *log_listener,
      const vector<string> &extra_args
  ) {
      // TODO - allow no-op - just print commands and do nothing
      auto main_file = FileSystem::absolute(file, std_fs::current_path()).string();

      if (!FileSystem::is_file_or_symlink(main_file)) {
          throw MakeFileException("make file <%s> does not exist", main_file.c_str());
      }

      auto pipeline = PipelineParser::from_file(main_file, env_vals, extra_args, log_listener);

      for (const string &stage : stages) {
          if (!pipeline.has_stage(stage)) {
              throw MakeFileException(
                  "make file <%s> does not have stage <%s>",
                  main_file.c_str(),
                  stage.c_str()
              );
          }
      }

      const auto local_api = new LocalApi(log_listener);

      for (const string &stage_name : stages) {
          LoggingTimer stage_timer = log_listener->emit_info_wt("Entering stage <%s>", stage_name.c_str());

          log_listener->context_push(stage_name);

          const auto stage = pipeline.get_stage(stage_name);

          for (const auto &task: stage.get_tasks()) {
              log_listener->emit_debug("Executing stage command from declaration <%s>", task.c_str());


              try {
                  const string command = parser_replace_values(task, pipeline.get_vals());
                  const auto result = local_api->local_popen(command, true);

                  if (result.get_code() != 0) {
                      throw RuntimeException("Non-zero command exit code <%d>", result.get_code());
                  }

              } catch (RuntimeException &e) {
                  log_listener->emit_error("Stage execution failed - %s", e.what());
                  return 1;
              }
          }

          log_listener->emit_success(&stage_timer, "Stage complete");
          log_listener->context_pop();
      }
      return 0;
  }
}
