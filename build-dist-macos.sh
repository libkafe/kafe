#!/usr/bin/env bash

# Build OSX from current directory.

echo "Building libkafe for macOS"

mkdir -p build/osx/
cd build/osx

if [[ -z "${KAFE_CC}" ]]; then
  if [ -f "/opt/homebrew/opt/llvm/bin/clang" ]; then
    export CC=/opt/homebrew/opt/llvm/bin/clang
  else
    export CC=/usr/local/opt/llvm/bin/clang
  fi
  echo "Will look for C compiler at ${CC}"
else
  echo "Using preset C compiler from environment - KAFE_CC - ${KAFE_CC}"
  export CC=${KAFE_CC}
fi

if [[ -z "${KAFE_CXX}" ]]; then
    if [ -f "/opt/homebrew/opt/llvm/bin/clang++" ]; then
      export CXX=/opt/homebrew/opt/llvm/bin/clang++
    else
      export CXX=/usr/local/opt/llvm/bin/clang++
    fi
    echo "Will look for C++ compiler at ${CXX}"
else
    echo "Using preset C++ compiler from environment - KAFE_CXX - ${KAFE_CXX}"
    export CXX=${KAFE_CXX}
fi

if [ ! -f "${CC}" ]; then
  echo "Fail: C compiler not found. Declare explicitly using KAFE_CC environment variable"
  exit 1
fi

if [ ! -f "${CXX}" ]; then
  echo "Fail: C++ compiler not found. Declare explicitly using KAFE_CXX environment variable"
  exit 1
fi

set -xe
cmake ../..
make