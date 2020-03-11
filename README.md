Kafe (`/ka'fe:/`) is an open source scriptable systems automation toolkit. It provides a basic set of features to interact 
with local and remote systems over SSH. Kafe is well suited for application deployment and similar 
remote systems administration tasks.

## Downloads

See [available binary packages](DOWNLOAD.md) for current stable release or [releases](https://github.com/libkafe/kafe/releases) for 
all available downloads and historic versions.

Binary builds are available for:

- **CentOS** and **RHEL** versions [7<sup>1</sup>](./DOWNLOAD.md#centos-and-rhel-7), [8](./DOWNLOAD.md#centos-and-rhel-8)
- **Fedora** versions [31](./DOWNLOAD.md#fedora-31), [32](./DOWNLOAD.md#fedora-32), [33](./DOWNLOAD.md#fedora-33)
- **Ubuntu** versions [18.04](./DOWNLOAD.md#ubuntu-1804), [19.10](./DOWNLOAD.md#ubuntu-1910), [20.04](./DOWNLOAD.md#ubuntu-2004)
- **Debian** versions [9](./DOWNLOAD.md#debian-9), [10](./DOWNLOAD.md#debian-10), [11](./DOWNLOAD.md#debian-11)

You should be able to use these binary packages for any derivative distributions.
For example, Elementary OS 5.1 users can use Ubuntu 18.04 packages, since Elementary OS is based on Ubuntu 18.04.

**macOS is officially supported**, but binary builds are not provided. See bellow how to compile 
Kafe from source - building Kafe on macOS is fairly trivial.

<sup>1</sup> - EL7 does not ship with Lua 5.3. We currently use 
[Cheese](http://www.nosuchhost.net/~cheese/fedora/packages/epel-7/x86_64/cheese-release.html) repository
to obtain Lua 5.3 on EL7.

### Installation

Installing a binary build is easy - download the binary packages from [releases](https://github.com/libkafe/kafe/releases)
and install using your operating systems package manager. You will need to download both `kafe-cli` and `libkafe`
artifacts. You do not need to install `libkafe-dev` unless you aim to embed libkafe.

You can also build the sources locally using `CMake`. See bellow for instructions to build from source.

## Writing Kafe scripts

You can declare your server inventory and automation tasks in Kafe scripts. Kafe scripts are written in 
[Lua](https://www.lua.org) programming language. Kafe CLI looks for a file named `kafe.lua`
in current working directory to execute the tasks from.

### Why Lua?

Lua is widely used, minimal and easy to learn programming language. It is easy to adopt and has
[extensive documentation](http://www.lua.org/docs.html) with large and stable standard library -
a perfect fit for scripting against Kafe APIs.

### Main concepts

#### Tasks

All automation code in Kafe is divided in work units called tasks. A task is isolated unit of work that describes
all actions needed to perform against local or remote systems.

Tasks are independent, and there are no facilities to chain or relate tasks - this is by design. To share code and
operations between tasks, you can use standard Lua functions.

#### Inventory

Kafe categorizes all remote servers by environment and role. And environment is a logical unit of remote servers -
testing environment, staging environment, production environment, and so forth. A role is a logical unit
of one or more servers performing the same infrastructure role - a database server, a web server, an application host, to
name a few. Each inventory item can have one or more roles and environment.

All remote servers in Kafe must be placed in inventory. Inventory is a list of remote servers with associated
roles and environments. Each server can have one or more roles and can be present in one or more environments.
Each inventory line uniquely combines remote server address, one role, and one environment. To add the same 
remote server to multiple roles and environments duplicate the inventory definition line. 

#### Execution of tasks

All tasks are executed in order as invoked by the script (top-down). This is by design. Parallelization of
remote management tasks might seem a good idea at first, but it rarely is - parallelization makes remote error
handling much more difficult, might leave remote systems in an inconsistent state and requires a
much larger investment in writing the automation tasks.

### Examples

You can find examples of how to write Kafe scripts in the [examples](./examples) directory.

### Scripting API

Scripting API documentation for API level 1 is available [here](docs/SCRIPTING_API_L1.md).

### Using the CLI

To execute a Kafe script, you need to invoke `kafe` command with sub-command `do`. You need to pass
the desired execution environment and a list of one or more tasks to be executed, separated by a comma.

For example, to execute example tasks named `deploy`, `verify` on `production` environment, you would
execute following Kafe CLI command:

`kafe do production deploy,verify`

When executed, Kafe CLI will look for a file named `kafe.lua` in the current working direcory. This
file will be interpreted and requested tasks from it will be executed against all relevant remote servers.

#### SSH and SSH authentication

Kafe is using SSH for remote command execution and file uploads. It will attempt to use SSH agent, any known local
SSH keys, as well as GSSAPI-WITH-MIC and password based authentication to authenticate to remote hosts.

If no SSH agent is present, you can set the passphrase to be used to decrypt any encrypted private keys using
environment variable `KAFE_SSH_PKEY_PASS`. You can set password to be used for password based authentication using 
environment variable named `KAFE_SSH_USER_PASS`.

**IMPORTANT:** Kafe will not automatically add remote keys to known hosts nor will it provide a way to do so interactively.
It is your responsibility to ensure remote host keys are added to known hosts before attempting to connect to remote hosts
using Kafe. Any attempts to connect to remote hosts with unknown or changed host keys will fail.

### Debugging

You can change the logging level of the CLI tool by setting `KAFE_LOG_LEVEL` environment variable. For example:

`KAFE_LOG_LEVEL=1 kafe do staging deploy`

Available log levels are:

ALL = 0, TRACE = 1, DEBUG = 2, INFO = 3, SUCCESS = 4, WARNING = 5, ERROR = 6, NONE = 7.

## Kafe and libkafe

Kafe is written entirely in C++ - it is distributed as a native binary (kafe-cli) and a shared library (libkafe) 
with C++ development headers made available for embedding (libkafe-dev).

## Operating system support

Kafe should work with any UNIX-like operating system, provided all external dependencies are met.
Binary packages are provided for:

- **CentOS** and **RHEL** versions 7, 8 
- **Fedora** versions 31, 32, 33
- **Ubuntu** versions 18.04, 19.10, 20.04
- **Debian** versions 9, 10, 11
- **macOS**  versions 10.15 Catalina

Windows is not supported and there are no plans to support Windows builds. Windows users
should be able to use Windows Subsystem for Linux to run or build Kafe. Take note that contributions
and issues related to support for Windows will not be accepted.

macOS is officially supported for versions 10.15 Catalina and newer. It is entirely possible Kafe works just
fine on older versions of macOS, but no testing has been done on them. If you are unable to upgrade
to latest macOS version, you should still try to install Kafe from sources. It will most likely work
just fine provided all external dependencies are met.

If you would like to contribute your distribution to the above list, you can implement a build for it. 
Just follow the samples already present in [dist](./dist) folder. Any contributions adding support for
new distributions must contain all current non-EOL distribution versions, including RC versions, if any.

#### Does Kafe have APT / YUM repositories?

Not at the moment. Having repositories for CentOS, RHEL, Fedora, Debian and Ubuntu is something that I plan
to do in the future.

#### Is Kafe available on Homebrew?

No, but it is planned.

#### Does Kafe have official Docker images with binaries?

No and it is unlikely there ever will be. At this time, Kafe is primarily a CLI tool - having a Docker image for it
has as much benefit as having a Docker image for `tail`. If you need to have Kafe in Docker for CI or some other use,
creating your own image using any of the supported base operating system images should be trivial.

## Backwards compatibility

### Kafe CLI and libkafe

Kafe CLI should be compatible with libkafe within the same major version.

### libkafe

Libkafe development libraries are guaranteed to be compatible within the same major version.

### Scripting API

Scripting API is versioned separately from CLI and libkafe. Scripting API compatibility is guaranteed
within the same API version, meaning, any script built for API version 1 will work with any Kafe version supporting
version 1 regardless of the internals of Kafe itself.

| libkafe version | API level    | Lua      | Status     |
|-----------------|--------------|----------|------------|
| 1.x             | 1            | 5.3, 5.4 | Locked     |
| 2.x             | 2<sup>1</sup>| 5.3, 5.4 | Planned    |

<sup>1</sup> - it is very likely API level 2 will be fully backward compatible with level 1.

### Lua compatibility

Kafe currently supports Lua version 5.3 with 5.4 support in development. Lua versions
older than 5.3 are not supported. Kafe will build against the latest available version of Lua in the
build system. For most part, it is the latest version distributed by the operating system itself with the sole exception
of CentOS 7, where third party repository is required to install and use Kafe.

### Building from source

To build Kafe from the source, you will need C++ and C compiler, CMake (version 3.11.4 or newer), Make and following 
development libraries:

- liblua version 5.3 or newer (up to version 5.4) 
- libcurl (reserved for future APIs)
- libarchive 
- libssh 
- libgit2 (reserved for future APIs)

Kafe is built and tested using Clang toolchain - version 7 or higher, depending on build environment,
both in development and distribution. You should be able to build your own binaries using any
C11 and C++17 compatible C/C++ compiler toolchain.

See [/dist/](./dist) directory for example Docker build files and scripts per each supported build target.

You can also refer to [build-dist.sh](./build-dist.sh) for how to build distribution specific packages
using Docker.

#### Building on macOS

To build from sources on macOS you will need `git` and Kafe dependencies installed on your system.

To install dependencies using Homebrew, use this command:

`brew install llvm lua libssh libgit2 curl libarchive`

To build 
1. Clone the source from `git@github.com:libkafe/kafe.git`;
2. Optionally, check out the desired version tag to build from GIT;
3. Execute [build-dist-macos.sh](./build-dist-macos.sh) file found at the root of the cloned sources.
4. Optionally, enter `build/osx` directory, and issue `make install` command to install Kafe on your local machine.

## Future expansions

Scripting API level 1 ships with a basic set of commands required for remote deployment and maintenance. It is considered
feature complete and no new features will be added to this API level.

Scripting API level 2 will incorporate more features to simplify application deployment and systems operations. Following
is planned for level 2 expansion:

- `.kafeignore` support for archive and directory operations.
- Local and remote file system utility functions, expanded standard operations library.
- Subset of Git features to streamline working with repositories (cloning, pulling, pushing, tagging, reading tags, releases...).
- Simplistic CURL based HTTP client (basic requests, file uploads and downloads).
- Test framework for Kafe scripts.

A side-goal of API level 2 is to retain compatibility with API level 1, meaning, if at all possible, all scripts written
for API level 1 will work just fine with API level 2 compatible libkafe.

## History

Kafe is nearly complete rewrite of OPM (Optional Package Manager) - a command-line tool I wrote several years ago for my
personal use to automate application and content deployment. It was written entirely in C using Python as a scripting
language to interface against remote APIs. OPMs design had several drawbacks, namely very tight integration with specific
Python version and gratuitous use of static linking against certain 3rd party dependencies. It worked (and still does!)
just fine, but was not suitable at all to be open sourced nor was it easily sharable - meaning it was fairly useless
outside the realm of my own personal use.

[Hemp](https://github.com/Addvilz/hemp) was another tool I wrote prior to Kafe for use in production to automate deployment
and remote automation. This tool was based on now unmaintained 3rd party remote automation library and was deprecated
due to lack of support for Python 3 and breaking changes from upstream library vendor. Unfortunately, Hemp was and still
is being used in a number of production projects and many production systems rely on it being functional and maintained, 
something that is no longer viable.

I created Kafe to replace both of these tools. Kafe is designed to be as minimal and straightforward as possible. 
I wanted to create an automation tool with strong backwards compatibility guarantees, meaning that once written,
automation tasks should work with minimal or no changes years to come.

## License

Licensed under the Apache License, Version 2.0 (the "License"). See [LICENSE](./LICENSE) for more details.
