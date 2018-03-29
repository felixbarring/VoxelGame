#!/bin/sh

if [ -d "../build/release" ]; then
  rm -r ../build/release
fi

mkdir ../build/release
mkdir ../build/release/data
touch ../build/release/data/meta
cp -al ../../resources ../build/release

cd ../build/release
cmake -DCMAKE_BUILD_TYPE=Release -DAPPLICATION=TRUE -DDEMO=FALSE -DWARNINGS=TRUE -DUNIT_TEST=FALSE ../..
cd ../../scripts
