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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-value-param"
#ifndef LIBKAFE_LOGGING_HPP
#define LIBKAFE_LOGGING_HPP

#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <utility>
#include <vector>
#include <cstring>
#include <chrono>

using namespace std;

namespace kafe {
    enum LogLevel {
        ALL = 0,
        TRACE = 1,
        DEBUG = 2,
        INFO = 3,
        SUCCESS = 4,
        WARNING = 5,
        ERROR = 6,
        NONE = 7
    };

    class LoggingTimer {
    private:
        chrono::time_point<chrono::system_clock> t_start;
        chrono::time_point<chrono::system_clock> t_end;
    public:
        LoggingTimer() {
            this->t_start = chrono::system_clock::now();
            this->t_end = t_start;
        }

        bool is_stopped() {
            return t_start != t_end;
        }

        LoggingTimer stop() {
            if (is_stopped()) {
                return *this;
            }
            t_end = chrono::system_clock::now();
            return *this;
        }

        [[nodiscard]] string duration() const {
            auto micros = chrono::duration_cast<chrono::microseconds>(t_end - t_start);
            auto duration_micros = micros.count();
            stringstream buffer;

            string unit;
            double duration;
            if (duration_micros < 1000) {
                unit = "μs";
                duration = (double) duration_micros;
            } else if (duration_micros < 100000) {
                unit = "ms";
                duration = (double) duration_micros / 1000;
            } else if (duration_micros < 1000000) {
                unit = "s";
                duration = (double) duration_micros / 1000 / 1000;
            }

            buffer << duration << unit;
            return buffer.str();
        }
    };

    class LogEvent {
        const LogLevel level;
        const string message;
        chrono::time_point<chrono::system_clock> event_time;
        const LoggingTimer *timer;
    public:
        LogEvent(
                const LogLevel level,
                string message,
                const LoggingTimer *timer
        ) : level(level), message(move(message)), timer(timer) {
            this->event_time = chrono::system_clock::now();
        }

        [[nodiscard]] LogLevel get_level() const {
            return level;
        }

        [[nodiscard]] const string &get_message() const {
            return message;
        }

        [[nodiscard]] chrono::time_point<chrono::system_clock> get_event_time() const {
            return event_time;
        }

        [[nodiscard]] const LoggingTimer *get_timer() const {
            return timer;
        }
    };

#define log_at_level \
    if (level < get_level()) { return; } \
    va_list args; \
    va_start(args, format); \
    char *buf; \
    vasprintf(&buf, format.c_str(), args); \
    va_end(args); \
    auto event = LogEvent(level, buf, nullptr); \
    on_log(event); \
    free(buf);

#define log_at_level_it \
    if (nullptr != timer && !timer->is_stopped()) { timer->stop(); } \
    if (level < get_level()) { return; } \
    va_list args; \
    va_start(args, format); \
    char *buf; \
    vasprintf(&buf, format.c_str(), args); \
    va_end(args); \
    auto event = LogEvent(level, buf, timer); \
    on_log(event); \
    free(buf);

#define log_at_level_wt \
    if (level < get_level()) { return timer(); } \
    va_list args; \
    va_start(args, format); \
    char *buf; \
    vasprintf(&buf, format.c_str(), args); \
    va_end(args); \
    auto event = LogEvent(level, buf, nullptr); \
    on_log(event); \
    free(buf); \
    return timer();

    class ILogEventListener {
        vector<string> context = {};

    protected:
        virtual void on_log(const LogEvent &event) const = 0;

    public:
        [[nodiscard]] vector<string> get_context() const {
            return context;
        }

    public:
        [[nodiscard]] LoggingTimer timer() const {
            return LoggingTimer();
        }

        void context_push(const string &ctx) {
            this->context.push_back(ctx);
        }

        void context_pop() {
            if (this->context.empty()) {
                return;
            }
            this->context.pop_back();
        }

        void emit_trace(const string format, ...) const {
            auto level = LogLevel::TRACE;
            log_at_level
        }

        void emit_trace(LoggingTimer *timer, const string format, ...) const {
            auto level = LogLevel::TRACE;
            log_at_level_it
        }

        LoggingTimer emit_trace_wt(const string format, ...) const {
            auto level = LogLevel::TRACE;
            log_at_level_wt
        }

        void emit_debug(const string format, ...) const {
            auto level = LogLevel::DEBUG;
            log_at_level
        }

        void emit_debug(LoggingTimer *timer, const string format, ...) const {
            auto level = LogLevel::DEBUG;
            log_at_level_it
        }

        LoggingTimer emit_debug_wt(const string format, ...) const {
            auto level = LogLevel::DEBUG;
            log_at_level_wt
        }

        void emit_info(const string format, ...) const {
            auto level = LogLevel::INFO;
            log_at_level
        }

        void emit_info(LoggingTimer *timer, const string format, ...) const {
            auto level = LogLevel::INFO;
            log_at_level_it
        }

        LoggingTimer emit_info_wt(const string format, ...) const {
            auto level = LogLevel::INFO;
            log_at_level_wt
        }

        void emit_success(const string format, ...) const {
            auto level = LogLevel::SUCCESS;
            log_at_level
        }

        void emit_success(LoggingTimer *timer, const string format, ...) const {
            auto level = LogLevel::SUCCESS;
            log_at_level_it
        }

        LoggingTimer emit_success_wt(const string format, ...) const {
            auto level = LogLevel::SUCCESS;
            log_at_level_wt
        }

        void emit_warning(const string format, ...) const {
            auto level = LogLevel::WARNING;
            log_at_level
        }

        void emit_warning(LoggingTimer *timer, const string format, ...) const {
            auto level = LogLevel::WARNING;
            log_at_level_it
        }

        LoggingTimer emit_warning_wt(const string format, ...) const {
            auto level = LogLevel::WARNING;
            log_at_level_wt
        }

        void emit_error(const string format, ...) const {
            auto level = LogLevel::ERROR;
            log_at_level
        }

        void emit_error(LoggingTimer *timer, const string format, ...) const {
            auto level = LogLevel::ERROR;
            log_at_level_it
        }

        LoggingTimer emit_error_wt(const string format, ...) const {
            auto level = LogLevel::ERROR;
            log_at_level_wt
        }

        virtual void on_stdout_line(string line) const = 0;

        virtual void on_stderr_line(string line) const = 0;

        virtual void on_stdout_line(string prefix, string line) const = 0;

        virtual void on_stderr_line(string prefix, string line) const = 0;

        [[nodiscard]] virtual FILE *get_stdout() const = 0;

        [[nodiscard]] virtual FILE *get_stderr() const = 0;

        [[nodiscard]] virtual LogLevel get_level() const = 0;
    };
}
#endif

#pragma clang diagnostic pop