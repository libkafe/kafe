FROM almalinux:8

RUN yum -y update && \
    yum -y install dnf-plugins-core && \
    yum config-manager --set-enabled powertools && \
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

CMD /kafe/dist/almalinux/8/build-dist.sh