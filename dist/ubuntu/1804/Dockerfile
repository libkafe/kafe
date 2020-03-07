FROM ubuntu:18.04

RUN DEBIAN_FRONTEND=noninteractive apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y -y \
        wget \
        cmake \
        build-essential \
        clang-9 \
        liblua5.3-dev \
        libcurl4-gnutls-dev \
        libarchive-dev \
        libssh-dev \
        libgit2-dev

ENV CC /usr/bin/clang-9
ENV CXX /usr/bin/clang++-9

# Build requires at least CMake 3.11.4, Ubuntu 18.04 has CMake 3.10
RUN mkdir -p /opt/cmake/ && \
    cd /opt/cmake && \
    wget https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-Linux-x86_64.tar.gz && \
    tar -xvf cmake-3.16.4-Linux-x86_64.tar.gz

ENV PATH="/opt/cmake/cmake-3.16.4-Linux-x86_64/bin:${PATH}"

CMD /kafe/dist/ubuntu/1804/build-dist.sh