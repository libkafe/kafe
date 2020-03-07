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

#include <cstdio>
#include <string>
#include <cstring>
#include <utility>
#include "kafe/local/local_api.hpp"
#include "kafe/io/file_system.hpp"
#include "kafe/logging.hpp"

using namespace std;
using namespace kafe::io;

namespace kafe::local {
    LocalShellResult::LocalShellResult(string out, const int code) : out(move(out)), code(code) {}

    string LocalShellResult::get_out() const {
        return out;
    }

    int LocalShellResult::get_code() const {
        return code;
    }

    LocalApi::LocalApi(const ILogEventListener *log_listener) : log_listener(log_listener) {
    }

    string LocalApi::read_out(FILE *pFile, bool print_output) {
        const size_t buffer_size = 256;
        size_t output_size = 4096;

        char *output = (char *) malloc(output_size);
        unsigned long current_pos = 0;
        unsigned long last_line_pos = 0;
        size_t n_read;
        char buffer[buffer_size];
        char *line_buffer = (char *) malloc(256);
        do {
            n_read = fread(buffer, 1, buffer_size, pFile);
            if (n_read > 0) {
                for (int i = 0; i < n_read; i++) {
                    auto c = buffer[i];
                    output[current_pos] = c;

                    if (print_output && c == '\n') {
                        const unsigned long size_of = sizeof(char) * (current_pos - last_line_pos + 1);
                        line_buffer = (char *) realloc(line_buffer, size_of + 1);
                        memcpy(line_buffer, output + last_line_pos, size_of);
                        line_buffer[size_of] = '\0';

                        log_listener->on_stdout_line("-> out", line_buffer);

                        last_line_pos = current_pos + 1;
                    }

                    current_pos++;

                    if (current_pos > output_size) {
                        output_size += buffer_size;
                        output = (char *) realloc(output, output_size);
                    }
                }
            }
        } while (n_read > 0);

        // Strip last line
        if (output[current_pos - 1] == '\n') {
            output[current_pos - 1] = '\0';
        } else {
            output[current_pos] = '\0';
        }

        auto result = string(output);

        free(line_buffer);
        free(output);

        return result;
    }

    // TODO: this works, but does not capture stderr for obvious reasons...
    LocalShellResult LocalApi::local_popen(const string &command, bool print_output) {
        string cmd;
        LoggingTimer timer;

        if (!this->current_chdir.empty()) {
            timer = log_listener->emit_info_wt(
                    "In directory <%s> executing <%s>", this->current_chdir.c_str(), command.c_str());
            cmd = "cd " + this->current_chdir + " && " + string(command);
        } else {
            timer = log_listener->emit_info_wt("Executing local command %s", command.c_str());
            cmd = string(command);
        }

        log_listener->emit_debug("Full local shell command is <%s>", cmd.c_str());

        FILE *p = ::popen(cmd.c_str(), "r");

        if (nullptr == p) {
            return LocalShellResult("", -1);
        }

        auto output = read_out(p, print_output);
        int exit_code = pclose(p);

        if (0 == exit_code) {
            log_listener->emit_info(&timer, "Local command complete");
        } else {
            log_listener->emit_warning(&timer, "Local command complete with non-zero exit code <%d>", exit_code);
        }

        return LocalShellResult(output, exit_code);
    }

    void LocalApi::chdir(const string &chdir) {
        this->current_chdir = FileSystem::normalize(chdir, std_fs::current_path());
    }

    const string &LocalApi::get_chdir() const {
        return current_chdir;
    }
}