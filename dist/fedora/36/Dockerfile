FROM fedora:36

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

ENV CC /usr/bin/clang-13
ENV CXX /usr/bin/clang++-13

CMD /kafe/dist/fedora/36/build-dist.sh
