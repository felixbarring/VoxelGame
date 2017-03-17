#!/bin/sh

cd ../..
rm -r Eclipse
mkdir Eclipse
cd Eclipse
mkdir data
touch data/meta
cmake -G "Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug ../librecraft
cd ../librecraft/scripts
