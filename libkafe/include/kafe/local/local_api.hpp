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

#ifndef LIBKAFE_LOCAL_LOCAL_API_HPP
#define LIBKAFE_LOCAL_LOCAL_API_HPP

#include <string>
#include "kafe/logging.hpp"

using namespace std;
using namespace kafe;

namespace kafe::local {
    class LocalShellResult {
        const string out;
        const int code;

    public:
        LocalShellResult(string out, int code);

        [[nodiscard]] string get_out() const;

        [[nodiscard]] int get_code() const;
    };

    class LocalApi {
        const ILogEventListener *log_listener;
        string current_chdir;
    private:
        string read_out(FILE *pFile, bool print_output);

    public:
        explicit LocalApi(const ILogEventListener *log_listener);

        LocalShellResult local_popen(const string &command, bool print_output);

        void chdir(const string &chdir);

        [[nodiscard]] const string &get_chdir() const;
    };
}
#endif
