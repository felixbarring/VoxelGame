#!/bin/sh

rm -r ../build
mkdir ../build
mkdir ../build/data
touch ../build/data/meta
cp -r ../../resources ../build

cd ../build
cmake -DCMAKE_BUILD_TYPE=Debug -DAPPLICATION=TRUE -DDEMO=TRUE -DWARNINGS=TRUE ..
cd ../scripts
