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

#include <string>
#include <iomanip>
#include <cstring>
#include <iostream>
#include <kafe/io/tty.hpp>
#include "logger.hpp"

using namespace std;
using namespace kafe;

namespace kafe {
    static LogLevel get_env_log_level() {
        char *level = getenv("KAFE_LOG_LEVEL");

        if (nullptr == level) {
            return LogLevel::INFO;
        }

        int i_level;
        try {
            i_level = stoi(level);
        } catch (exception &e) {
            std::cerr << "Logger error: unable to set log level from environment, expected integer level, got <"
                      << level
                      << ">"
                      << endl;
            return LogLevel::INFO;
        }

        if (LogLevel::ALL > i_level || LogLevel::NONE < i_level) {
            std::cerr << "Logger error: invalid log level " << level << ", ignoring" << endl;
        } else {
            return static_cast<LogLevel>(i_level);
        }

        return LogLevel::INFO;
    }

    static string level_to_string(LogLevel level) {
        switch (level) {
            case ALL:
                return "    ALL";
            case NONE:
                return "   NONE";
            case TRACE:
                return "  TRACE";
            case DEBUG:
                return "  DEBUG";
            case INFO:
                return "   INFO";
            case SUCCESS:
                return "SUCCESS";
            case WARNING:
                return "WARNING";
            case ERROR:
                return "  ERROR";
        }
    }

    static string context_to_s(const vector<string> &context) {
        if (context.empty()) {
            return {};
        }

        ostringstream ss;

        ss << IO_TTY_ANSI_COLOR_MAGENTA;

        for (const auto &ctx : context) {
            ss << '[' << ctx << "] ";
        }

        ss << IO_TTY_ANSI_COLOR_RESET;

        return ss.str();
    }

    inline string expand_prefix(const string& level) {
        char buffer[sizeof(char) * 18];
        sprintf(buffer, "%*s", 18, level.c_str());
        return string(buffer);
    }

    void Logger::on_log(const LogEvent &event) const {
        string color;

        switch (event.get_level()) {
            case NONE:
            case ALL:
            case TRACE:
            case DEBUG:
                color = {};
                break;
            case INFO:
                color = IO_TTY_ANSI_COLOR_BLUE;
                break;
            case SUCCESS:
                color = IO_TTY_ANSI_COLOR_GREEN;
                break;
            case WARNING:
                color = IO_TTY_ANSI_COLOR_YELLOW;
                break;
            case ERROR:
                color = IO_TTY_ANSI_COLOR_RED;
                break;
        }

        flockfile(stdout);
        flockfile(stderr);

        ostringstream time_stream;

        auto etime = chrono::system_clock::to_time_t(event.get_event_time());
        auto tm = *std::localtime(&etime);
        time_stream << put_time(&tm, "[%T]");
        const auto *timer = event.get_timer();

        auto context_s = context_to_s(get_context());

        if (nullptr == timer) {
            fprintf(stdout,
                    "%s %s%s%s %s%s\n",
                    time_stream.str().c_str(),
                    color.c_str(),
                    level_to_string(event.get_level()).c_str(),
                    IO_TTY_ANSI_COLOR_RESET,
                    context_s.c_str(),
                    event.get_message().c_str()
            );
        } else {
            fprintf(stdout,
                    "%s %s%s%s %s%s %s[%s]%s\n",
                    time_stream.str().c_str(),
                    color.c_str(),
                    level_to_string(event.get_level()).c_str(),
                    IO_TTY_ANSI_COLOR_RESET,
                    context_s.c_str(),
                    event.get_message().c_str(),
                    IO_TTY_ANSI_COLOR_MAGENTA,
                    timer->duration().c_str(),
                    IO_TTY_ANSI_COLOR_RESET
            );
        }

        funlockfile(stdout);
        funlockfile(stderr);
    }

    void Logger::on_stdout_line(string line) const {
        on_stdout_line("stdout", line);
    }

    void Logger::on_stdout_line(string prefix, string line) const {
        size_t len = line.length();

        if (0 == len) {
            return;
        }

        flockfile(stdout);
        flockfile(stderr);
        fprintf(
                stdout,
                "%s%s%s %s%s",
                IO_TTY_ANSI_COLOR_GREEN,
                expand_prefix(prefix).c_str(),
                IO_TTY_ANSI_COLOR_RESET,
                context_to_s(get_context()).c_str(),
                line.c_str()
        );

        if (len > 0 && line[len - 1] != '\n') {
            fputs("\n", stdout);
        }

        fflush(stdout);
        funlockfile(stdout);
        funlockfile(stderr);
    }

    void Logger::on_stderr_line(string line) const {
        on_stderr_line("stderr", line);
    }

    void Logger::on_stderr_line(string prefix, string line) const {
        size_t len = line.length();

        if (0 == len) {
            return;
        }

        flockfile(stdout);
        flockfile(stderr);
        fprintf(
                stderr,
                "%s%s%s %s%s",
                IO_TTY_ANSI_COLOR_YELLOW,
                expand_prefix(prefix).c_str(),
                IO_TTY_ANSI_COLOR_RESET,
                context_to_s(get_context()).c_str(),
                line.c_str()
        );

        if (len > 1 && line[len - 1] != '\n') {
            fputs("\n", stderr);
        }

        funlockfile(stdout);
        funlockfile(stderr);
    }

    FILE *Logger::get_stdout() const {
        return stdout;
    }

    FILE *Logger::get_stderr() const {
        return stderr;
    }

    LogLevel Logger::get_level() const {
        return get_env_log_level();
    }
}
