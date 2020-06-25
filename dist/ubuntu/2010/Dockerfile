FROM ubuntu:20.10

RUN DEBIAN_FRONTEND=noninteractive apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        wget \
        cmake \
        build-essential \
        clang-10 \
        liblua5.3-dev \
        libcurl4-gnutls-dev \
        libarchive-dev \
        libssh-dev \
        libgit2-dev

ENV CC /usr/bin/clang-10
ENV CXX /usr/bin/clang++-10

CMD /kafe/dist/ubuntu/2010/build-dist.sh