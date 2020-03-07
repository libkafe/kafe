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

#ifndef LIBKAFE_RUNTIME_RUNTIME_EXCEPTION_HPP
#define LIBKAFE_RUNTIME_RUNTIME_EXCEPTION_HPP

#include <exception>
#include <string>

using namespace std;

namespace kafe::runtime {
    class RuntimeException : public exception {
        string message;

    public:
        RuntimeException();

        explicit RuntimeException(const char *format, ...);

        explicit RuntimeException(const string *format, ...);

        [[nodiscard]] const char *what() const noexcept override;
    };
}

#endif