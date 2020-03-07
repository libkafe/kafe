FROM fedora:33

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

ENV CC /usr/bin/clang-10
ENV CXX /usr/bin/clang++-10

CMD /kafe/dist/fedora/33/build-dist.sh