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

#ifndef LIBKAFE_EXECUTION_SCOPE_HPP
#define LIBKAFE_EXECUTION_SCOPE_HPP

#include "kafe/remote/ssh_api.hpp"
#include "kafe/context.hpp"
#include "kafe/project/inventory.hpp"
#include "kafe/project/tasks.hpp"
#include "kafe/remote/ssh_pool.hpp"
#include "kafe/remote/ssh_api.hpp"
#include "kafe/local/local_api.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::project;
using namespace kafe::remote;
using namespace kafe::local;

namespace kafe {
    class ExecutionScope {
        const Context &context;
        const Inventory &inventory;
        TaskList *tasks;
        SshPool *ssh_pool;
        SshApi *current_api = nullptr;
        vector<string> files_to_rm_on_destruct;
        map<string, string> values;
        LocalApi *local;
        const vector<string> &extra_args;
        const string& project_file;
        bool is_strict_exec_mode = false;
    public:
        explicit ExecutionScope(
                const Context &context,
                const Inventory &inventory,
                const vector<string> &extra_args,
                const string& project_file
        );

        [[nodiscard]] const Context *get_context() const;

        [[nodiscard]] const Inventory *get_inventory() const;

        [[nodiscard]] const TaskList *get_tasks() const;

        [[nodiscard]] const SshPool *get_ssh_pool() const;

        void set_current_remote(SshApi *api);

        [[nodiscard]] const SshApi *get_current_api() const;

        [[nodiscard]] bool has_current_api() const;

        void clear_current_api();

        void add_rm_on_destruct(const string &file);

        virtual ~ExecutionScope();

        void define_var(const string &key, const string &value);

        [[nodiscard]] const map<string, string> &get_vars() const;

        [[nodiscard]] string replace_vars(const string &input) const;

        [[nodiscard]] string replace_env(const string &input) const;

        [[nodiscard]] LocalApi *get_local_api() const;

        const vector<string> &get_extra_args() const;

        const string &get_project_file() const;

        const string get_env(const string &key) const;

        void set_strict(bool b);

        bool is_strict() const;
    };
}

#endif
