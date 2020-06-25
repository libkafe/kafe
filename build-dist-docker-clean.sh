#!/usr/bin/env bash

docker rmi kafe/centos:7-build
docker rmi kafe/centos:8-build
docker rmi kafe/debian:9-build
docker rmi kafe/debian:10-build
docker rmi kafe/debian:11-build
docker rmi kafe/ubuntu:1804-build
docker rmi kafe/ubuntu:1910-build
docker rmi kafe/ubuntu:2004-build
docker rmi kafe/ubuntu:2010-build
docker rmi kafe/fedora:31-build
docker rmi kafe/fedora:32-build
docker rmi kafe/fedora:33-build