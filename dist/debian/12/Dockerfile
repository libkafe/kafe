FROM debian:bookworm

RUN DEBIAN_FRONTEND=noninteractive apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
        wget \
        cmake \
        build-essential \
        clang-11 \
        liblua5.4-dev \
        libcurl4-gnutls-dev \
        libarchive-dev \
        libssh-dev \
        libgit2-dev

ENV CC /usr/bin/clang-11
ENV CXX /usr/bin/clang++-11

CMD /kafe/dist/debian/12/build-dist.sh