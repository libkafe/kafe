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

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include "kafe/remote/ssh_api.hpp"
#include "kafe/io/file_system.hpp"

using namespace kafe;
using namespace kafe::io;

namespace kafe::remote {
    RemoteResult::RemoteResult(string &out, string &err, int code) : out(out), err(err), code(code) {
    }

    const string &RemoteResult::get_stdout() const {
        return out;
    }

    const string &RemoteResult::get_stderr() const {
        return err;
    }

    int RemoteResult::get_code() const {
        return code;
    }

    static string ssh_read_channel_out(
            const ILogEventListener *listener,
            ssh_channel channel,
            int is_stderr,
            bool print_output
    ) {
        const size_t buffer_size = 256;
        size_t output_size = 4096;

        char *output = (char *) malloc(output_size);
        unsigned long current_pos = 0;
        unsigned long last_line_pos = 0;
        int ssh_n_read = 0;
        char buffer[buffer_size];
        char *line_buffer = (char *) malloc(256);
        do {
            ssh_n_read = ssh_channel_read_timeout(channel, buffer, buffer_size, is_stderr, 1800000);
            if (ssh_n_read > 0) {
                for (int i = 0; i < ssh_n_read; i++) {
                    auto c = buffer[i];
                    output[current_pos] = c;

                    if (print_output && c == '\n') {
                        const unsigned long size_of = sizeof(char) * (current_pos - last_line_pos + 1);
                        line_buffer = (char *) realloc(line_buffer, size_of + 1);
                        memcpy(line_buffer, output + last_line_pos, size_of);
                        line_buffer[size_of] = '\0';

                        if (is_stderr) {
                            listener->on_stderr_line(">> err", line_buffer);
                        } else {
                            listener->on_stdout_line(">> out", line_buffer);
                        }

                        last_line_pos = current_pos + 1;
                    }

                    current_pos++;

                    if (current_pos > output_size) {
                        output_size += buffer_size;
                        output = (char *) realloc(output, output_size);
                    }
                }
            }
        } while (ssh_n_read > 0);

        // Strip last line
        if (output[current_pos - 1] == '\n') {
            output[current_pos - 1] = '\0';
        } else {
            output[current_pos] = '\0';
        }

        auto result = string(output);

        delete[](line_buffer);
        delete[](output);

        return result;
    }

    SshApi::SshApi(const SshManager *manager, const ILogEventListener *log_listener)
            : manager(const_cast<SshManager *>(manager)), log_listener(log_listener) {}

    void SshApi::chdir(const string &chdir) {
        this->current_chdir = chdir;
    }

    RemoteResult SshApi::execute(const string &command, const bool print_output) const {
        const auto *session = manager->get_or_create_session(log_listener->get_level());
        auto *ssh_session = session->get_ssh_session();

        ssh_channel channel;
        int rc;

        channel = ssh_channel_new(ssh_session);
        if (nullptr == channel) {
            throw RuntimeException("SSH error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        rc = ssh_channel_open_session(channel);
        if (SSH_OK != rc) {
            ssh_channel_free(channel);
            throw RuntimeException("SSH error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        ostringstream cmd_buf;

        LoggingTimer timer;
        if (!this->current_chdir.empty()) {
            timer = log_listener->emit_info_wt(
                    "In directory <%s> executing <%s>", this->current_chdir.c_str(), command.c_str());
            cmd_buf << "cd " << this->current_chdir << " && ";
        } else {
            timer = log_listener->emit_info_wt("Executing command <%s>", command.c_str());
        }

        cmd_buf << command;

        log_listener->emit_debug("Full shell command is <%s>", cmd_buf.str().c_str());

        rc = ssh_channel_request_exec(channel, cmd_buf.str().c_str());

        timer.stop();

        if (SSH_OK != rc) {
            ssh_channel_close(channel);
            ssh_channel_free(channel);

            throw RuntimeException("SSH error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        auto out = ssh_read_channel_out(log_listener, channel, 0, print_output);
        auto err = ssh_read_channel_out(log_listener, channel, 1, print_output);

        if (ssh_channel_is_open(channel)) {
            ssh_channel_send_eof(channel);
            ssh_channel_close(channel);
        }

        auto e = ssh_channel_get_exit_status(channel);

        ssh_channel_free(channel);

        if (0 == e) {
            log_listener->emit_info(&timer, "Command complete");
        } else {
            log_listener->emit_warning(&timer, "Command complete with non-zero exit code <%d>", e);
        }

        return RemoteResult(out, err, e);
    }

    void SshApi::scp_upload_file(const string &file, const string &remote_file) const {
        if (!FileSystem::is_file_or_symlink(file)) {
            throw RuntimeException("File <%s> is not file", file.c_str());
        }

        auto file_path = std_fs::path(file);
        auto remote_file_path = std_fs::path(remote_file);

        std_fs::path remote_dir;
        if (!current_chdir.empty()) {
            remote_dir = FileSystem::absolute(remote_file, current_chdir);
        } else {
            remote_dir = remote_file_path;
        }

        const auto *session = manager->get_or_create_session(log_listener->get_level());
        auto *ssh_session = session->get_ssh_session();

        auto *scp = ssh_scp_new(ssh_session, SSH_SCP_WRITE, remote_dir.c_str());
        if (nullptr == scp) {
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        if (SSH_OK != ssh_scp_init(scp)) {
            ssh_scp_free(scp);
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        auto size = std_fs::file_size(file_path);

        auto rc = ssh_scp_push_file(scp, file_path.filename().c_str(), size, 0400 | 0200);

        if (SSH_OK != rc) {
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        bool failed = false;
        ifstream fin(file, ifstream::binary);
        char buffer[4096];
        do {
            fin.read(buffer, 4096);
            auto rcc = ssh_scp_write(scp, buffer, fin.gcount());

            if (SSH_OK != rcc) {
                failed = true;
                break;
            }
        } while (fin);
        fin.close();

        if (failed) {
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }

    void SshApi::scp_upload_file_from_string(const string &content, const string &remote_file) const {
        auto remote_file_path = std_fs::path(remote_file);

        std_fs::path remote_dir;
        if (!current_chdir.empty()) {
            remote_dir = FileSystem::absolute(remote_file, current_chdir);
        } else {
            remote_dir = remote_file_path;
        }

        const auto *session = manager->get_or_create_session(log_listener->get_level());
        auto *ssh_session = session->get_ssh_session();

        auto *scp = ssh_scp_new(ssh_session, SSH_SCP_WRITE, remote_dir.c_str());
        if (nullptr == scp) {
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        if (SSH_OK != ssh_scp_init(scp)) {
            ssh_scp_free(scp);
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        auto rc = ssh_scp_push_file(scp, remote_file.c_str(), content.size(), 0400 | 0200);

        if (SSH_OK != rc) {
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        auto rcc = ssh_scp_write(scp, content.c_str(), content.size());

        if (SSH_OK != rcc) {
            throw RuntimeException("SCP error [code %d: %s]", ssh_get_error_code(ssh_session),
                                   ssh_get_error(ssh_session));
        }

        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }

    void SshApi::scp_download_file(const string &file, const string &remote_file) const {
        const auto *session = manager->get_or_create_session(log_listener->get_level());
        auto *ssh_session = session->get_ssh_session();

        auto *scp = ssh_scp_new(ssh_session, SSH_SCP_READ, remote_file.c_str());
        if (scp == nullptr) {
            throw RuntimeException("SSH error: %s", ssh_get_error(ssh_session));
        }

        auto rc = ssh_scp_init(scp);
        if (rc != SSH_OK) {
            ssh_scp_free(scp);
            throw RuntimeException("SSH error: %s", ssh_get_error(ssh_session));
        }

        rc = ssh_scp_pull_request(scp);
        if (rc != SSH_SCP_REQUEST_NEWFILE) {
            ssh_scp_free(scp);
            throw RuntimeException("SSH error: %s", ssh_get_error(ssh_session));
        }

        auto size = ssh_scp_request_get_size(scp);

        ssh_scp_accept_request(scp);

        ofstream out(file, ofstream::binary);
        const size_t bsize = size > 4096 ? 4096 : (int) size;
        char *buffer = (char *) malloc(bsize);
        int read_count;
        do {
            read_count = ssh_scp_read(scp, buffer, bsize);
            if (read_count > 0) {
                out.write(buffer, read_count);
            }
        } while (read_count > 0);

        out.close();
        free(buffer);

        ssh_scp_close(scp);
        ssh_scp_free(scp);
    }

    string SshApi::scp_download_file_as_string(const string &remote_file) const {
        const auto *session = manager->get_or_create_session(log_listener->get_level());
        auto *ssh_session = session->get_ssh_session();

        auto *scp = ssh_scp_new(ssh_session, SSH_SCP_READ, remote_file.c_str());
        if (scp == nullptr) {
            throw RuntimeException("SSH error: %s", ssh_get_error(ssh_session));
        }

        auto rc = ssh_scp_init(scp);
        if (rc != SSH_OK) {
            ssh_scp_free(scp);
            throw RuntimeException("SSH error: %s", ssh_get_error(ssh_session));
        }

        rc = ssh_scp_pull_request(scp);
        if (rc != SSH_SCP_REQUEST_NEWFILE) {
            ssh_scp_free(scp);
            throw RuntimeException("SSH error: %s", ssh_get_error(ssh_session));
        }

        const size_t size = ssh_scp_request_get_size(scp);

        ssh_scp_accept_request(scp);

        stringstream bfs;
        const size_t bsize = size > 4096 ? 4096 : (size_t) size;
        char *buffer = (char *) malloc(bsize);
        int read_count = 0;
        do {
            read_count = ssh_scp_read(scp, buffer, bsize);
            if (read_count > 0) {
                bfs.write(buffer, read_count);
            }
        } while (read_count > 0);

        free(buffer);

        ssh_scp_close(scp);
        ssh_scp_free(scp);

        return bfs.str();
    }
}
