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

#ifndef LIBKAFE_PROJECT_TASK_HPP
#define LIBKAFE_PROJECT_TASK_HPP

#include <map>
#include <string>
#include "kafe/runtime/runtime_exception.hpp"

using namespace std;
using namespace kafe::runtime;

namespace kafe::project {
    class DuplicateTaskException : public RuntimeException {
        using RuntimeException::RuntimeException;
    };

    class UnknownTaskException : public RuntimeException {
        using RuntimeException::RuntimeException;
    };

    class Task {
        const string name;
        const int function_reference;
        const string source;

    public:
        Task(string  name, int function_reference, string  source);

        [[nodiscard]] const string &get_name() const;

        [[nodiscard]] int get_function_reference() const;

        [[nodiscard]] const string &get_source() const;
    };

    class TaskList {
        map<const string, const Task *> tasks;

    public:
        void add_task(const Task &task);

        [[nodiscard]] bool task_exists(const string &name) const;

        [[nodiscard]] const Task *get_task(const string &name) const;

        virtual ~TaskList();
    };
}
#endif
