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

#ifndef KAFE_CLI_LOGGER_HPP
#define KAFE_CLI_LOGGER_HPP

#include <kafe/logging.hpp>

namespace kafe {
    class Logger : public ILogEventListener {
    public:
        [[nodiscard]] LogLevel get_level() const override;

    private:
        void on_log(const LogEvent &event) const override;

        void on_stdout_line(string line) const override;

        void on_stdout_line(string prefix, string line) const override;

        void on_stderr_line(string line) const override;

        void on_stderr_line(string prefix, string line) const override;

        [[nodiscard]] FILE *get_stdout() const override;

        [[nodiscard]] FILE *get_stderr() const override;
    };
}


#endif
