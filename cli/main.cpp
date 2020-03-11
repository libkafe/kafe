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

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <kafe/version.hpp>
#include <kafe/project.hpp>
#include <kafe/context.hpp>

#include "main.hpp"
#include "logger.hpp"

using namespace std;
using namespace kafe;

#if __APPLE__
extern char** environ;
#endif

void print_usage() {
    fflush(stderr);
    fflush(stdout);

    fprintf(stdout, "kafe, version %s (with libkafe %s, API level %d)\n\n", KAFE_CLI_VERSION, LIBKAFE_VERSION,
            LIBKAFE_API_LEVEL);

    cout << "Usage: kafe <command> [arguments, ...]" << endl;
    cout << "       kafe do <environment> <task,task,task,...>" << endl;
    cout << "       kafe <help|--help>" << endl;
    cout << "       kafe <version|--version> [--lib]" << endl;
    cout << "       kafe <about|--about>" << endl;

    cout << "\n";

    cout << " kafe do: execute tasks from project file with given environment." << endl;
    cout << " kafe help: display this help." << endl;
    cout << " kafe version: display KAFE program version and exit. Optionally,"
            " show libkafe version used if argument --lib is set." << endl;
    cout << " kafe about: display software credits and licensing information." << endl;

    fflush(stdout);
}

void print_about() {
    fflush(stderr);
    fflush(stdout);

    fprintf(stdout, "kafe, version %s, built %s\n\n", KAFE_CLI_VERSION, KAFE_CLI_BUILD_TS);
    cout << "Copyright 2020 Matiss Treinis" << endl;
    cout << "Licensed under the Apache License, Version 2.0" << endl;
    cout << "Homepage   : https://github.com/libkafe/kafe" << endl;
    cout << "Bug reports: https://github.com/libkafe/kafe/issues" << endl;

    cout << "\n";
    cout << "API level: " << LIBKAFE_API_LEVEL << endl;
    cout << "\n";

    cout << "Using libkafe " << LIBKAFE_VERSION << " (built " << LIBKAFE_BUILD_TS << ")" << endl;
    cout << "Using " << LIBKAFE_VERSION_LIB_LUA << endl;
    cout << "Using libssh " << LIBKAFE_VERSION_LIB_SSH << endl;
    cout << "Using libarchive " << LIBKAFE_VERSION_LIB_ARCHIVE << endl;
    cout << "Using CURL " << LIBKAFE_VERSION_LIB_CURL << endl;
    cout << "Using libgit2 " << LIBKAFE_VERSION_LIB_GIT2 << endl;

    fflush(stdout);
}

void print_version() {
    fflush(stderr);
    fflush(stdout);
    cout << KAFE_CLI_VERSION << endl;
    fflush(stdout);
}

void print_lib_version() {
    fflush(stderr);
    fflush(stdout);
    cout << LIBKAFE_VERSION << endl;
    fflush(stdout);
}

vector<string> split_csv_arguments(const string &arguments, char delimiter) {
    vector<string> result;
    stringstream stream(arguments);
    string item;

    while (getline(stream, item, delimiter)) {
        result.push_back(item);
    }

    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Missing command to execute";
        print_usage();
        return 1;
    }

    if (0 == strcmp("do", argv[1])) {
        if (4 > argc) {
            cerr << "Command expects exactly at least two arguments - environment name and "
                    "a comma separated task list with any number of arbitrary arguments "
                    "to forward to the tasks being invoked.\n"
                    "Example: kafe do staging task1,task2,task3 <arg, arg, arg>";
            print_usage();
            return 1;
        }

        map<const string, const string> envvals;
        for (char **current = environ; *current; ++current) {
            const string envl = string(*current);
            const int pos = envl.find_first_of('=');
            pair<string, string> p = pair<string, string>(
                    envl.substr(0, pos),
                    envl.substr(pos + 1)
            );
            envvals.insert(p);
        }

        string environment = argv[2];
        string task_list_s = argv[3];
        vector<string> task_list_v = split_csv_arguments(task_list_s, ',');

        vector<string> extra_args;
        for (int ii = 4; ii < argc; ++ii) {
            extra_args.emplace_back(argv[ii]);
        }

        try {
            auto project = Project("kafe.lua");
            auto logger = Logger();
            auto context = Context(envvals, environment, task_list_v, &logger);
            auto inventory = Inventory();
            project.execute(context, inventory, extra_args);
        } catch (RuntimeException &e) {
            cerr << e.what() << endl;
            return 1;
        }
        return 0;
    }

    if (0 == strcmp("about", argv[1]) || 0 == strcmp("--about", argv[1])) {
        print_about();
        return 0;
    }

    if (0 == strcmp("help", argv[1]) || 0 == strcmp("--help", argv[1])) {
        print_usage();
        return 0;
    }

    if (0 == strcmp("version", argv[1]) || 0 == strcmp("--version", argv[1])) {
        if (2 == argc) {
            print_version();
            return 0;
        }

        if (3 == argc && 0 == strcmp("--lib", argv[2])) {
            print_lib_version();
            return 0;
        }

        cerr << "Invalid usage of version command";
        return 1;
    }

    // Default
    cerr << "Unknown command";
    print_usage();
    return 1;
}
