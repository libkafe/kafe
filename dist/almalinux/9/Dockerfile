FROM almalinux:9

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

ENV CC /usr/bin/clang-15
ENV CXX /usr/bin/clang++-15

CMD /kafe/dist/almalinux/9/build-dist.sh
