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
#include "kafe/project.hpp"
#include "kafe/execution_scope.hpp"
#include "kafe/io/file_system.hpp"
#include "kafe/scripting/script.hpp"

using namespace std;
using namespace kafe::io;
using namespace kafe::scripting;

namespace kafe {
    Project::Project(string project_file) : project_file(move(project_file)) {
        if (!FileSystem::is_file_or_symlink(this->project_file)) {
            throw ProjectFileException("Project file <%s> does not exist", this->project_file.c_str());
        }
    }

    Project::~Project() = default;

    void Project::execute(
            const Context &context,
            const Inventory &inventory,
            const vector<string> &extra_args
    ) {
        auto main_file = FileSystem::absolute(project_file, std_fs::current_path()).string();
        auto scope = ExecutionScope(context, inventory, extra_args, main_file);
        LoggingTimer timer;

        auto script = Script(scope);
        auto *logger = const_cast<ILogEventListener *>(scope.get_context()->get_log_listener());

        logger->emit_debug("Resolved project file is <%s>", main_file.c_str());

        timer = logger->emit_info_wt("Loading project file <%s>", project_file.c_str());
        script.load_file(main_file);
        logger->emit_success(&timer, "Loaded project file <%s>", project_file.c_str());

        timer = logger->emit_info_wt("Evaluating project file <%s>", project_file.c_str());
        script.evaluate();
        logger->emit_success(&timer, "Done evaluating project file <%s>", project_file.c_str());

        logger->emit_info("Verifying all tasks requested are defined");
        for (const auto &task_name : scope.get_context()->get_tasks()) {
            if (!scope.get_tasks()->task_exists(task_name)) {
                throw UnknownTaskException("Task <%s> is not defined in project", task_name.c_str());
            }
        }
        logger->emit_success("Tasks verified");

        for (const auto &task_name : scope.get_context()->get_tasks()) {
            timer = logger->emit_info_wt("Executing task <%s>", task_name.c_str());
            logger->context_push(task_name);

            const Task *task = scope.get_tasks()->get_task(task_name);
            int ref = task->get_function_reference();

            script.invoke_function_by_ref(ref, extra_args);

            logger->context_pop();
            logger->emit_success(&timer, "Task <%s> completed", task_name.c_str());
        }
    }
}