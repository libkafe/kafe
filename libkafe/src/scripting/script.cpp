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

#include <map>

#include "kafe/version.hpp"
#include "kafe/execution_scope.hpp"
#include "kafe/scripting/script.hpp"
#include "kafe/remote/ssh_manager.hpp"
#include "kafe/remote/ssh_api.hpp"
#include "kafe/io/archive.hpp"
#include "kafe/io/file_system.hpp"

using namespace kafe::io;

static map<lua_State *, const ExecutionScope *> state;

static inline ExecutionScope *get_scope(lua_State *L) {
    auto res = state.find(L);

    if (res == state.end()) {
        throw RuntimeException("Out of scope");
    }

    return const_cast<ExecutionScope *>(res->second);
}

namespace kafe::scripting {
    // Lua stdout/stderr
    // TODO: split line by line
    static int lua_logger_print(lua_State *L, bool is_err) {
        auto *scope = get_scope(L);
        int n_args = lua_gettop(L);
        lua_getglobal(L, "tostring");
        for (int i = 1; i <= n_args; i++) {
            lua_pushvalue(L, -1);
            lua_pushvalue(L, i);
            lua_call(L, 1, 1);
            const auto *line = lua_tostring(L, -1);
            if (nullptr == line) {
                return luaL_error(L, "'tostring' must return a string to 'print'");
            }

            auto with_vars = scope->replace_vars(line);

            if (is_err) {
                scope->get_context()->get_log_listener()->on_stderr_line("#! err", with_vars);
            } else {
                scope->get_context()->get_log_listener()->on_stdout_line("#! out", with_vars);
            }
            lua_pop(L, 1);
        }
        return 0;
    }

    static int lua_logger_print(lua_State *L) {
        return lua_logger_print(L, false);
    }

    static int lua_logger_print_err(lua_State *L) {
        return lua_logger_print(L, true);
    }
    // end Lua stdout/stderr

    Script::Script(const ExecutionScope &scope) : scope(scope) {
        auto *lst = luaL_newstate();
        auto entry = pair<lua_State *, const ExecutionScope *>(lst, &scope);
        state.insert(entry);

        this->lua_state = lst;
        luaL_openlibs(this->lua_state);

        // Override print to send output to logger
        lua_register(lua_state, "print", lua_logger_print);
        lua_register(lua_state, "print_err", lua_logger_print_err);

        initialize();
    }

    Script::~Script() {
        state.erase(this->lua_state);
        lua_close(this->lua_state);
    }

    void Script::load_file(const string &script_file) {
        auto status = luaL_loadfile(this->lua_state, script_file.c_str());

        if (status) {
            const auto *lua_error = lua_tostring(this->lua_state, -1);
            const auto with_vars = scope.replace_vars(lua_error);
            throw ScriptEngineException("%s", with_vars.c_str());
        }
    }

    void Script::evaluate() {
        int status = lua_pcall(this->lua_state, 0, LUA_MULTRET, 0);

        if (status) {
            const auto *lua_error = lua_tostring(this->lua_state, -1);
            const auto with_vars = scope.replace_vars(lua_error);
            throw ScriptEvaluationException("%s", with_vars.c_str());
        }
    }

    void Script::invoke_function_by_ref(const int reference, const vector<string> &extra_args) {
        lua_rawgeti(this->lua_state, LUA_REGISTRYINDEX, reference);

        for (const auto &var : extra_args) {
            lua_pushstring(this->lua_state, var.c_str());
        }

        unsigned long status = lua_pcall(this->lua_state, extra_args.size(), 0, 0);

        if (status) {
            const auto *lua_error = lua_tostring(this->lua_state, -1);
            const auto with_vars = scope.replace_vars(lua_error);
            throw ScriptInvocationException("%s", with_vars.c_str());
        }
    }

    // BEGIN integration with Lua
    lua_Debug get_lua_debug(lua_State *L) {
        lua_Debug ar;
        lua_getstack(L, 1, &ar);
        lua_getinfo(L, "Sl", &ar);
        return ar;
    }

    int lua_api_level_require(lua_State *L) {
        if (1 != lua_gettop(L) || !lua_isinteger(L, 1)) {
            return luaL_error(L, "Expected one argument - api level as integer");
        }

        auto api_level_required = luaL_checkinteger(L, 1);

        if (api_level_required > LIBKAFE_API_LEVEL) {
            return luaL_error(
                    L,
                    "Script requires minimum API level %d, libkafe API level is %d",
                    api_level_required,
                    LIBKAFE_API_LEVEL
            );
        }
        return 0;
    }

    // TODO: allow kDSN format - <env+role://user@host:port>
    int lua_api_inventory_add(lua_State *L) {
        const auto *scope = get_scope(L);

        if (5 != lua_gettop(L)) {
            return luaL_error(L, "Expected five arguments - username, hostname, port, environment, role");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one must be a string - username");
        }

        if (!lua_isstring(L, 2)) {
            return luaL_error(L, "Argument two must be a string - hostname");
        }

        if (!lua_isinteger(L, 3)) {
            return luaL_error(L, "Argument three must be a integer - port");
        }

        if (!lua_isstring(L, 4)) {
            return luaL_error(L, "Argument four must be a string - environment");
        }

        if (!lua_isstring(L, 5)) {
            return luaL_error(L, "Argument five must be a string - role");
        }

        const auto *username = luaL_checkstring(L, 1);
        const auto *hostname = luaL_checkstring(L, 2);
        auto port = luaL_checkinteger(L, 3);
        const auto *environment = luaL_checkstring(L, 4);
        const auto *role = luaL_checkstring(L, 5);

        if (port < 1 || port > 65535) {
            return luaL_error(L, "Invalid port value <%s>", port);
        }

        auto *item = new InventoryItem(username, hostname, (unsigned int) port, environment, role);
        auto *inventory = const_cast<Inventory *>(scope->get_inventory());

        if (inventory->item_exists(*item)) {
            return luaL_error(L, "Duplicate inventory item <%s>", item->to_string().c_str());
        }

        scope->get_context()->get_log_listener()->emit_info(
                "Remote <%s+%s://%s@%s:%d> added to inventory",
                item->get_environment().c_str(),
                item->get_role().c_str(),
                item->get_user().c_str(),
                item->get_host().c_str(),
                item->get_port()
        );

        inventory->add(*item);

        return 0;
    }

    int lua_api_task_define(lua_State *L) {
        const auto *scope = get_scope(L);

        if (2 != lua_gettop(L)) {
            return luaL_error(L, "Expected two arguments, task name and task function");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one must be a string task name");
        }

        if (!lua_isfunction(L, 2)) {
            return luaL_error(L, "Argument two must be a function");
        }

        const auto *task_name = luaL_checkstring(L, 1);
        auto *tasks = const_cast<TaskList *>(scope->get_tasks());

        if (tasks->task_exists(task_name)) {
            const auto *other = tasks->get_task(task_name);
            return luaL_error(
                    L,
                    "Duplicate task definition with name <%s>, already defined at %s",
                    task_name,
                    other->get_source().c_str()
            );
        }

        auto function_reference = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_Debug ar = get_lua_debug(L);

        auto source = string(ar.short_src)
                      + string(":")
                      + to_string(ar.currentline);

        auto *task = new Task(
                task_name,
                function_reference,
                source
        );

        tasks->add_task(*task);

        scope->get_context()->get_log_listener()->emit_info(
                "Defined task <%s>",
                task_name
        );

        return 0;
    }

    int lua_api_on_role_invoke(lua_State *L) {
        auto *scope = get_scope(L);
        auto *logger = const_cast<ILogEventListener *>(scope->get_context()->get_log_listener());

        if (scope->has_current_api()) {
            return luaL_error(L, "Nested role context invocation is not allowed (using kafe.on(...) when already "
                                 "scoped by another kafe.on(...))");
        }

        auto n_args = lua_gettop(L);
        if (2 != n_args && 3 != n_args) {
            return luaL_error(L,
                              "Expected two or three arguments, environment name and function to execute, and optional skip flag");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one must be a string environment name");
        }

        if (!lua_isfunction(L, 2)) {
            return luaL_error(L, "Argument two must be a function");
        }

        bool skip_empty = true;

        if (3 == n_args) {
            if (!lua_isboolean(L, 3)) {
                return luaL_error(L, "Argument three must be a boolean");
            }

            skip_empty = static_cast<bool>(lua_toboolean(L, 3));
        }

        const auto *role = luaL_checkstring(L, 1);
        auto function_reference = luaL_ref(L, LUA_REGISTRYINDEX);

        auto inventory_items = scope->get_inventory()->find_for_scope(
                scope->get_context()->get_environment(),
                role
        );

        if (inventory_items.empty()) {
            if (skip_empty) {
                return 0;
            }

            return luaL_error(L, "Unable to invoke method on role <%s> - has no targets", role);
        }

        logger->emit_info("Entering role <%s>", string(role).c_str());
        logger->context_push(string(role));

        bool failed = false;
        for (const auto *item : inventory_items) {
            auto remote_id = item->remote_id();
            logger->emit_info("Entering node <%s>", remote_id.c_str());
            logger->context_push(remote_id);
            const auto *envvals = scope->get_context()->get_envvals();
            auto ssh_manager = SshManager(scope->get_ssh_pool(), envvals, item);
            auto ssh_api = SshApi(&ssh_manager, logger);

            scope->set_current_remote(&ssh_api);

            lua_rawgeti(L, LUA_REGISTRYINDEX, function_reference);
            int status = lua_pcall(L, 0, 0, 0);

            scope->clear_current_api();

            if (status) {
                failed = true;
                const auto *lua_error = lua_tostring(L, -1);
                const auto with_vars = scope->replace_vars(lua_error);
                logger->emit_warning("%s", with_vars.c_str());
                logger->context_pop();
                break;
            }
            logger->context_pop();
        }

        logger->context_pop();

        lua_pushboolean(L, !failed);

        return 1;
    }

    int lua_api_remote_within(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not change remote directory when not in remote scope");
        }

        if (1 != lua_gettop(L) || !lua_isstring(L, 1)) {
            luaL_error(L, "Expected one argument - string");
        }

        auto directory = scope->replace_vars(luaL_checkstring(L, 1));
        auto *api = const_cast<SshApi *>(scope->get_current_api());

        scope->get_context()->get_log_listener()->emit_info(
                "Changing remote working directory for current context to <%s>",
                directory.c_str()
        );

        api->chdir(directory);

        return 0;
    }

    int lua_api_remote_exec(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not execute remote command when not in remote scope");
        }

        int n_args = lua_gettop(L);

        if (1 != n_args && 2 != n_args) {
            return luaL_error(L, "Expected one or two arguments");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        bool print_output = true;
        if (n_args == 2) {
            if (!lua_isboolean(L, 2)) {
                return luaL_error(L, "Argument two is expected to be boolean");
            }
            print_output = static_cast<bool>(lua_toboolean(L, 2));
        }

        auto command = scope->replace_vars(luaL_checkstring(L, 1));
        const auto *api = scope->get_current_api();

        auto result = api->execute(command, print_output);

        lua_pushstring(L, result.get_stdout().c_str());
        lua_pushstring(L, result.get_stderr().c_str());
        lua_pushinteger(L, result.get_code());

        return 3;
    }

    int lua_api_remote_shell(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not execute remote command when not in remote scope");
        }

        int n_args = lua_gettop(L);

        if (1 != n_args) {
            return luaL_error(L, "Expected one argument");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        auto command = scope->replace_vars(luaL_checkstring(L, 1));
        const auto *api = scope->get_current_api();

        auto result = api->execute(command, true);

        lua_pushboolean(L, 0 == result.get_code());

        return 1;
    }

    int lua_api_archive_dir_tmp(lua_State *L) {
        auto *scope = get_scope(L);
        auto *logger = const_cast<ILogEventListener *>(scope->get_context()->get_log_listener());

        if (scope->has_current_api()) {
            scope->get_context()->get_log_listener()->emit_warning(
                    "Creating archive while connected to remote server. Archive will be created per each"
                    "remote server. This is usually not optimal."
            );
        }

        if (1 != lua_gettop(L) || !lua_isstring(L, 1)) {
            return luaL_error(L, "Expected one argument - string");
        }

        auto directory = scope->replace_vars(luaL_checkstring(L, 1));
        auto directory_norm = FileSystem::normalize(directory, scope->get_local_api()->get_chdir());

        auto timer = scope->get_context()->get_log_listener()->emit_info_wt(
                "Archiving directory <%s> into temporary archive",
                directory_norm.c_str()
        );

        auto path = Archive::tmp_archive_from_directory(directory_norm, logger);

        scope->get_context()->get_log_listener()->emit_success(
                &timer,
                "Archive created in <%s>",
                path.c_str()
        );

        scope->add_rm_on_destruct(path);

        lua_pushstring(L, path.c_str());

        return 1;
    }

    int lua_api_archive_dir(lua_State *L) {
        const auto *scope = get_scope(L);
        auto *logger = const_cast<ILogEventListener *>(scope->get_context()->get_log_listener());

        if (scope->has_current_api()) {
            scope->get_context()->get_log_listener()->emit_warning(
                    "Creating archive while connected to remote server. Archive will be created per each"
                    "remote server. This is usually not optimal."
            );
        }

        if (2 != lua_gettop(L) || !lua_isstring(L, 1) || !lua_isstring(L, 2)) {
            return luaL_error(L, "Expected two arguments - strings");
        }

        auto archive = scope->replace_vars(luaL_checkstring(L, 1));
        auto archive_norm = FileSystem::normalize(archive, scope->get_local_api()->get_chdir());
        auto directory = scope->replace_vars(luaL_checkstring(L, 2));
        auto directory_norm = FileSystem::normalize(directory, scope->get_local_api()->get_chdir());

        auto timer = scope->get_context()->get_log_listener()->emit_info_wt(
                "Archiving directory <%s> into archive file <%s>",
                directory_norm.c_str(),
                archive_norm.c_str()
        );

        Archive::archive_from_directory(archive_norm, directory_norm, logger);

        scope->get_context()->get_log_listener()->emit_success(
                &timer,
                "Archive created in <%s>",
                archive_norm.c_str()
        );

        return 0;
    }

    int lua_api_upload_file(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not upload files when not in remote scope");
        }

        int n_args = lua_gettop(L);

        if (2 != n_args) {
            return luaL_error(L, "Expected two arguments");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        if (!lua_isstring(L, 2)) {
            return luaL_error(L, "Argument two is expected to be string");
        }

        auto local_file = scope->replace_vars(luaL_checkstring(L, 1));
        auto remote_file = scope->replace_vars(luaL_checkstring(L, 2));

        auto local_file_norm = FileSystem::normalize(local_file, scope->get_local_api()->get_chdir());

        const auto *api = scope->get_current_api();

        auto timer = scope->get_context()->get_log_listener()->emit_info_wt(
                "Uploading local file <%s> to remote <%s>",
                local_file_norm.c_str(),
                remote_file.c_str()
        );

        try {
            api->scp_upload_file(local_file_norm, remote_file);
            lua_pushboolean(L, true);

            scope->get_context()->get_log_listener()->emit_success(
                    &timer,
                    "Upload complete"
            );
            lua_pushboolean(L, true);
        } catch (exception &e) {
            scope->get_context()->get_log_listener()->emit_error(
                    &timer,
                    "Upload failed - %s",
                    e.what()
            );
            lua_pushboolean(L, false);
        }

        return 1;
    }

    int lua_api_download_file(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not download files when not in remote scope");
        }

        int n_args = lua_gettop(L);

        if (2 != n_args) {
            return luaL_error(L, "Expected two arguments");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        if (!lua_isstring(L, 2)) {
            return luaL_error(L, "Argument two is expected to be string");
        }

        auto local_file = scope->replace_vars(luaL_checkstring(L, 1));
        auto remote_file = scope->replace_vars(luaL_checkstring(L, 2));

        auto local_file_norm = FileSystem::normalize(local_file, scope->get_local_api()->get_chdir());

        const auto *api = scope->get_current_api();

        auto timer = scope->get_context()->get_log_listener()->emit_info_wt(
                "Downloading file <%s> from remote <%s>",
                local_file_norm.c_str(),
                remote_file.c_str()
        );

        try {
            api->scp_download_file(local_file_norm, remote_file);
            lua_pushboolean(L, true);

            scope->get_context()->get_log_listener()->emit_success(
                    &timer,
                    "Download complete"
            );
            lua_pushboolean(L, true);
        } catch (exception &e) {
            scope->get_context()->get_log_listener()->emit_error(
                    &timer,
                    "Download failed - %s",
                    e.what()
            );
            lua_pushboolean(L, false);
        }

        return 1;
    }

    int lua_api_upload_str(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not upload files when not in remote scope");
        }

        int n_args = lua_gettop(L);

        if (2 != n_args) {
            return luaL_error(L, "Expected two arguments");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        if (!lua_isstring(L, 2)) {
            return luaL_error(L, "Argument two is expected to be string");
        }

        auto content = scope->replace_vars(luaL_checkstring(L, 1));
        auto remote_file = scope->replace_vars(luaL_checkstring(L, 2));

        const auto *api = scope->get_current_api();

        auto timer = scope->get_context()->get_log_listener()->emit_info_wt(
                "Uploading string as file to remote <%s>",
                remote_file.c_str()
        );

        try {
            api->scp_upload_file_from_string(content, remote_file);
            lua_pushboolean(L, true);

            scope->get_context()->get_log_listener()->emit_success(
                    &timer,
                    "Upload complete"
            );
            lua_pushboolean(L, true);
        } catch (exception &e) {
            scope->get_context()->get_log_listener()->emit_error(
                    &timer,
                    "Upload failed - %s",
                    e.what()
            );
            lua_pushboolean(L, false);
        }

        return 1;
    }

    int lua_api_download_str(lua_State *L) {
        const auto *scope = get_scope(L);

        if (!scope->has_current_api()) {
            return luaL_error(L, "Can not download files when not in remote scope");
        }

        int n_args = lua_gettop(L);

        if (1 != n_args) {
            return luaL_error(L, "Expected two arguments");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        auto remote_file = scope->replace_vars(luaL_checkstring(L, 1));
        const auto *api = scope->get_current_api();
        auto timer = scope->get_context()->get_log_listener()->emit_info_wt(
                "Downloading string from remote <%s>",
                remote_file.c_str()
        );

        try {
            auto string = api->scp_download_file_as_string(remote_file);
            lua_pushstring(L, string.c_str());

            scope->get_context()->get_log_listener()->emit_success(
                    &timer,
                    "Download complete"
            );
        } catch (exception &e) {
            scope->get_context()->get_log_listener()->emit_error(
                    &timer,
                    "Download failed - %s",
                    e.what()
            );
            lua_pushboolean(L, false);
        }

        return 1;
    }

    int lua_api_define(lua_State *L) {
        auto *scope = get_scope(L);

        int n_args = lua_gettop(L);

        if (2 != n_args) {
            return luaL_error(L, "Expected two arguments");
        }

        const auto *key = luaL_checkstring(L, 1);
        auto value = scope->replace_vars(lua_tostring(L, 2));

        scope->get_context()->get_log_listener()->emit_debug(
                "Defining variable <%s>, value <%s>",
                key,
                value.c_str()
        );

        scope->define_var(key, value);

        return 0;
    }

    int lua_api_strfvars(lua_State *L) {
        const auto *scope = get_scope(L);
        int n_args = lua_gettop(L);

        if (1 != n_args) {
            return luaL_error(L, "Expected one argument");
        }

        const auto *input = luaL_checkstring(L, 1);

        try {
            lua_pushstring(L, scope->replace_vars(input).c_str());
        } catch (exception &e) {
            return luaL_error(L, (string("Failed to format string - ") + string(e.what())).c_str());
        }

        return 1;
    }

    int lua_api_strfenv(lua_State *L) {
        const auto *scope = get_scope(L);
        int n_args = lua_gettop(L);

        if (1 != n_args) {
            return luaL_error(L, "Expected one argument");
        }

        const auto *input = luaL_checkstring(L, 1);
        lua_pushstring(L, scope->replace_env(input).c_str());

        return 1;
    }

    int lua_api_getenv(lua_State *L) {
        const auto *scope = get_scope(L);
        int n_args = lua_gettop(L);

        if (1 != n_args) {
            return luaL_error(L, "Expected one argument");
        }

        const auto *input = luaL_checkstring(L, 1);
        const string val = scope->get_env(input);
        lua_pushstring(L, val.empty() ? nullptr : val.c_str());

        return 1;
    }

    int lua_api_local_within(lua_State *L) {
        const auto *scope = get_scope(L);

        if (1 != lua_gettop(L) || !lua_isstring(L, 1)) {
            luaL_error(L, "Expected one argument - string");
        }

        auto directory = scope->replace_vars(luaL_checkstring(L, 1));
        auto directory_norm = FileSystem::normalize(directory, scope->get_local_api()->get_chdir());

        scope->get_context()->get_log_listener()->emit_info(
                "Changing local working directory to <%s>",
                directory_norm.c_str()
        );

        if (!FileSystem::is_directory(directory_norm)) {
            return luaL_error(L, "Not a directory <%s>", directory_norm.c_str());
        }

        scope->get_local_api()->chdir(directory_norm);

        return 0;
    }

    int lua_api_local_exec(lua_State *L) {
        const auto *scope = get_scope(L);

        if (scope->has_current_api()) {
            auto debug = get_lua_debug(L);
            scope->get_context()->get_log_listener()->emit_warning(
                    "Executing local command with active remote scope in %s:%d. "
                    "Command will be executed for each server in context!",
                    debug.short_src,
                    debug.currentline
            );
        }

        int n_args = lua_gettop(L);

        if (1 != n_args && 2 != n_args) {
            return luaL_error(L, "Expected one or two arguments");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        bool print_output = true;
        if (n_args == 2) {
            if (!lua_isboolean(L, 2)) {
                return luaL_error(L, "Argument two is expected to be boolean");
            }
            print_output = static_cast<bool>(lua_toboolean(L, 2));
        }

        auto command = scope->replace_vars(luaL_checkstring(L, 1));
        auto result = scope->get_local_api()->local_popen(command, print_output);

        lua_pushstring(L, result.get_out().c_str());
        lua_pushinteger(L, result.get_code());

        return 2;
    }

    int lua_api_local_shell(lua_State *L) {
        const auto *scope = get_scope(L);

        if (scope->has_current_api()) {
            auto debug = get_lua_debug(L);
            scope->get_context()->get_log_listener()->emit_warning(
                    "Executing local command with active remote scope in %s:%d. "
                    "Command will be executed for each server in context!",
                    debug.short_src,
                    debug.currentline
            );
        }

        int n_args = lua_gettop(L);

        if (1 != n_args) {
            return luaL_error(L, "Expected one argument");
        }

        if (!lua_isstring(L, 1)) {
            return luaL_error(L, "Argument one is expected to be string");
        }

        auto command = scope->replace_vars(luaL_checkstring(L, 1));
        auto result = scope->get_local_api()->local_popen(command, true);

        lua_pushboolean(L, 0 == result.get_code());

        return 1;
    }

    extern "C" const struct luaL_Reg module_def[] = {
            {"require_api",     lua_api_level_require},
            {"task",            lua_api_task_define},
            {"add_inventory",   lua_api_inventory_add},
            {"on",              lua_api_on_role_invoke},
            {"within",          lua_api_remote_within},
            {"exec",            lua_api_remote_exec},
            {"shell",           lua_api_remote_shell},
            {"archive_dir_tmp", lua_api_archive_dir_tmp},
            {"archive_dir",     lua_api_archive_dir},
            {"upload_file",     lua_api_upload_file},
            {"download_file",   lua_api_download_file},
            {"upload_str",      lua_api_upload_str},
            {"download_str",    lua_api_download_str},
            {"define",          lua_api_define},
            {"strfvars",        lua_api_strfvars},
            {"strfenv",         lua_api_strfenv},
            {"getenv",         lua_api_getenv},
            {"local_exec",      lua_api_local_exec},
            {"local_shell",     lua_api_local_shell},
            {"local_within",    lua_api_local_within},
            {nullptr,           nullptr}
    };

    extern "C" int lua_module_init(lua_State *L) {
        const auto *scope = get_scope(L);

        lua_newtable(L);
        luaL_setfuncs(L, module_def, 0);

        lua_pushstring(L, LIBKAFE_VERSION);
        lua_setfield(L, -2, "version");

        lua_pushinteger(L, LIBKAFE_VERSION_MAJOR);
        lua_setfield(L, -2, "version_major");

        lua_pushinteger(L, LIBKAFE_VERSION_MINOR);
        lua_setfield(L, -2, "version_minor");

        lua_pushinteger(L, LIBKAFE_VERSION_RELEASE);
        lua_setfield(L, -2, "version_release");

        lua_pushinteger(L, LIBKAFE_API_LEVEL);
        lua_setfield(L, -2, "api_level");

        lua_pushstring(L, scope->get_context()->get_environment().c_str());
        lua_setfield(L, -2, "environment");

        lua_pushvalue(L, -1);

        return 1;
    }

    extern "C" void lua_bootstrap(lua_State *L) {
        const auto *scope = get_scope(L);
        auto extra_args = scope->get_extra_args();

        // Init "kafe" module
        luaL_requiref(L, LIBKAFE_LUA_MODULE_NAME, lua_module_init, false);
        lua_pop(L, 1);

        // Populate "arg"
        lua_createtable(L, extra_args.size() + 1, 0);
        int arg_table = lua_gettop(L);

        //// Push project file name
        lua_pushstring(L, scope->get_project_file().c_str());
        lua_rawseti(L, arg_table, 0);

        //// Push remaining args
        long long index = 1;
        for (auto arg: extra_args) {
            lua_pushstring(L, arg.c_str());
            lua_rawseti(L, arg_table, index);
            ++index;
        }
        lua_setglobal(L, "arg");
    }
    // END integration with Lua

    void Script::initialize() {
        lua_bootstrap(this->lua_state);
    }
}
