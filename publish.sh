#!/usr/bin/env bash

# EL
cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/kafe-cli-1.1.3-1.x86_64.el7.rpm
cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/libkafe-1.1.3-1.x86_64.el7.rpm
cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/libkafe-devel-1.1.3-1.x86_64.el7.rpm

cloudsmith push rpm kafe/libkafe/el/8 build-artifact/centos-8/kafe-cli-1.1.3-1.x86_64.el8.rpm
cloudsmith push rpm kafe/libkafe/el/8 build-artifact/centos-8/libkafe-1.1.3-1.x86_64.el8.rpm
cloudsmith push rpm kafe/libkafe/el/8 build-artifact/centos-8/libkafe-devel-1.1.3-1.x86_64.el8.rpm

# Debian
cloudsmith push deb kafe/libkafe/debian/stretch build-artifact/debian-9/kafe-cli_1.1.3_amd64.deb9.deb
cloudsmith push deb kafe/libkafe/debian/stretch build-artifact/debian-9/libkafe_1.1.3_amd64.deb9.deb
cloudsmith push deb kafe/libkafe/debian/stretch build-artifact/debian-9/libkafe-dev_1.1.3_amd64.deb9.deb

cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/kafe-cli_1.1.3_amd64.deb10.deb
cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/libkafe_1.1.3_amd64.deb10.deb
cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/libkafe-dev_1.1.3_amd64.deb10.deb

cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/kafe-cli_1.1.3_amd64.deb11.deb
cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/libkafe_1.1.3_amd64.deb11.deb
cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/libkafe-dev_1.1.3_amd64.deb11.deb

# Ubuntu
cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/kafe-cli_1.1.3_amd64.ubu1804.deb
cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/libkafe_1.1.3_amd64.ubu1804.deb
cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/libkafe-dev_1.1.3_amd64.ubu1804.deb

cloudsmith push deb kafe/libkafe/ubuntu/eoan build-artifact/ubuntu-1910/kafe-cli_1.1.3_amd64.ubu1910.deb
cloudsmith push deb kafe/libkafe/ubuntu/eoan build-artifact/ubuntu-1910/libkafe_1.1.3_amd64.ubu1910.deb
cloudsmith push deb kafe/libkafe/ubuntu/eoan build-artifact/ubuntu-1910/libkafe-dev_1.1.3_amd64.ubu1910.deb

cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/kafe-cli_1.1.3_amd64.ubu2004.deb
cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/libkafe_1.1.3_amd64.ubu2004.deb
cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/libkafe-dev_1.1.3_amd64.ubu2004.deb

cloudsmith push deb kafe/libkafe/ubuntu/groovy build-artifact/ubuntu-2010/kafe-cli_1.1.3_amd64.ubu2010.deb
cloudsmith push deb kafe/libkafe/ubuntu/groovy build-artifact/ubuntu-2010/libkafe_1.1.3_amd64.ubu2010.deb
cloudsmith push deb kafe/libkafe/ubuntu/groovy build-artifact/ubuntu-2010/libkafe-dev_1.1.3_amd64.ubu2010.deb

# Fedora
cloudsmith push rpm kafe/libkafe/fedora/31 build-artifact/fedora-31/kafe-cli-1.1.3-1.x86_64.f31.rpm
cloudsmith push rpm kafe/libkafe/fedora/31 build-artifact/fedora-31/libkafe-1.1.3-1.x86_64.f31.rpm
cloudsmith push rpm kafe/libkafe/fedora/31 build-artifact/fedora-31/libkafe-devel-1.1.3-1.x86_64.f31.rpm

cloudsmith push rpm kafe/libkafe/fedora/32 build-artifact/fedora-32/kafe-cli-1.1.3-1.x86_64.f32.rpm
cloudsmith push rpm kafe/libkafe/fedora/32 build-artifact/fedora-32/libkafe-1.1.3-1.x86_64.f32.rpm
cloudsmith push rpm kafe/libkafe/fedora/32 build-artifact/fedora-32/libkafe-devel-1.1.3-1.x86_64.f32.rpm

cloudsmith push rpm kafe/libkafe/fedora/33 build-artifact/fedora-33/kafe-cli-1.1.3-1.x86_64.f33.rpm
cloudsmith push rpm kafe/libkafe/fedora/33 build-artifact/fedora-33/libkafe-1.1.3-1.x86_64.f33.rpm
cloudsmith push rpm kafe/libkafe/fedora/33 build-artifact/fedora-33/libkafe-devel-1.1.3-1.x86_64.f33.rpm
