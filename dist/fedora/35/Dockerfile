FROM fedora:35

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

ENV CC /usr/bin/clang-12
ENV CXX /usr/bin/clang++-12

CMD /kafe/dist/fedora/35/build-dist.sh
