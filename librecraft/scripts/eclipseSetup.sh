#!/bin/sh

cd ../..

if [ -d "Eclipse" ]; then
  rm -r Eclipse
fi
mkdir Eclipse
cd Eclipse
mkdir data
touch data/meta
cp -r ../resources .
cmake -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DAPPLICATION=TRUE -DDEMO=TRUE -DWARNINGS=TRUE ../librecraft
cd ../librecraft/scripts
