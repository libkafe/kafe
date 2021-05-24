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

#ifndef LIBKAFE_CONTEXT_HPP
#define LIBKAFE_CONTEXT_HPP

#include <string>
#include <vector>
#include <map>
#include "logging.hpp"

using namespace std;

namespace kafe {
    class Context {
        const map<const string, const string> &envvals;
        const string environment;
        const vector<string> tasks;
        const ILogEventListener *log_listener;
    public:
        Context(
                const map<const string, const string> &envvals,
                string environment,
                vector<string> tasks,
                const ILogEventListener *log_listener
        );

        virtual ~Context();

        [[nodiscard]] const map<const string, const string> *get_envvals() const;

        [[nodiscard]] const string &get_environment() const;

        [[nodiscard]] const vector<string> &get_tasks() const;

        [[nodiscard]] const ILogEventListener *get_log_listener() const;

        [[nodiscard]] bool is_local_context() const;
    };
}

#endif