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

#include <cstring>
#include <exception>
#include "kafe/execution_scope.hpp"
#include "kafe/io/file_system.hpp"

using namespace kafe::io;
using namespace kafe::local;
using namespace kafe::remote;

namespace kafe {
    ExecutionScope::ExecutionScope(
            const Context &context,
            const Inventory &inventory,
            const vector<string> &extra_args,
            const string &project_file
    ) : context(context), inventory(inventory), extra_args(extra_args), project_file(project_file) {
        this->ssh_pool = new SshPool();
        this->tasks = new TaskList();
        this->local = new LocalApi(context.get_log_listener());
    }

    const Context *ExecutionScope::get_context() const {
        return &context;
    }

    const Inventory *ExecutionScope::get_inventory() const {
        return &inventory;
    }

    const TaskList *ExecutionScope::get_tasks() const {
        return tasks;
    }

    const SshPool *ExecutionScope::get_ssh_pool() const {
        return ssh_pool;
    }

    void ExecutionScope::set_current_remote(SshApi *api) {
        this->current_api = api;
    }

    const SshApi *ExecutionScope::get_current_api() const {
        return this->current_api;
    }

    bool ExecutionScope::has_current_api() const {
        return nullptr != this->current_api;
    }

    void ExecutionScope::clear_current_api() {
        this->current_api = nullptr;
    }

    void ExecutionScope::add_rm_on_destruct(const string &file) {
        files_to_rm_on_destruct.push_back(file);
    }

    ExecutionScope::~ExecutionScope() {
        clear_current_api();

        for (const auto &file: files_to_rm_on_destruct) {
            try {
                FileSystem::try_rm_r(file);
            } catch (exception &e) {
                // NOOP
            }
        }

        delete this->local;
    }

    void ExecutionScope::define_var(const string &key, const string &value) {
        values[key] = value;
    }

    const map<string, string> &ExecutionScope::get_vars() const {
        return values;
    }

    static const char *const START_TOK = "{{";
    static const size_t STRLEN_START_TOK = strlen(START_TOK);

    static const char *const END_TOK = "}}";
    static const size_t STRLEN_END_TOK = strlen(END_TOK);

    string ExecutionScope::replace_vars(const string &input) const {
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
            auto subst_it = values.find(input.substr(subst_pos, end_pos - subst_pos));

            if (subst_it != values.end()) {
                out << subst_it->second;
            }

            pos = end_pos + STRLEN_END_TOK;
        }
        out << input.substr(pos, string::npos);

        return out.str();
    }

    string ExecutionScope::replace_env(const string &input) const {
        const auto *envvals = context.get_envvals();
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
            auto env_val = envvals->find(key);

            if (env_val != envvals->end()) {
                out << env_val->second;
            }

            pos = end_pos + STRLEN_END_TOK;
        }
        out << input.substr(pos, string::npos);

        return out.str();
    }

    const string ExecutionScope::get_env(const string &key) const {
        const auto *envvals = context.get_envvals();

        auto env_val = envvals->find(key);

        if (env_val != envvals->end()) {
            return env_val->second;
        }

        return {};
    }

    LocalApi *ExecutionScope::get_local_api() const {
        return local;
    }

    const vector<string> &ExecutionScope::get_extra_args() const {
        return extra_args;
    }

    const string &ExecutionScope::get_project_file() const {
        return project_file;
    }

    void ExecutionScope::set_strict(bool state) {
        this->is_strict_exec_mode = state;
    }

    bool ExecutionScope::is_strict() const {
        return is_strict_exec_mode;
    }
}
