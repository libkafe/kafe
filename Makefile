clean:
	rm -rf build/
	rm -rf build-artifact/

clean-docker-build:
	docker rmi kafe/centos:7-build
	docker rmi kafe/almalinux:8-build
	docker rmi kafe/almalinux:9-build
	docker rmi kafe/debian:10-build
	docker rmi kafe/debian:11-build
	docker rmi kafe/debian:12-build
	docker rmi kafe/ubuntu:1804-build
	docker rmi kafe/ubuntu:2004-build
	docker rmi kafe/ubuntu:2204-build
	docker rmi kafe/fedora:36-build
	docker rmi kafe/fedora:37-build
	docker rmi kafe/fedora:38-build

build-el-7:
	-rm -rf build-artifact/centos-7/
	if [[ "$(docker images -q "kafe/centos:7-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/centos:7-build dist/centos/7; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/centos:7-build
	mkdir -p build-artifact/centos-7/
	cp build/centos/7/kafe-cli-*.rpm build-artifact/centos-7/
	cp build/centos/7/libkafe-*.rpm build-artifact/centos-7/
	for f in build-artifact/centos-7/*; do mv -v "$$f" `sed "s/\.rpm/\.el7\.rpm/" <<< "$$f"`; done

build-el-8:
	-rm -rf build-artifact/almalinux-8/
	if [[ "$(docker images -q "kafe/almalinux:8-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/almalinux:8-build dist/almalinux/8; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/almalinux:8-build
	mkdir -p build-artifact/almalinux-8/
	cp build/almalinux/8/kafe-cli-*.rpm build-artifact/almalinux-8/
	cp build/almalinux/8/libkafe-*.rpm build-artifact/almalinux-8/
	for f in build-artifact/almalinux-8/*; do mv -v "$$f" `sed "s/\.rpm/\.el8\.rpm/" <<< "$$f"`; done

build-debian-10:
	-rm -rf build-artifact/debian-10/
	if [[ "$(docker images -q "kafe/debian:10-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/debian:10-build dist/debian/10; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/debian:10-build
	mkdir -p build-artifact/debian-10/
	cp build/debian/10/kafe-cli*.deb build-artifact/debian-10/
	cp build/debian/10/libkafe*.deb build-artifact/debian-10/
	for f in build-artifact/debian-10/*; do mv -v "$$f" `sed "s/\.deb/\.deb10\.deb/" <<< "$$f"`; done

build-debian-11:
	-rm -rf build-artifact/debian-11/
		if [[ "$(docker images -q "kafe/debian:11-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/debian:11-build dist/debian/11; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/debian:11-build
	mkdir -p build-artifact/debian-11/
	cp build/debian/11/kafe-cli*.deb build-artifact/debian-11/
	cp build/debian/11/libkafe*.deb build-artifact/debian-11/
	for f in build-artifact/debian-11/*; do mv -v "$$f" `sed "s/\.deb/\.deb11\.deb/" <<< "$$f"`; done

build-debian-12:
	-rm -rf build-artifact/debian-12/
	if [[ "$(docker images -q "kafe/debian:12-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/debian:12-build dist/debian/12; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/debian:12-build
	mkdir -p build-artifact/debian-12/
	cp build/debian/12/kafe-cli*.deb build-artifact/debian-12/
	cp build/debian/12/libkafe*.deb build-artifact/debian-12/
	for f in build-artifact/debian-12/*; do mv -v "$$f" `sed "s/\.deb/\.deb12\.deb/" <<< "$$f"`; done

build-ubuntu-1804:
	-rm -rf build-artifact/ubuntu-1804/
		if [[ "$(docker images -q "kafe/ubuntu:1804-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/ubuntu:1804-build dist/ubuntu/1804; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/ubuntu:1804-build
	mkdir -p build-artifact/ubuntu-1804/
	cp build/ubuntu/1804/kafe-cli*.deb build-artifact/ubuntu-1804/
	cp build/ubuntu/1804/libkafe*.deb build-artifact/ubuntu-1804/
	for f in build-artifact/ubuntu-1804/*; do mv -v "$$f" `sed "s/\.deb/\.ubu1804\.deb/" <<< "$$f"`; done

build-ubuntu-2004:
	-rm -rf build-artifact/ubuntu-2004/
	if [[ "$(docker images -q "kafe/ubuntu:2004-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/ubuntu:2004-build dist/ubuntu/2004; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/ubuntu:2004-build
	mkdir -p build-artifact/ubuntu-2004/
	cp build/ubuntu/2004/kafe-cli*.deb build-artifact/ubuntu-2004/
	cp build/ubuntu/2004/libkafe*.deb build-artifact/ubuntu-2004/
	for f in build-artifact/ubuntu-2004/*; do mv -v "$$f" `sed "s/\.deb/\.ubu2004\.deb/" <<< "$$f"`; done

build-ubuntu-2204:
	-rm -rf build-artifact/ubuntu-2204/
	if [[ "$(docker images -q "kafe/ubuntu:2204-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/ubuntu:2204-build dist/ubuntu/2204; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/ubuntu:2204-build
	mkdir -p build-artifact/ubuntu-2204/
	cp build/ubuntu/2204/kafe-cli*.deb build-artifact/ubuntu-2204/
	cp build/ubuntu/2204/libkafe*.deb build-artifact/ubuntu-2204/
	for f in build-artifact/ubuntu-2204/*; do mv -v "$$f" `sed "s/\.deb/\.ubu2204\.deb/" <<< "$$f"`; done

build-fedora-36:
	-rm -rf build-artifact/fedora-36/
		if [[ "$(docker images -q "kafe/fedora:36-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:36-build dist/fedora/36; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:36-build
	mkdir -p build-artifact/fedora-36/
	cp build/fedora/36/kafe-cli-*.rpm build-artifact/fedora-36/
	cp build/fedora/36/libkafe-*.rpm build-artifact/fedora-36/
	for f in build-artifact/fedora-36/*; do mv -v "$$f" `sed "s/\.rpm/\.f36\.rpm/" <<< "$$f"`; done

build-fedora-37:
	-rm -rf build-artifact/fedora-37/
		if [[ "$(docker images -q "kafe/fedora:37-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:37-build dist/fedora/37; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:37-build
	mkdir -p build-artifact/fedora-37/
	cp build/fedora/37/kafe-cli-*.rpm build-artifact/fedora-37/
	cp build/fedora/37/libkafe-*.rpm build-artifact/fedora-37/
	for f in build-artifact/fedora-37/*; do mv -v "$$f" `sed "s/\.rpm/\.f37\.rpm/" <<< "$$f"`; done

build-fedora-38:
	-rm -rf build-artifact/fedora-38/
		if [[ "$(docker images -q "kafe/fedora:38-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:38-build dist/fedora/38; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:38-build
	mkdir -p build-artifact/fedora-38/
	cp build/fedora/38/kafe-cli-*.rpm build-artifact/fedora-38/
	cp build/fedora/38/libkafe-*.rpm build-artifact/fedora-38/
	for f in build-artifact/fedora-38/*; do mv -v "$$f" `sed "s/\.rpm/\.f38\.rpm/" <<< "$$f"`; done

build-all: clean \
	 build-el-7 \
	 build-el-8 \
	 build-debian-10 \
	 build-debian-11 \
	 build-debian-12 \
	 build-ubuntu-1804 \
	 build-ubuntu-2004 \
	 build-ubuntu-2204 \
	 build-fedora-36 \
	 build-fedora-37 \
	 build-fedora-38

publish-el-7:
	cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/kafe-cli-1.1.7-1.x86_64.el7.rpm
	cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/libkafe-1.1.7-1.x86_64.el7.rpm
	cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/libkafe-devel-1.1.7-1.x86_64.el7.rpm

publish-el-8:
	cloudsmith push rpm kafe/libkafe/el/8 build-artifact/almalinux-8/kafe-cli-1.1.7-1.x86_64.el8.rpm
	cloudsmith push rpm kafe/libkafe/el/8 build-artifact/almalinux-8/libkafe-1.1.7-1.x86_64.el8.rpm
	cloudsmith push rpm kafe/libkafe/el/8 build-artifact/almalinux-8/libkafe-devel-1.1.7-1.x86_64.el8.rpm

publish-el-9:
	cloudsmith push rpm kafe/libkafe/el/9 build-artifact/almalinux-9/kafe-cli-1.1.7-1.x86_64.el9.rpm
	cloudsmith push rpm kafe/libkafe/el/9 build-artifact/almalinux-9/libkafe-1.1.7-1.x86_64.el9.rpm
	cloudsmith push rpm kafe/libkafe/el/9 build-artifact/almalinux-9/libkafe-devel-1.1.7-1.x86_64.el9.rpm

publish-debian-10:
	cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/kafe-cli_1.1.7_amd64.deb10.deb
	cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/libkafe_1.1.7_amd64.deb10.deb
	cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/libkafe-dev_1.1.7_amd64.deb10.deb

publish-debian-11:
	cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/kafe-cli_1.1.7_amd64.deb11.deb
	cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/libkafe_1.1.7_amd64.deb11.deb
	cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/libkafe-dev_1.1.7_amd64.deb11.deb

publish-debian-12:
	cloudsmith push deb kafe/libkafe/debian/bookworm build-artifact/debian-12/kafe-cli_1.1.7_amd64.deb12.deb
	cloudsmith push deb kafe/libkafe/debian/bookworm build-artifact/debian-12/libkafe_1.1.7_amd64.deb12.deb
	cloudsmith push deb kafe/libkafe/debian/bookworm build-artifact/debian-12/libkafe-dev_1.1.7_amd64.deb12.deb

publish-ubuntu-1804:
	cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/kafe-cli_1.1.7_amd64.ubu1804.deb
	cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/libkafe_1.1.7_amd64.ubu1804.deb
	cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/libkafe-dev_1.1.7_amd64.ubu1804.deb

publish-ubuntu-2004:
	cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/kafe-cli_1.1.7_amd64.ubu2004.deb
	cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/libkafe_1.1.7_amd64.ubu2004.deb
	cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/libkafe-dev_1.1.7_amd64.ubu2004.deb

publish-ubuntu-2204:
	cloudsmith push deb kafe/libkafe/ubuntu/jammy build-artifact/ubuntu-2204/kafe-cli_1.1.7_amd64.ubu2204.deb
	cloudsmith push deb kafe/libkafe/ubuntu/jammy build-artifact/ubuntu-2204/libkafe_1.1.7_amd64.ubu2204.deb
	cloudsmith push deb kafe/libkafe/ubuntu/jammy build-artifact/ubuntu-2204/libkafe-dev_1.1.7_amd64.ubu2204.deb

publish-fedora-36:
	cloudsmith push rpm kafe/libkafe/fedora/36 build-artifact/fedora-36/kafe-cli-1.1.7-1.x86_64.f36.rpm
	cloudsmith push rpm kafe/libkafe/fedora/36 build-artifact/fedora-36/libkafe-1.1.7-1.x86_64.f36.rpm
	cloudsmith push rpm kafe/libkafe/fedora/36 build-artifact/fedora-36/libkafe-devel-1.1.7-1.x86_64.f36.rpm

publish-fedora-37:
	cloudsmith push rpm kafe/libkafe/fedora/37 build-artifact/fedora-37/kafe-cli-1.1.7-1.x86_64.f37.rpm
	cloudsmith push rpm kafe/libkafe/fedora/37 build-artifact/fedora-37/libkafe-1.1.7-1.x86_64.f37.rpm
	cloudsmith push rpm kafe/libkafe/fedora/37 build-artifact/fedora-37/libkafe-devel-1.1.7-1.x86_64.f37.rpm

publish-fedora-38:
	cloudsmith push rpm kafe/libkafe/fedora/38 build-artifact/fedora-38/kafe-cli-1.1.7-1.x86_64.f38.rpm
	cloudsmith push rpm kafe/libkafe/fedora/38 build-artifact/fedora-38/libkafe-1.1.7-1.x86_64.f38.rpm
	cloudsmith push rpm kafe/libkafe/fedora/38 build-artifact/fedora-38/libkafe-devel-1.1.7-1.x86_64.f38.rpm

publish-all: \
	 publish-el-7 \
	 publish-el-8 \
	 publish-el-9 \
	 publish-debian-10 \
	 publish-debian-11 \
	 publish-debian-12 \
	 publish-ubuntu-1804 \
	 publish-ubuntu-2004 \
	 publish-ubuntu-2204 \
	 publish-fedora-36 \
	 publish-fedora-37 \
	 publish-fedora-38
