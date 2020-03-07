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

#ifndef LIBKAFE_IO_FILE_SYSTEM_HPP
#define LIBKAFE_IO_FILE_SYSTEM_HPP

#ifdef KAFE_WITH_FILESYSTEM
#include <filesystem>

namespace kafe::io {
    namespace std_fs = std::filesystem;
}

#elif defined(KAFE_WITH_FILESYSTEM_EXPERIMENTAL)

#include <experimental/filesystem>

namespace kafe::io {
    namespace std_fs = std::experimental::filesystem;
}
#else
#error "No filesystem library"
#endif

using namespace std;
using namespace kafe;

namespace kafe::io {
    class FileSystem {
    public:
        static bool is_file_or_symlink(const string &path_s);

        static bool is_directory(const string &path_s);

        static bool exists(const string &path);

        static bool mkdirs(const string &path);

        static void try_rm_r(const string &path);

        static std_fs::path absolute(const std_fs::path &p, const std_fs::path &base);

        static std_fs::path expand(const std_fs::path &p);

        static std_fs::path normalize(const std_fs::path &p, const std_fs::path &base);
    };
}

#endif