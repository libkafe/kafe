#!/usr/bin/env bash
set -xe

# TODO - I do not like this copy paste...

rm -rf build/
rm -rf build-artifact/

# CentOS 7
if [[ "$(docker images -q "kafe/centos:7-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/centos:7-build dist/centos/7
fi

docker run -it --rm -v `pwd`:/kafe kafe/centos:7-build
mkdir -p build-artifact/centos-7/
cp build/centos/7/kafe-cli-*.rpm build-artifact/centos-7/
cp build/centos/7/libkafe-*.rpm build-artifact/centos-7/
for f in build-artifact/centos-7/*; do mv -v "$f" $(echo "$f" | sed "s/\.rpm/\.el7\.rpm/"); done

# CentOS 8
if [[ "$(docker images -q "kafe/centos:8-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/centos:8-build dist/centos/8
fi

docker run -it --rm -v `pwd`:/kafe kafe/centos:8-build
mkdir -p build-artifact/centos-8/
cp build/centos/8/kafe-cli-*.rpm build-artifact/centos-8/
cp build/centos/8/libkafe-*.rpm build-artifact/centos-8/
for f in build-artifact/centos-8/*; do mv -v "$f" $(echo "$f" | sed "s/\.rpm/\.el8\.rpm/"); done

# Debian 9
if [[ "$(docker images -q "kafe/debian:9-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/debian:9-build dist/debian/9
fi

docker run -it --rm -v `pwd`:/kafe kafe/debian:9-build
mkdir -p build-artifact/debian-9/
cp build/debian/9/kafe-cli*.deb build-artifact/debian-9/
cp build/debian/9/libkafe*.deb build-artifact/debian-9/
for f in build-artifact/debian-9/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.deb9\.deb/"); done

# Debian 10
if [[ "$(docker images -q "kafe/debian:10-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/debian:10-build dist/debian/10
fi

docker run -it --rm -v `pwd`:/kafe kafe/debian:10-build
mkdir -p build-artifact/debian-10/
cp build/debian/10/kafe-cli*.deb build-artifact/debian-10/
cp build/debian/10/libkafe*.deb build-artifact/debian-10/
for f in build-artifact/debian-10/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.deb10\.deb/"); done

# Debian 11
if [[ "$(docker images -q "kafe/debian:11-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/debian:11-build dist/debian/11
fi

docker run -it --rm -v `pwd`:/kafe kafe/debian:11-build
mkdir -p build-artifact/debian-11/
cp build/debian/11/kafe-cli*.deb build-artifact/debian-11/
cp build/debian/11/libkafe*.deb build-artifact/debian-11/
for f in build-artifact/debian-11/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.deb11\.deb/"); done

# Ubuntu 18.04
if [[ "$(docker images -q "kafe/ubuntu:1804-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/ubuntu:1804-build dist/ubuntu/1804
fi

docker run -it --rm -v `pwd`:/kafe kafe/ubuntu:1804-build
mkdir -p build-artifact/ubuntu-1804/
cp build/ubuntu/1804/kafe-cli*.deb build-artifact/ubuntu-1804/
cp build/ubuntu/1804/libkafe*.deb build-artifact/ubuntu-1804/
for f in build-artifact/ubuntu-1804/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.ubu1804\.deb/"); done

# Ubuntu 19.10
if [[ "$(docker images -q "kafe/ubuntu:1910-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/ubuntu:1910-build dist/ubuntu/1910
fi

docker run -it --rm -v `pwd`:/kafe kafe/ubuntu:1910-build
mkdir -p build-artifact/ubuntu-1910/
cp build/ubuntu/1910/kafe-cli*.deb build-artifact/ubuntu-1910/
cp build/ubuntu/1910/libkafe*.deb build-artifact/ubuntu-1910/
for f in build-artifact/ubuntu-1910/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.ubu1910\.deb/"); done

# Ubuntu 20.04
if [[ "$(docker images -q "kafe/ubuntu:2004-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/ubuntu:2004-build dist/ubuntu/2004
fi

docker run -it --rm -v `pwd`:/kafe kafe/ubuntu:2004-build
mkdir -p build-artifact/ubuntu-2004/
cp build/ubuntu/2004/kafe-cli*.deb build-artifact/ubuntu-2004/
cp build/ubuntu/2004/libkafe*.deb build-artifact/ubuntu-2004/
for f in build-artifact/ubuntu-2004/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.ubu2004\.deb/"); done

# Ubuntu 20.10
if [[ "$(docker images -q "kafe/ubuntu:2010-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/ubuntu:2010-build dist/ubuntu/2010
fi

docker run -it --rm -v `pwd`:/kafe kafe/ubuntu:2010-build
mkdir -p build-artifact/ubuntu-2010/
cp build/ubuntu/2010/kafe-cli*.deb build-artifact/ubuntu-2010/
cp build/ubuntu/2010/libkafe*.deb build-artifact/ubuntu-2010/
for f in build-artifact/ubuntu-2010/*; do mv -v "$f" $(echo "$f" | sed "s/\.deb/\.ubu2010\.deb/"); done

# Fedora 31
if [[ "$(docker images -q "kafe/fedora:31-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/fedora:31-build dist/fedora/31
fi

docker run -it --rm -v `pwd`:/kafe kafe/fedora:31-build
mkdir -p build-artifact/fedora-31/
cp build/fedora/31/kafe-cli-*.rpm build-artifact/fedora-31/
cp build/fedora/31/libkafe-*.rpm build-artifact/fedora-31/
for f in build-artifact/fedora-31/*; do mv -v "$f" $(echo "$f" | sed "s/\.rpm/\.f31\.rpm/"); done

# Fedora 32
if [[ "$(docker images -q "kafe/fedora:32-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/fedora:32-build dist/fedora/32
fi

docker run -it --rm -v `pwd`:/kafe kafe/fedora:32-build
mkdir -p build-artifact/fedora-32/
cp build/fedora/32/kafe-cli-*.rpm build-artifact/fedora-32/
cp build/fedora/32/libkafe-*.rpm build-artifact/fedora-32/
for f in build-artifact/fedora-32/*; do mv -v "$f" $(echo "$f" | sed "s/\.rpm/\.f32\.rpm/"); done

# Fedora 33
if [[ "$(docker images -q "kafe/fedora:33-build" 2> /dev/null)" == "" ]]; then
  docker build -t kafe/fedora:33-build dist/fedora/33
fi

docker run -it --rm -v `pwd`:/kafe kafe/fedora:33-build
mkdir -p build-artifact/fedora-33/
cp build/fedora/33/kafe-cli-*.rpm build-artifact/fedora-33/
cp build/fedora/33/libkafe-*.rpm build-artifact/fedora-33/
for f in build-artifact/fedora-33/*; do mv -v "$f" $(echo "$f" | sed "s/\.rpm/\.f33\.rpm/"); done
