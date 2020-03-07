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

#include "kafe/remote/ssh_session.hpp"

namespace kafe::remote {
    SshSession::SshSession(const string &user, const string &host, int port, LogLevel level) {
        ssh_session session_new = ssh_new();

        int verbosity;
        if (level == LogLevel::ALL) {
            verbosity = SSH_LOG_FUNCTIONS;
        } else if (level == LogLevel::TRACE) {
            verbosity = SSH_LOG_PROTOCOL;
        } else {
            verbosity = SSH_LOG_NOLOG;
        }

        ssh_options_set(session_new, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
        ssh_options_set(session_new, SSH_OPTIONS_USER, user.c_str());
        ssh_options_set(session_new, SSH_OPTIONS_HOST, host.c_str());
        ssh_options_set(session_new, SSH_OPTIONS_PORT, &port);

        auto result = ssh_connect(session_new);

        if (SSH_OK != result) {
            throw SshSessionException();
        }

        auto is_known = ssh_is_server_known(session_new);
        if (is_known != SSH_SERVER_KNOWN_OK) {
            throw SshSessionException();
        }

        // TODO: load passphrase from env properties
        auto is_auth = ssh_userauth_publickey_auto(session_new, nullptr, nullptr);

        // TODO: verify
        if (is_auth != SSH_AUTH_SUCCESS) {
            throw SshSessionException();
        }

        this->session = session_new;
    }

    SshSession::~SshSession() {
        if (is_active()) {
            ssh_free(this->session);
        }
    }

    bool SshSession::is_active() const {
        return static_cast<bool>(ssh_is_connected(session));
    }

    ssh_session SshSession::get_ssh_session() const {
        return session;
    }

    void SshSession::close() const {
        if (ssh_is_connected(session)) {
            ssh_disconnect(session);
        }
    }
}