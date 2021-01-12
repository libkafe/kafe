FROM fedora:34

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

ENV CC /usr/bin/clang-11
ENV CXX /usr/bin/clang++-11

CMD /kafe/dist/fedora/34/build-dist.sh
