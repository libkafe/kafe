/**
 * This file is part of Kafe.
 * https://github.com/libkafe/kafe/
 *
 * Copyright 2020 Matiss Treinis
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
#include "kafe/context.hpp"

namespace kafe {
    Context::Context(
            const map<const string, const string> &envvals,
            string environment,
            vector<string> tasks,
            const ILogEventListener *log_listener
    ) : envvals(envvals), environment(move(environment)), tasks(move(tasks)), log_listener(log_listener) {
    }

    Context::~Context() = default;

    const string &Context::get_environment() const {
        return environment;
    }

    const vector<string> &Context::get_tasks() const {
        return tasks;
    }

    const ILogEventListener *Context::get_log_listener() const {
        return log_listener;
    }

    const map<const string, const string> *Context::get_envvals() const {
        return &envvals;
    }
}