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

#ifndef LIBKAFE_IO_ARCHIVE_HPP
#define LIBKAFE_IO_ARCHIVE_HPP

extern "C" {
#include <archive.h>
#include <archive_entry.h>
}

#include <string>

using namespace std;

namespace kafe::io {
    class Archive {
    public:
        static string tmp_archive_from_directory(const string &directory, kafe::ILogEventListener *p_listener);

        static void
        archive_from_directory(const string &archive_path, const string &directory, ILogEventListener *p_listener);
    };
}

#endif
