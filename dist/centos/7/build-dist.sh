#!/bin/bash
# Enable software collections LLVM toolset (7.0)
source scl_source enable llvm-toolset-7.0

set -e

# Clean existing build workspace
rm -rf /kafe/build/centos/7 2> /dev/null

# Create build workspace
mkdir -p /kafe/build/centos/7/

# Warp to build workspace
cd /kafe/build/centos/7/

# Prepare build
cmake -DCMAKE_BUILD_TYPE=Release -DCPACK_GENERATOR=RPM ../../../

# Compile project
make

# Package project
cpack

# Test install
rpm -i ./kafe-cli*.rpm ./libkafe*.rpm
env kafe about

# Fix permissions
cd /

WHO=/kafe/

stat $WHO > /dev/null || (echo You must mount a file to "$WHO" in order to properly assume user && exit 1)

USERID=$(stat -c %u $WHO)
GROUPID=$(stat -c %g $WHO)

if [ "$USERID" -eq "0" ]; then
   USERID=1000
fi

if [ "$GROUPID" -eq "0" ]; then
   GROUPID=1000
fi

chown -Rf $USERID:$GROUPID /kafe/build