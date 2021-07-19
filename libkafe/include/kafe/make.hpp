/**
 * This file is part of Kafe.
 * https://github.com/libkafe/kafe/
 *
 * Copyright 2021 Matiss Treinis
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

#ifndef LIBKAFE_MAKE_HPP
#define LIBKAFE_MAKE_HPP

#include <string>
#include <vector>
#include <map>
#include "logging.hpp"
#include "kafe/runtime/runtime_exception.hpp"

using namespace std;
using namespace kafe;
using namespace kafe::runtime;

namespace kafe {
  class MakeFileException : public RuntimeException {
    using RuntimeException::RuntimeException;
  };

  class Maker {
    void call_internal(const string &command, ILogEventListener *logger);

  public:
    int make(
        const string& file,
        const map<const string, const string> &env_vals,
        const vector <string>& stages,
        ILogEventListener *logger,
        const vector <string> &extra_args
    );
  };
}
#endif //LIBKAFE_MAKE_HPP
