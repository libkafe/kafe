FROM fedora:38

RUN yum -y update && \
    yum -y install \
        make \
        rpm-build \
        wget \
        clang \
        cmake \
        lua-devel \
        libcurl-devel \
        libarchive-devel \
        libssh-devel \
        libgit2-devel

ENV CC /usr/bin/clang-16
ENV CXX /usr/bin/clang++-16

CMD /kafe/dist/fedora/38/build-dist.sh
