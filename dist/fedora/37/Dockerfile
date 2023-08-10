FROM fedora:37

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

ENV CC /usr/bin/clang-15
ENV CXX /usr/bin/clang++-15

CMD /kafe/dist/fedora/37/build-dist.sh
