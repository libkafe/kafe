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

#ifndef LIBKAFE_PIPELINE_PIPELINE_HPP
#define LIBKAFE_PIPELINE_PIPELINE_HPP

#include <string>
#include <vector>
#include <map>
#include "kafe/logging.hpp"
#include "kafe/runtime/runtime_exception.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::runtime;

namespace kafe::pipeline {
  class PipelineStage {
    const string name;
    const vector<string> tasks;

  public:
    PipelineStage(string name, vector<string> tasks);

    [[nodiscard]] const string &get_name() const;

    [[nodiscard]] vector<string> get_tasks() const;
  };

  class Pipeline {
    const vector<PipelineStage> stages;
    const map<const string, const string> vals;

  public:
    explicit Pipeline(const vector<PipelineStage> &stages, map<const string, const string> map1);

    [[nodiscard]] const vector<PipelineStage> &get_stages() const;

    [[nodiscard]] const map<const string, const string> &get_vals() const;

    [[nodiscard]] bool has_stage(const string &name);

    const PipelineStage &get_stage(const string &name);
  };

  class PipelineParser {

  public:
    static Pipeline
    from_file(const string &path, const map<const string, const string> &map, const vector<string> &vector1,
              const kafe::ILogEventListener *p_listener);

  };

  class PipelineExecutor {
    void execute(Pipeline &pipeline);
  };
}

#endif //LIBKAFE_PIPELINE_PIPELINE_HPP
