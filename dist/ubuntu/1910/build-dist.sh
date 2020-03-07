#!/usr/bin/env bash
set -e

# Clean existing build workspace
rm -rf /kafe/build/ubuntu/1910 2> /dev/null

# Create build workspace
mkdir -p /kafe/build/ubuntu/1910/

# Warp to build workspace
cd /kafe/build/ubuntu/1910/

# Prepare build
cmake -DCMAKE_BUILD_TYPE=Release -DCPACK_GENERATOR=DEB ../../../

# Compile project
make

# Package project
cpack

# Test install
apt-get install -y ./kafe-cli*.deb ./libkafe*.deb
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