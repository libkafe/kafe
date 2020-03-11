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

#ifndef LIBKAFE_REMOTE_SSH_SESSION_HPP
#define LIBKAFE_REMOTE_SSH_SESSION_HPP

#ifndef _LIBSSH_H
extern "C" {
#include "libssh/libssh.h"
#include "libssh/sftp.h"
}
#endif

#include <string>
#include <map>
#include "kafe/runtime/runtime_exception.hpp"
#include "kafe/logging.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::runtime;

namespace kafe::remote {
    class SshSessionException : public RuntimeException {
        using RuntimeException::RuntimeException;
    };

    class SshSession {
        ssh_session session;
    public:
        SshSession(const map<const string, const string> *envvals, const string &user, const string &host, unsigned int port, LogLevel level);

        [[nodiscard]] bool is_active() const;

        [[nodiscard]] ssh_session get_ssh_session() const;

        virtual ~SshSession();

        void close() const;
    };
}

#endif
