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

#include "kafe/project/inventory.hpp"
#include <utility>

namespace kafe::project {
    InventoryItem::InventoryItem(
            string user,
            string host,
            unsigned int port,
            string environment,
            string role
    ) : environment(move(environment)), role(move(role)), user(move(user)), host(move(host)), port(port) {}

    const string &InventoryItem::get_environment() const {
        return environment;
    }

    const string &InventoryItem::get_role() const {
        return role;
    }

    const string &InventoryItem::get_user() const {
        return user;
    }

    const string &InventoryItem::get_host() const {
        return host;
    }

    unsigned int InventoryItem::get_port() const {
        return port;
    }

    bool InventoryItem::same_as(const InventoryItem &other) const {
        return other.get_user() == user
               && other.get_host() == host
               && other.get_port() == port
               && other.get_environment() == environment
               && other.get_role() == role;
    }

    string InventoryItem::to_string() const {
        return get_environment() + "+" + role + "://"
               + get_user() + "@" + get_host() + ":" + ::to_string(get_port());
    }

    string InventoryItem::remote_id() const {
        return get_user() + "@" + get_host() + ":" + ::to_string(get_port());
    }

    bool Inventory::item_exists(const InventoryItem &item) const {
        for (const InventoryItem *local : this->items) {
            if (local->same_as(item)) {
                return true;
            }
        }
        return false;
    }

    void Inventory::add(const InventoryItem &item) {
        if (item_exists(item)) {
            throw DuplicateInventoryException("Duplicate inventory item <%s>", item.to_string().c_str());
        }

        this->items.push_back(&item);
    }

    list<const InventoryItem *> Inventory::find_for_scope(const string &environment, const string &role) const {
        list<const InventoryItem *> scoped;

        for (const InventoryItem *item : this->items) {
            if (item->get_environment() != environment) {
                continue;
            }

            const auto &item_role = item->get_role();

            if (item_role != role) {
                continue;
            }

            scoped.push_back(item);
        }

        return scoped;
    }

    Inventory::~Inventory() {
        for (auto item : items) {
            delete (item);
        }
        items.clear();
    }
}

