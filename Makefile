clean:
	rm -rf build/
	rm -rf build-artifact/

clean-docker-build:
	docker rmi kafe/centos:7-build
	docker rmi kafe/almalinux:8-build
	docker rmi kafe/debian:9-build
	docker rmi kafe/debian:10-build
	docker rmi kafe/debian:11-build
	docker rmi kafe/debian:12-build
	docker rmi kafe/ubuntu:1804-build
	docker rmi kafe/ubuntu:2004-build
	docker rmi kafe/ubuntu:2204-build
	docker rmi kafe/fedora:31-build
	docker rmi kafe/fedora:32-build
	docker rmi kafe/fedora:33-build
	docker rmi kafe/fedora:34-build
	docker rmi kafe/fedora:35-build
	docker rmi kafe/fedora:36-build

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

build-debian-9:
	-rm -rf build-artifact/debian-9/
	if [[ "$(docker images -q "kafe/debian:9-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/debian:9-build dist/debian/9; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/debian:9-build
	mkdir -p build-artifact/debian-9/
	cp build/debian/9/kafe-cli*.deb build-artifact/debian-9/
	cp build/debian/9/libkafe*.deb build-artifact/debian-9/
	for f in build-artifact/debian-9/*; do mv -v "$$f" `sed "s/\.deb/\.deb9\.deb/" <<< "$$f"`; done

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

build-fedora-31:
	-rm -rf build-artifact/fedora-31/
	if [[ "$(docker images -q "kafe/fedora:31-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:31-build dist/fedora/31; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:31-build
	mkdir -p build-artifact/fedora-31/
	cp build/fedora/31/kafe-cli-*.rpm build-artifact/fedora-31/
	cp build/fedora/31/libkafe-*.rpm build-artifact/fedora-31/
	for f in build-artifact/fedora-31/*; do mv -v "$$f" `sed "s/\.rpm/\.f31\.rpm/" <<< "$$f"`; done

build-fedora-32:
	-rm -rf build-artifact/fedora-32/
	if [[ "$(docker images -q "kafe/fedora:32-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:32-build dist/fedora/32; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:32-build
	mkdir -p build-artifact/fedora-32/
	cp build/fedora/32/kafe-cli-*.rpm build-artifact/fedora-32/
	cp build/fedora/32/libkafe-*.rpm build-artifact/fedora-32/
	for f in build-artifact/fedora-32/*; do mv -v "$$f" `sed "s/\.rpm/\.f32\.rpm/" <<< "$$f"`; done

build-fedora-33:
	-rm -rf build-artifact/fedora-33/
	if [[ "$(docker images -q "kafe/fedora:33-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:33-build dist/fedora/33; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:33-build
	mkdir -p build-artifact/fedora-33/
	cp build/fedora/33/kafe-cli-*.rpm build-artifact/fedora-33/
	cp build/fedora/33/libkafe-*.rpm build-artifact/fedora-33/
	for f in build-artifact/fedora-33/*; do mv -v "$$f" `sed "s/\.rpm/\.f33\.rpm/" <<< "$$f"`; done

build-fedora-34:
	-rm -rf build-artifact/fedora-34/
	if [[ "$(docker images -q "kafe/fedora:34-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:34-build dist/fedora/34; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:34-build
	mkdir -p build-artifact/fedora-34/
	cp build/fedora/34/kafe-cli-*.rpm build-artifact/fedora-34/
	cp build/fedora/34/libkafe-*.rpm build-artifact/fedora-34/
	for f in build-artifact/fedora-34/*; do mv -v "$$f" `sed "s/\.rpm/\.f34\.rpm/" <<< "$$f"`; done

build-fedora-35:
	-rm -rf build-artifact/fedora-35/
	if [[ "$(docker images -q "kafe/fedora:35-build" 2> /dev/null)" == "" ]]; then \
	  docker build -t kafe/fedora:35-build dist/fedora/35; \
	fi

	docker run -it --rm -v `pwd`:/kafe:z kafe/fedora:35-build
	mkdir -p build-artifact/fedora-35/
	cp build/fedora/35/kafe-cli-*.rpm build-artifact/fedora-35/
	cp build/fedora/35/libkafe-*.rpm build-artifact/fedora-35/
	for f in build-artifact/fedora-35/*; do mv -v "$$f" `sed "s/\.rpm/\.f35\.rpm/" <<< "$$f"`; done

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

build-all: clean \
	 build-el-7 \
	 build-el-8 \
	 build-debian-9 \
	 build-debian-10 \
	 build-debian-11 \
	 build-debian-12 \
	 build-ubuntu-1804 \
	 build-ubuntu-2004 \
	 build-ubuntu-2204 \
	 build-fedora-31 \
	 build-fedora-32 \
	 build-fedora-33 \
	 build-fedora-34 \
	 build-fedora-35 \
	 build-fedora-36

publish-el-7:
	cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/kafe-cli-1.1.6-1.x86_64.el7.rpm
	cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/libkafe-1.1.6-1.x86_64.el7.rpm
	cloudsmith push rpm kafe/libkafe/el/7 build-artifact/centos-7/libkafe-devel-1.1.6-1.x86_64.el7.rpm

publish-el-8:
	cloudsmith push rpm kafe/libkafe/el/8 build-artifact/almalinux-8/kafe-cli-1.1.6-1.x86_64.el8.rpm
	cloudsmith push rpm kafe/libkafe/el/8 build-artifact/almalinux-8/libkafe-1.1.6-1.x86_64.el8.rpm
	cloudsmith push rpm kafe/libkafe/el/8 build-artifact/almalinux-8/libkafe-devel-1.1.6-1.x86_64.el8.rpm

publish-debian-9:
	cloudsmith push deb kafe/libkafe/debian/stretch build-artifact/debian-9/kafe-cli_1.1.6_amd64.deb9.deb
	cloudsmith push deb kafe/libkafe/debian/stretch build-artifact/debian-9/libkafe_1.1.6_amd64.deb9.deb
	cloudsmith push deb kafe/libkafe/debian/stretch build-artifact/debian-9/libkafe-dev_1.1.6_amd64.deb9.deb

publish-debian-10:
	cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/kafe-cli_1.1.6_amd64.deb10.deb
	cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/libkafe_1.1.6_amd64.deb10.deb
	cloudsmith push deb kafe/libkafe/debian/buster build-artifact/debian-10/libkafe-dev_1.1.6_amd64.deb10.deb

publish-debian-11:
	cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/kafe-cli_1.1.6_amd64.deb11.deb
	cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/libkafe_1.1.6_amd64.deb11.deb
	cloudsmith push deb kafe/libkafe/debian/bullseye build-artifact/debian-11/libkafe-dev_1.1.6_amd64.deb11.deb

publish-debian-12:
	cloudsmith push deb kafe/libkafe/debian/bookworm build-artifact/debian-12/kafe-cli_1.1.6_amd64.deb12.deb
	cloudsmith push deb kafe/libkafe/debian/bookworm build-artifact/debian-12/libkafe_1.1.6_amd64.deb12.deb
	cloudsmith push deb kafe/libkafe/debian/bookworm build-artifact/debian-12/libkafe-dev_1.1.6_amd64.deb12.deb

publish-ubuntu-1804:
	cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/kafe-cli_1.1.6_amd64.ubu1804.deb
	cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/libkafe_1.1.6_amd64.ubu1804.deb
	cloudsmith push deb kafe/libkafe/ubuntu/bionic build-artifact/ubuntu-1804/libkafe-dev_1.1.6_amd64.ubu1804.deb

publish-ubuntu-2004:
	cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/kafe-cli_1.1.6_amd64.ubu2004.deb
	cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/libkafe_1.1.6_amd64.ubu2004.deb
	cloudsmith push deb kafe/libkafe/ubuntu/focal build-artifact/ubuntu-2004/libkafe-dev_1.1.6_amd64.ubu2004.deb

publish-ubuntu-2204:
	cloudsmith push deb kafe/libkafe/ubuntu/jammy build-artifact/ubuntu-2204/kafe-cli_1.1.6_amd64.ubu2204.deb
	cloudsmith push deb kafe/libkafe/ubuntu/jammy build-artifact/ubuntu-2204/libkafe_1.1.6_amd64.ubu2204.deb
	cloudsmith push deb kafe/libkafe/ubuntu/jammy build-artifact/ubuntu-2204/libkafe-dev_1.1.6_amd64.ubu2204.deb

publish-fedora-31:
	cloudsmith push rpm kafe/libkafe/fedora/31 build-artifact/fedora-31/kafe-cli-1.1.6-1.x86_64.f31.rpm
	cloudsmith push rpm kafe/libkafe/fedora/31 build-artifact/fedora-31/libkafe-1.1.6-1.x86_64.f31.rpm
	cloudsmith push rpm kafe/libkafe/fedora/31 build-artifact/fedora-31/libkafe-devel-1.1.6-1.x86_64.f31.rpm

publish-fedora-32:
	cloudsmith push rpm kafe/libkafe/fedora/32 build-artifact/fedora-32/kafe-cli-1.1.6-1.x86_64.f32.rpm
	cloudsmith push rpm kafe/libkafe/fedora/32 build-artifact/fedora-32/libkafe-1.1.6-1.x86_64.f32.rpm
	cloudsmith push rpm kafe/libkafe/fedora/32 build-artifact/fedora-32/libkafe-devel-1.1.6-1.x86_64.f32.rpm

publish-fedora-33:
	cloudsmith push rpm kafe/libkafe/fedora/33 build-artifact/fedora-33/kafe-cli-1.1.6-1.x86_64.f33.rpm
	cloudsmith push rpm kafe/libkafe/fedora/33 build-artifact/fedora-33/libkafe-1.1.6-1.x86_64.f33.rpm
	cloudsmith push rpm kafe/libkafe/fedora/33 build-artifact/fedora-33/libkafe-devel-1.1.6-1.x86_64.f33.rpm

publish-fedora-34:
	cloudsmith push rpm kafe/libkafe/fedora/34 build-artifact/fedora-34/kafe-cli-1.1.6-1.x86_64.f34.rpm
	cloudsmith push rpm kafe/libkafe/fedora/34 build-artifact/fedora-34/libkafe-1.1.6-1.x86_64.f34.rpm
	cloudsmith push rpm kafe/libkafe/fedora/34 build-artifact/fedora-34/libkafe-devel-1.1.6-1.x86_64.f34.rpm

publish-fedora-35:
	cloudsmith push rpm kafe/libkafe/fedora/35 build-artifact/fedora-35/kafe-cli-1.1.6-1.x86_64.f35.rpm
	cloudsmith push rpm kafe/libkafe/fedora/35 build-artifact/fedora-35/libkafe-1.1.6-1.x86_64.f35.rpm
	cloudsmith push rpm kafe/libkafe/fedora/35 build-artifact/fedora-35/libkafe-devel-1.1.6-1.x86_64.f35.rpm

publish-fedora-36:
	cloudsmith push rpm kafe/libkafe/fedora/36 build-artifact/fedora-36/kafe-cli-1.1.6-1.x86_64.f36.rpm
	cloudsmith push rpm kafe/libkafe/fedora/36 build-artifact/fedora-36/libkafe-1.1.6-1.x86_64.f36.rpm
	cloudsmith push rpm kafe/libkafe/fedora/36 build-artifact/fedora-36/libkafe-devel-1.1.6-1.x86_64.f36.rpm

publish-all: \
	 publish-el-7 \
	 publish-el-8 \
	 publish-debian-9 \
	 publish-debian-10 \
	 publish-debian-11 \
	 publish-debian-12 \
	 publish-ubuntu-1804 \
	 publish-ubuntu-2004 \
	 publish-ubuntu-2204 \
	 publish-fedora-31 \
	 publish-fedora-32 \
	 publish-fedora-33 \
	 publish-fedora-34 \
	 publish-fedora-35 \
	 publish-fedora-36
