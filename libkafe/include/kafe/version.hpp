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

#ifndef LIBKAFE_VERSION_HPP
#define LIBKAFE_VERSION_HPP

#ifndef LIBKAFE_SCRIPTING_LUAINC_HPP

#include "kafe/scripting/luainc.hpp"

#endif
extern "C" {
#ifndef ARCHIVE_H_INCLUDED
#include <archive.h>
#endif
#ifndef __CURL_CURLVER_H
#include <curl/curlver.h>
#endif
#ifndef INCLUDE_git_version_h__
#include <git2/version.h>
#endif
#ifndef _LIBSSH_H
#include <libssh/libssh.h>
#endif
}

#define LIBKAFE_VERSION KAFE_CMAKE_LIB_VERSION
#define LIBKAFE_BUILD_TS KAFE_CMAKE_LIB_BUILD_TS
#define LIBKAFE_VERSION_INT KAFE_CMAKE_LIB_VERSION_INT
#define LIBKAFE_API_LEVEL KAFE_CMAKE_LIB_API_VERSION
#define LIBKAFE_VERSION_MAJOR KAFE_CMAKE_LIB_VERSION_MAJOR
#define LIBKAFE_VERSION_MINOR KAFE_CMAKE_LIB_VERSION_MINOR
#define LIBKAFE_VERSION_RELEASE KAFE_CMAKE_LIB_VERSION_RELEASE

// Vendor versions
#ifdef ARCHIVE_VERSION_ONLY_STRING
#define LIBKAFE_VERSION_LIB_ARCHIVE ARCHIVE_VERSION_ONLY_STRING
#elif defined(ARCHIVE_VERSION_STRING)
#define LIBKAFE_VERSION_LIB_ARCHIVE ARCHIVE_VERSION_STRING
#else
#define LIBKAFE_VERSION_LIB_ARCHIVE "???"
#endif

#define LIBKAFE_VERSION_LIB_LUA LUA_VERSION
#define LIBKAFE_VERSION_LIB_SSH SSH_STRINGIFY(LIBSSH_VERSION)
#define LIBKAFE_VERSION_LIB_CURL LIBCURL_VERSION
#define LIBKAFE_VERSION_LIB_GIT2 LIBGIT2_VERSION

#endif