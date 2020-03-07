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

#ifndef LIBKAFE_PROJECT_INVENTORY_HPP
#define LIBKAFE_PROJECT_INVENTORY_HPP

#include <string>
#include <list>
#include <set>
#include "kafe/runtime/runtime_exception.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::runtime;

namespace kafe::project {
    class DuplicateInventoryException : public RuntimeException {
        using RuntimeException::RuntimeException;
    };

    class InventoryItem {
        const string user;
        const string host;
        unsigned int port;
        const string environment;
        const string role;

    public:
        InventoryItem(
                string user,
                string host,
                unsigned int port,
                string environment,
                string role
        );

        [[nodiscard]] const string &get_environment() const;

        [[nodiscard]] const string &get_role() const;

        [[nodiscard]] const string &get_user() const;

        [[nodiscard]] const string &get_host() const;

        [[nodiscard]] unsigned int get_port() const;

        [[nodiscard]] bool same_as(const InventoryItem &inventoryItem) const;

        [[nodiscard]] string to_string() const;

        [[nodiscard]] string remote_id() const;
    };

    class Inventory {
        list<const InventoryItem *> items;

    public:
        virtual ~Inventory();

        [[nodiscard]] bool item_exists(const InventoryItem &item) const;

        void add(const InventoryItem &item);

        [[nodiscard]] list<const InventoryItem *> find_for_scope(
                const string &environment,
                const string &role
        ) const;
    };
}
#endif
