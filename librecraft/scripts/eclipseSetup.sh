#!/bin/sh

cd ../..
rm -r Eclipse
mkdir Eclipse
cd Eclipse
mkdir data
touch data/meta
cmake -G "Eclipse CDT4 - Unix Makefiles" DCMAKE_BUILD_TYPE=Debug -DAPPLICATION=TRUE -DDEMO=FALSE -DWARNINGS=TRUE ../librecraft
cd ../librecraft/scripts
