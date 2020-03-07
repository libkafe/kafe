FROM debian:10

RUN DEBIAN_FRONTEND=noninteractive apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get upgrade -y && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y -y \
        wget \
        cmake \
        build-essential \
        clang-7 \
        liblua5.3-dev \
        libcurl4-gnutls-dev \
        libarchive-dev \
        libssh-dev \
        libgit2-dev

ENV CC /usr/bin/clang-7
ENV CXX /usr/bin/clang++-7

CMD /kafe/dist/debian/10/build-dist.sh