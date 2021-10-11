#!/bin/bash
set -e

mkdir -p build-cucumber-cpp/build
mkdir -p build-cucumber-cpp/dist

cd build-cucumber-cpp/build
cmake -DCMAKE_INSTALL_PREFIX=../dist ../../cucumber-cpp
make install
