### Latest stable release downloads

Here you can find instructions on how to setup latest binary packages for all supported operating systems.

Binary package repositories for Kafe are generously hosted by [Cloudsmith](https://cloudsmith.io).

- [Debian 10](#debian-10)
- [Debian 11](#debian-11)
- [Debian 12](#debian-12)
- [Ubuntu 18.04](#ubuntu-1804)
- [Ubuntu 20.04](#ubuntu-2004)
- [Ubuntu 22.04](#ubuntu-2204)
- [EL7](#el7)
- [EL8](#el8)
- [EL9](#el9)
- [Fedora 36](#fedora-36)
- [Fedora 37](#fedora-37)
- [Fedora 38](#fedora-38)
- [macOS](https://github.com/libkafe/homebrew-kafe)

See [releases](https://github.com/libkafe/kafe/releases) for all available downloads and historic versions,
including development headers.

See [Homebrew tap](https://github.com/libkafe/homebrew-kafe) for how to install Kafe on macOS.

*NOTE:* You should be able to use these binary packages for any derivative distributions too. For example,
Elementary OS 5.1 users can use Ubuntu 18.04 packages, since Elementary OS 5.1 is based on Ubuntu 18.04.

**IMPORTANT:** snippets bellow assume for commands listed to be executed as root.

#### Debian 10

```shell script
# Install APT dependencies
apt update && apt-get install -y apt-transport-https
# Setup Kafe repository signing key
apt-key adv --keyserver keyserver.ubuntu.com --recv AEE35447B5D3E4A69860622702F762CA6730E6A7
# Setup repository
echo 'deb https://dl.cloudsmith.io/public/kafe/libkafe/deb/debian buster main' | \
  tee /etc/apt/sources.list.d/kafe.list
# Update APT package lists and install kafe-cli
apt update && apt -y install kafe-cli
# Print about
kafe about
```

#### Debian 11

```shell script
# Install APT dependencies
apt update && apt-get install -y apt-transport-https
# Setup Kafe repository signing key
apt-key adv --keyserver keyserver.ubuntu.com --recv AEE35447B5D3E4A69860622702F762CA6730E6A7
# Setup repository
echo 'deb https://dl.cloudsmith.io/public/kafe/libkafe/deb/debian bullseye main' | \
  tee /etc/apt/sources.list.d/kafe.list
# Update APT package lists and install kafe-cli
apt update && apt -y install kafe-cli
# Print about
kafe about
```

#### Debian 12

```shell script
# Install APT dependencies
apt update && apt-get install -y apt-transport-https
# Setup Kafe repository signing key
apt-key adv --keyserver keyserver.ubuntu.com --recv AEE35447B5D3E4A69860622702F762CA6730E6A7
# Setup repository
echo 'deb https://dl.cloudsmith.io/public/kafe/libkafe/deb/debian bookworm main' | \
  tee /etc/apt/sources.list.d/kafe.list
# Update APT package lists and install kafe-cli
apt update && apt -y install kafe-cli
# Print about
kafe about
```

#### Ubuntu 18.04

```shell script
# Setup Kafe repository signing key
apt-key adv --keyserver keyserver.ubuntu.com --recv AEE35447B5D3E4A69860622702F762CA6730E6A7
# Setup repository
echo 'deb https://dl.cloudsmith.io/public/kafe/libkafe/deb/ubuntu bionic main' | \
  tee /etc/apt/sources.list.d/kafe.list
# Update APT package lists and install kafe-cli
apt update && apt -y install kafe-cli
# Print about
kafe about
```

#### Ubuntu 20.04

```shell script
# Setup Kafe repository signing key
apt-key adv --keyserver keyserver.ubuntu.com --recv AEE35447B5D3E4A69860622702F762CA6730E6A7
# Setup repository
echo 'deb https://dl.cloudsmith.io/public/kafe/libkafe/deb/ubuntu focal main' | \
  tee /etc/apt/sources.list.d/kafe.list
# Update APT package lists and install kafe-cli
apt update && apt -y install kafe-cli
# Print about
kafe about
```


#### Ubuntu 22.04

```shell script
# Setup Kafe repository signing key
apt-key adv --keyserver keyserver.ubuntu.com --recv AEE35447B5D3E4A69860622702F762CA6730E6A7
# Setup repository
echo 'deb https://dl.cloudsmith.io/public/kafe/libkafe/deb/ubuntu jammy main' | \
  tee /etc/apt/sources.list.d/kafe.list
# Update APT package lists and install kafe-cli
apt update && apt -y install kafe-cli
# Print about
kafe about
```

#### EL7

**IMPORTANT:** Kafe requires Lua version 5.3, a dependency not available in EL7 by default. You can either
build the package yourself or use [Cheese](http://www.nosuchhost.net/~cheese/fedora/packages/epel-7/x86_64/cheese-release.html)
repository to obtain compatible Lua package.

```shell script
# Setup cheese repository (Lua 5.3)
rpm -i http://www.nosuchhost.net/~cheese/fedora/packages/epel-7/x86_64/cheese-release-7-1.noarch.rpm
# Setup repository
curl -1sLf 'https://dl.cloudsmith.io/public/kafe/libkafe/cfg/setup/bash.rpm.sh' | distro=el codename=7 bash
# Install kafe-cli
yum -y install kafe-cli
# Print about
kafe about
```

#### EL8

```shell script
# Setup repository
curl -1sLf 'https://dl.cloudsmith.io/public/kafe/libkafe/cfg/setup/bash.rpm.sh' | distro=el codename=8 bash
# Install kafe-cli
dnf install -y kafe-cli
# Print about
kafe about
```

**NOTE:** this build WILL work on all RHEL 8 forks and derivatives. It SHOULD work on CentOS Stream 8.

#### EL9

```shell script
# Setup repository
curl -1sLf 'https://dl.cloudsmith.io/public/kafe/libkafe/cfg/setup/bash.rpm.sh' | distro=el codename=9 bash
# Install kafe-cli
dnf install -y kafe-cli
# Print about
kafe about
```

**NOTE:** this build WILL work on all RHEL 9 forks and derivatives. It SHOULD work on CentOS Stream 9.

#### Fedora 36

```shell script
# Setup repository
curl -1sLf 'https://dl.cloudsmith.io/public/kafe/libkafe/cfg/setup/bash.rpm.sh' | distro=fedora codename=36 bash
# Install kafe-cli
dnf install -y kafe-cli
# Print about
kafe about
```

#### Fedora 37

```shell script
# Setup repository
curl -1sLf 'https://dl.cloudsmith.io/public/kafe/libkafe/cfg/setup/bash.rpm.sh' | distro=fedora codename=37 bash
# Install kafe-cli
dnf install -y kafe-cli
# Print about
kafe about
```

#### Fedora 38

```shell script
# Setup repository
curl -1sLf 'https://dl.cloudsmith.io/public/kafe/libkafe/cfg/setup/bash.rpm.sh' | distro=fedora codename=38 bash
# Install kafe-cli
dnf install -y kafe-cli
# Print about
kafe about
```
