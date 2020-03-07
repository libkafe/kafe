FROM centos:7

RUN rpm -i http://www.nosuchhost.net/~cheese/fedora/packages/epel-7/x86_64/cheese-release-7-1.noarch.rpm

RUN yum -y update && \
    yum -y install centos-release-scl && \
    yum-config-manager --enable rhel-server-rhscl-7-rpms && \
    yum -y install \
        make \
        rpm-build \
        wget \
        llvm-toolset-7.0 \
        lua-devel \
        libcurl-devel \
        libarchive-devel \
        libssh-devel \
        libgit2-devel

ENV CC /opt/rh/llvm-toolset-7.0/root/usr/bin/clang
ENV CXX /opt/rh/llvm-toolset-7.0/root/usr/bin/clang++

# Build requires at least CMake 3.11.4, CentOS 8 has CMake 2.8
RUN mkdir -p /opt/cmake/ && \
    cd /opt/cmake && \
    wget https://github.com/Kitware/CMake/releases/download/v3.16.4/cmake-3.16.4-Linux-x86_64.tar.gz && \
    tar -xvf cmake-3.16.4-Linux-x86_64.tar.gz

ENV PATH="/opt/cmake/cmake-3.16.4-Linux-x86_64/bin:${PATH}"

CMD /kafe/dist/centos/7/build-dist.sh