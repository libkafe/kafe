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
    SshSession::SshSession(const map<const string, const string> *envvals, const string &user, const string &host,
                           unsigned int port, LogLevel level) {
        ssh_session session_new = this->session = ssh_new();

        int verbosity = SSH_LOG_NOLOG;
        if (level == LogLevel::ALL) {
            verbosity = SSH_LOG_FUNCTIONS;
        } else if (level == LogLevel::TRACE) {
            verbosity = SSH_LOG_PROTOCOL;
        }

        ssh_options_set(session_new, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
        ssh_options_set(session_new, SSH_OPTIONS_USER, user.c_str());
        ssh_options_set(session_new, SSH_OPTIONS_HOST, host.c_str());
        ssh_options_set(session_new, SSH_OPTIONS_PORT, &port);

        auto result = ssh_connect(session_new);

        if (SSH_OK != result) {
            const auto *error = ssh_get_error(session_new);
            throw SshSessionException("Remote connection failed. %s", error);
        }

#if LIBSSH_VERSION_INT >= SSH_VERSION_INT(0, 8, 4)
        auto is_known = ssh_session_is_known_server(session_new);
        auto is_known_b = SSH_KNOWN_HOSTS_OK == is_known;
#else
        auto is_known = ssh_is_server_known(session_new);
        auto is_known_b = SSH_SERVER_KNOWN_OK == is_known;
#endif
        if (!is_known_b) {
            throw SshSessionException(
                    "Cowardly refusing to connect to <%s:%d> - remote host key verification failed. "
                    "You are required to verify this host and add it to known hosts independently before "
                    "any attempts to connect to it using Kafe.",
                    host.c_str(),
                    port
            );
        }

        string key_passphrase;
        auto env_key_passphrase = envvals->find("KAFE_SSH_PKEY_PASS");
        if (env_key_passphrase != envvals->end()) {
            key_passphrase = env_key_passphrase->second;
        }

        string user_passphrase;
        auto env_user_passphrase = envvals->find("KAFE_SSH_USER_PASS");
        if (env_user_passphrase != envvals->end()) {
            user_passphrase = env_user_passphrase->second;
        }

        auto req_retr_auth_none = ssh_userauth_none(session_new, nullptr);

        if (SSH_AUTH_ERROR == req_retr_auth_none) {
            throw SshSessionException("Failed to fetch list of methods for host <%s:%d>", host.c_str(), port);
        }

        unsigned int methods = ssh_userauth_list(session_new, nullptr);
        int is_auth;

        if (SSH_AUTH_METHOD_UNKNOWN == methods) {
            // Fallback to public key IF no method listed
            is_auth = ssh_userauth_publickey_auto(
                    session_new,
                    nullptr,
                    key_passphrase.empty() ? nullptr : key_passphrase.c_str()
            );

            if (SSH_AUTH_SUCCESS == is_auth) {
                return;
            }

            throw SshSessionException("Authentication failed for host <%s:%d>.", host.c_str(), port);
        }

        if (methods & (unsigned int) SSH_AUTH_METHOD_PUBLICKEY) {
            is_auth = ssh_userauth_publickey_auto(
                    session_new,
                    nullptr,
                    key_passphrase.empty() ? nullptr : key_passphrase.c_str()
            );
            if (SSH_AUTH_SUCCESS == is_auth) {
                return;
            }
        }

        if (methods & (unsigned int) SSH_AUTH_METHOD_GSSAPI_MIC) {
            is_auth = ssh_userauth_gssapi(session_new);
            if (SSH_AUTH_SUCCESS == is_auth) {
                return;
            }
        }

        if ((methods & (unsigned int) SSH_AUTH_METHOD_PASSWORD) && !user_passphrase.empty()) {
            is_auth = ssh_userauth_password(session_new, nullptr, user_passphrase.c_str());
            if (SSH_AUTH_SUCCESS == is_auth) {
                return;
            }
        }

        const auto *error = ssh_get_error(session_new);
        throw SshSessionException("Authentication failed for host <%s:%d>. %s", host.c_str(), port, error);
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
        ssh_free(session);
    }
}