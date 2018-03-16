#!/bin/sh

if [ -d "../build/debug" ]; then
  rm -r ../build/debug
fi

mkdir ../build/debug
mkdir ../build/debug/data
touch ../build/debug/data/meta
cp -al ../../resources ../build/debug

cd ../build/debug
cmake -DCMAKE_BUILD_TYPE=Debug -DAPPLICATION=TRUE -DDEMO=TRUE -DWARNINGS=TRUE ../..
cd ../../scripts
