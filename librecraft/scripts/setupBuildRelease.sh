#!/bin/sh

rm -r ../build
mkdir ../build
mkdir ../build/data
touch ../build/data/meta

cd ../build
cmake -DCMAKE_BUILD_TYPE=Release -DAPPLICATION=TRUE -DDEMO=FALSE -DWARNINGS=TRUE ..
cd ../scripts
