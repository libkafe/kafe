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

#ifndef LIBKAFE_SCRIPTING_SCRIPT_HPP
#define LIBKAFE_SCRIPTING_SCRIPT_HPP
#define LIBKAFE_LUA_MODULE_NAME "kafe"

#include <string>
#include "kafe/scripting/luainc.hpp"
#include "kafe/runtime/runtime_exception.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::runtime;

namespace kafe::scripting {
    class ScriptEngineException : public RuntimeException {
        using RuntimeException::RuntimeException;
    };

    class ScriptEvaluationException : public ScriptEngineException {
        using ScriptEngineException::ScriptEngineException;
    };

    class ScriptInvocationException : public ScriptEngineException {
        using ScriptEngineException::ScriptEngineException;
    };

    class Script {
        const ExecutionScope &scope;
        lua_State *lua_state;

    public:
        explicit Script(const ExecutionScope &scope);

        virtual ~Script();

        void load_file(const string &script_file);

        void evaluate();

        void invoke_function_by_ref(int reference, const vector<string> &extra_args);

    private:
        void initialize();
    };
}

#endif
