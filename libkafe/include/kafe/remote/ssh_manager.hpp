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

#ifndef LIBKAFE_REMOTE_SSH_MANAGER_HPP
#define LIBKAFE_REMOTE_SSH_MANAGER_HPP

#include "kafe/project/inventory.hpp"
#include "kafe/remote/ssh_pool.hpp"
#include "kafe/remote/ssh_session.hpp"
#include "kafe/logging.hpp"

using namespace kafe;
using namespace kafe::remote;
using namespace kafe::project;

namespace kafe::remote {
    class SshManager {
        SshPool *pool;
        const map<const string, const string> *envvals;
        const InventoryItem *item;
    public:
        SshManager(const SshPool *pool, const map<const string, const string> *envvals, const InventoryItem *item);

        const SshSession *get_or_create_session(LogLevel level);
    };
}

#endif
