FROM debian:bullseye

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

CMD /kafe/dist/debian/11/build-dist.sh