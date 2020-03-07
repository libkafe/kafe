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

#include "kafe/remote/ssh_pool.hpp"

namespace kafe::remote {
    SshPool::~SshPool() {
        for (const auto&[key, value] : sessions) {
            value->close();
            delete (value);
        }

        sessions.clear();
    }

    bool SshPool::has_session(const string &remote_id) const {
        return this->sessions.find(remote_id) != this->sessions.end();
    }

    SshSession *SshPool::get_session(const string &remote_id) const {
        if (!has_session(remote_id)) {
            return nullptr;
        }

        return const_cast<SshSession *>(this->sessions.find(remote_id)->second);
    }

    void SshPool::add_session(const string &remote_id, const SshSession *session) {
        auto entry = pair<const string, const SshSession *>(remote_id, session);
        this->sessions.insert(entry);
    }

    void SshPool::remove_session(const string &remote_id) {
        auto candidate = sessions.find(remote_id);

        if (candidate == sessions.end()) {
            return;
        }

        sessions.erase(remote_id);
        delete (candidate->second);
    }
}