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

#ifndef LIBKAFE_REMOTE_SSH_API_HPP
#define LIBKAFE_REMOTE_SSH_API_HPP

#include "kafe/logging.hpp"
#include "kafe/remote/ssh_manager.hpp"
#include "kafe/remote/ssh_session.hpp"

namespace kafe::remote {
    class RemoteResult {
        string out;
        string err;
        int code;

    public:
        RemoteResult(string &out, string &err, int code);

        [[nodiscard]] const string &get_stdout() const;

        [[nodiscard]] const string &get_stderr() const;

        [[nodiscard]] int get_code() const;
    };

    class SshApi {
        SshManager *manager;
        const ILogEventListener *log_listener;
        string current_chdir;

    public:
        SshApi(const SshManager *manager, const ILogEventListener *listener);

        void chdir(const string &string);

        [[nodiscard]] RemoteResult execute(const string &command, bool print_output) const;

        void scp_upload_file(const string &file, const string &remote_path) const;

        void scp_download_file(const string &file, const string &remote_path) const;

        [[nodiscard]] string scp_download_file_as_string(const string &remote_file) const;

        void scp_upload_file_from_string(const string &content, const string &remote_file) const;
    };
}

#endif
