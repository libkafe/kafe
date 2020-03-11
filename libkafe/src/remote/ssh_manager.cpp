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

#include "kafe/remote/ssh_manager.hpp"

namespace kafe::remote {
    SshManager::SshManager(const SshPool *pool, const map<const string, const string> *envvals, const InventoryItem *item)
            : pool(const_cast<SshPool *>(pool)), envvals(envvals), item(item) {
    }

    const SshSession *SshManager::get_or_create_session(LogLevel level) {
        auto remote_id = item->remote_id();

        if (pool->has_session(remote_id)) {
            auto *current_session = pool->get_session(remote_id);

            if (current_session->is_active()) {
                return current_session;
            }

            pool->remove_session(remote_id);
        }

        auto *session = new SshSession(
                envvals,
                item->get_user(),
                item->get_host(),
                item->get_port(),
                level
        );

        pool->add_session(remote_id, session);

        return session;
    }
}