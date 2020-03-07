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

#ifndef LIBKAFE_SCRIPTING_LUAINC_HPP
#define LIBKAFE_SCRIPTING_LUAINC_HPP
extern "C" {
#ifdef KAFE_WITH_LUA_54
#ifdef KAFE_WITH_LUA_54_VERSIONED
extern "C" {
    #include "lua5.4/lua.h"
    #include "lua5.4/lualib.h"
    #include "lua5.4/lauxlib.h"
}
#elif defined(KAFE_WITH_LUA_ROOT)
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#else
#error "Unable to resolve Lua header file to use"
#endif
#endif

#ifdef KAFE_WITH_LUA_53
#ifdef KAFE_WITH_LUA_53_VERSIONED
extern "C" {
    #include "lua5.3/lua.h"
    #include "lua5.3/lualib.h"
    #include "lua5.3/lauxlib.h"
}

#elif defined(KAFE_WITH_LUA_ROOT)
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#else
#error "Unable to resolve Lua header file to use"
#endif
#endif

#if not defined(KAFE_WITH_LUA_51) && not defined(KAFE_WITH_LUA_52) && not defined(KAFE_WITH_LUA_53) && not defined(KAFE_WITH_LUA_54)
#error "No suitable Lua found"
#endif
}
#endif
