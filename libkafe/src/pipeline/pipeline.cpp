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

#include <utility>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include "kafe/logging.hpp"
#include "kafe/pipeline/pipeline.hpp"

using namespace std;

namespace kafe::pipeline {
  string parser_strip_tailing_comment(string &str) {
      regex tail_comment_reg("[^\\\\]#");
      std::smatch tail_comment_m;
      bool found = regex_search(str, tail_comment_m, tail_comment_reg);

      if (found) {
          return tail_comment_m.prefix().str();
      }

      return str;
  }

  static inline bool parser_is_comment_line(string &str) {
      regex reg("^[ ]*#.*$");

      return regex_match(str, reg);
  }

  static inline bool parser_has_only_space(string &str) {
      return str.find_first_not_of(" \t\n\v\f\r") == std::string::npos;
  }

  static inline bool parser_is_label(string &str) {
      regex reg("^[A-Za-z][A-Za-z0-9_-]+:$");

      return regex_match(str, reg);
  }

  static inline bool parser_is_command(string &str) {
      return str.rfind('\t', 0) == 0
             || str.rfind("  ", 0) == 0
             || str.rfind("    ", 0) == 0;
  }

  static inline void ltrim(string &str) {
      str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) { return !isspace(ch); }));
  }

  static inline void rtrim(string &str) {
      str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), str.end());
  }

  static inline void trim(string &str) {
      ltrim(str);
      rtrim(str);
  }

  vector<PipelineStage> parse_pipeline_string(const string &pipeline_src) {
      bool in_stage_ctx = false;

      string stage_b;
      vector<string> commands_b;
      vector<PipelineStage> stages_b;

      istringstream src_stream(pipeline_src);
      string line;

      size_t line_c = 0;
      while (getline(src_stream, line)) {
          ++line_c;

          if (line.empty() || parser_is_comment_line(line) || parser_has_only_space(line)) {
              continue;
          }

          line = parser_strip_tailing_comment(line);

          if (parser_is_label(line)) {
              if (in_stage_ctx) {
                  stages_b.emplace_back(PipelineStage(stage_b, commands_b));
                  stage_b.clear();
                  commands_b.clear();
              }

              stage_b = line.substr(0, line.length() - 1);
              in_stage_ctx = true;
              continue;
          }

          if (parser_is_command(line)) {
              if (!in_stage_ctx) {
                  throw RuntimeException("Syntax error, line %d, command outside of stage - %s", line_c, line.c_str());
              }

              trim(line);

              if (!commands_b.empty()) {
                  string prev = commands_b.back();

                  if (prev[prev.length() - 1] == '\\') {
                      commands_b.back() = prev.substr(0, prev.length() - 1).append(line);
                      continue;
                  }
              }

              commands_b.push_back(line);
              continue;
          }

          if (!in_stage_ctx) {
              throw RuntimeException("Syntax error, line %d, expected stage label, got <%s>", line_c, line.c_str());
          }

          throw RuntimeException("Syntax error, line %d, expected command or label, got <%s>", line_c, line.c_str());
      }

      stages_b.emplace_back(PipelineStage(stage_b, commands_b));
      stage_b.clear();
      commands_b.clear();

      return stages_b;
  }


  PipelineStage::PipelineStage(string name, vector<string> tasks) : name(std::move(name)), tasks(std::move(tasks)) {
  }

  const string &PipelineStage::get_name() const {
      return name;
  }

  vector<string> PipelineStage::get_tasks() const {
      return tasks;
  }

  Pipeline::Pipeline(const vector<PipelineStage> &stages) : stages(stages) {}

  bool Pipeline::has_stage(const string &name) {
      return any_of(stages.begin(), stages.end(), [&](const auto &stage) {
        return stage.get_name() == name;
      });
  }

  const PipelineStage &Pipeline::get_stage(const string &name) {
      for (const auto &stage: stages) {
          if (stage.get_name() == name) {
              return stage;
          }
      }

      throw RuntimeException("Unknown stage");
  }

  Pipeline PipelineParser::from_file(const string &path) {
      std::ifstream f_in(path);
      std::string pipeline_str((std::istreambuf_iterator<char>(f_in)), std::istreambuf_iterator<char>());

      auto stages = parse_pipeline_string(pipeline_str);
      auto pipeline = Pipeline(stages);

      return pipeline;
  }
}
