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

#include <fstream>
#include <iostream>

#include "kafe/io/archive.hpp"
#include "kafe/io/file_system.hpp"
#include "kafe/runtime/runtime_exception.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::runtime;

namespace kafe::io {
    static const int ARCHIVE_FILE_BUFFER_S = 4096;

    string Archive::tmp_archive_from_directory(const string &directory) {
        auto name = tmpnam(nullptr); // TODO replace
        auto upload_name = string(name) + ".tar.gz";
        archive_from_directory(upload_name, directory);
        return upload_name;
    }

    // TODO: implement .kafeignore
    // TODO: https://stackoverflow.com/questions/23330065/adding-directory-to-tarfile-with-libarchive
    void Archive::archive_from_directory(const string &archive_path, const string &directory) {
        if (!FileSystem::is_directory(directory)) {
            throw RuntimeException("Can not create archive - directory <%s> not found", directory.c_str());
        }

        if (FileSystem::is_file_or_symlink(archive_path)) {
            throw RuntimeException("Can not create archive - path <%s> exists", directory.c_str());
        }

        auto archive_dir_name = std_fs::path(archive_path).parent_path();

        if (FileSystem::exists(archive_dir_name)) {
            if (!FileSystem::is_directory(archive_dir_name)) {
                throw RuntimeException("Can not create archive - output directory <%s> not found",
                                       archive_dir_name.c_str());
            }
        } else {
            FileSystem::mkdirs(archive_dir_name);
        }

        std_fs::recursive_directory_iterator iter(directory), end;

        auto *archive = archive_write_new();
        archive_write_add_filter_gzip(archive);
        archive_write_set_format_pax_restricted(archive);
        archive_write_open_filename(archive, archive_path.c_str());

        while (iter != end) {
            auto path_abs = std_fs::absolute(iter->path());

            if (std_fs::is_directory(path_abs)) {
                ++iter;
                continue;
            }

            // TODO: this is not nice at all..
            auto path_rel = iter->path().string().substr(directory.size());

            if (path_rel[0] == '/') {
                path_rel = path_rel.substr(1, path_rel.size() - 1);
            }
            // ENDTODO

            auto size = std_fs::file_size(path_abs);

            struct stat buf{};
            stat(path_abs.c_str(), &buf);

            auto *entry = archive_entry_new();
            archive_entry_set_pathname(entry, path_rel.c_str());
            archive_entry_set_size(entry, size);
            archive_entry_set_filetype(entry, AE_IFREG);
            archive_entry_set_perm(entry, buf.st_mode);
//            archive_entry_set_mtime(entry, buf.st_mtim.tv_sec, 0);
            archive_write_header(archive, entry);
            archive_entry_copy_stat(entry, &buf);

            ifstream fin(path_abs.string(), ifstream::binary);
            char buffer[ARCHIVE_FILE_BUFFER_S];
            do {
                fin.read(buffer, ARCHIVE_FILE_BUFFER_S);
                archive_write_data(archive, buffer, fin.gcount());
            } while (fin);

            fin.close();
            archive_entry_free(entry);

            ++iter;
        }

        archive_write_close(archive);
        archive_write_free(archive);
    }
}
