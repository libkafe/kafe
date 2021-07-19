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

#include <random>
#include "kafe/io/file_system.hpp"
#include "kafe/runtime/runtime_exception.hpp"

using namespace kafe::runtime;
using namespace kafe::io;

#define K_MAX_TRIES_TMP 1000

namespace kafe::io {
    std_fs::path FileSystem::tmp_dir(const string &prefix) {
        auto tmp_dir = std::filesystem::temp_directory_path();
        unsigned long long i = 0;

        mt19937 rng(random_device{}());
        uniform_int_distribution<unsigned long long> rand;
        std_fs::path path;

        while (true) {
            stringstream str_st;
            str_st << hex << rand(rng);
            path = tmp_dir / (prefix + str_st.str());

            if (std_fs::create_directory(path)) {
                break;
            }

            if (K_MAX_TRIES_TMP == i) {
                throw RuntimeException("Could not create random directory");
            }

            i++;
        }

        return path;
    }

    bool FileSystem::is_file_or_symlink(const string &path_s) {
        return (std_fs::is_regular_file(path_s) || std_fs::is_symlink(path_s))
               && !std_fs::is_block_file(path_s);
    }

    bool FileSystem::is_directory(const string &path_s) {
        return std_fs::is_directory(std_fs::absolute(path_s));
    }

    bool FileSystem::exists(const string &path) {
        return std_fs::exists(path);
    }

    bool FileSystem::mkdirs(const string &path) {
        return std_fs::create_directories(path);
    }

    void FileSystem::try_rm_r(const string &path) {
        if (path.empty()) {
            return;
        }

        if (!exists(path)) {
            return;
        }

        auto fs_path_abs = std_fs::absolute(path);

        if (std_fs::is_symlink(fs_path_abs)) {
            fs_path_abs = std_fs::absolute(std_fs::read_symlink(fs_path_abs));
        }

        if ("/" == fs_path_abs) {
            // TODO warn about the dire consequences of stupidity...
            return;
        }

        if (is_directory(fs_path_abs)) {
            std_fs::remove_all(fs_path_abs);
            return;
        }

        std_fs::remove(fs_path_abs);
    }

    /**
     * This method has been copied from Boost filesystem, version 1.65.0
     */
    std_fs::path FileSystem::absolute(const std_fs::path &p, const std_fs::path &base) {
        //  recursively calling absolute is sub-optimal, but is sure and simple
        std_fs::path abs_base(base.is_absolute() ? base : FileSystem::absolute(base, std_fs::current_path()));

        //  store expensive to compute values that are needed multiple times
        std_fs::path p_root_name(p.root_name());
        std_fs::path base_root_name(abs_base.root_name());
        std_fs::path p_root_directory(p.root_directory());

        if (p.empty())
            return abs_base;

        if (!p_root_name.empty())  // p.has_root_name()
        {
            if (p_root_directory.empty())  // !p.has_root_directory()
                return p_root_name / abs_base.root_directory()
                       / abs_base.relative_path() / p.relative_path();
            // p is absolute, so fall through to return p at end of block
        } else if (!p_root_directory.empty())  // p.has_root_directory()
        {
#     ifdef BOOST_POSIX_API
            // POSIX can have root name it it is a network path
      if (base_root_name.empty())   // !abs_base.has_root_name()
        return p;
#     endif
            return base_root_name / p;
        } else {
            return abs_base / p;
        }

        return p;  // p.is_absolute() is true
    }

    std_fs::path FileSystem::expand(const std_fs::path &p) {
        if (p.empty()) { return p; }
        // TODO: how does this affect env of the script? perhaps need an arg with home dir?
#ifdef _GNU_SOURCE
        const char *home = secure_getenv("HOME");
#else
        const char *home = getenv("HOME");
#endif
        if (home == nullptr) {
            throw RuntimeException("HOME environment variable not set - can not expand home directory");
        }

        string s = p.c_str();
        if ('~' == s[0]) {
            s = string(home) + s.substr(1, s.size() - 1);
            return std_fs::path(s);
        } else {
            return p;
        }
    }

    std_fs::path FileSystem::normalize(const std_fs::path &p, const std_fs::path &base) {
        auto base_p = base.empty() ? std_fs::current_path() : base;

        if (p.empty()) {
            return absolute(p, base_p);
        }

        auto abs_p = absolute(expand(p), base_p);

        try {
            return std_fs::canonical(abs_p);
        } catch (exception &e) {
            return abs_p;
        }
    }
}
