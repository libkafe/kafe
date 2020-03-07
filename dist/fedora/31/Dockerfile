FROM fedora:31

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

ENV CC /usr/bin/clang-9
ENV CXX /usr/bin/clang++-9

CMD /kafe/dist/fedora/31/build-dist.sh