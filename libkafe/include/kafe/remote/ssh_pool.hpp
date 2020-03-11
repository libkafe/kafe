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

#ifndef LIBKAFE_REMOTE_SSH_POOL_HPP
#define LIBKAFE_REMOTE_SSH_POOL_HPP

#include <string>
#include <map>
#include "kafe/remote/ssh_session.hpp"

using namespace std;

namespace kafe::remote {
    class SshPool {
        map<const string, const SshSession *> sessions = {};

    public:
        virtual ~SshPool();

        [[nodiscard]] bool has_session(const string &remote_id) const;

        [[nodiscard]] SshSession *get_session(const string &remote_id) const;

        void remove_session(const string &remote_id);

        void add_session(const string &remote_id, const SshSession *session);
    };
}

#endif
