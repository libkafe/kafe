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

#include "kafe/project/tasks.hpp"

#include <utility>

using namespace std;

namespace kafe::project {
    Task::Task(string name, int function_reference, string source)
            : name(move(name)), function_reference(function_reference), source(move(source)) {
    }

    const string &Task::get_name() const {
        return name;
    }

    int Task::get_function_reference() const {
        return function_reference;
    }

    const string &Task::get_source() const {
        return source;
    }

    void TaskList::add_task(const Task &task) {
        const auto &name = task.get_name();

        if (task_exists(name)) {
            throw DuplicateTaskException("Duplicate task <%s>", name.c_str());
        }

        pair<const string, const Task *> entry = pair<const string, const Task *>(name, &task);
        this->tasks.insert(entry);
    }

    const Task *TaskList::get_task(const string &name) const {
        auto task = tasks.find(name);

        if (task == tasks.end()) {
            throw UnknownTaskException("Unknown task <%s>", name.c_str());
        }

        return task->second;
    }

    bool TaskList::task_exists(const string &name) const {
        return tasks.find(name) != tasks.end();
    }

    TaskList::~TaskList() {
        for (const auto &task : tasks) {
            delete (task.second);
        }
        tasks.clear();
    }
}

