#!/usr/bin/env bash
set -xe

# Build OSX from current directory.

mkdir -p build/osx/
cd build/osx

export CC=/usr/local/opt/llvm/bin/clang
export CXX=/usr/local/opt/llvm/bin/clang++

cmake ../..
make