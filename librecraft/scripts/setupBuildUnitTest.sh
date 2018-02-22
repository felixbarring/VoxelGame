#!/bin/sh

rm -r ../build
mkdir ../build

cd ../build
cmake -DUNIT_TEST=TRUE -DCMAKE_BUILD_TYPE=FALSE -DAPPLICATION=TRUE -DDEMO=FALSE -DWARNINGS=TRUE ..
cd ../scripts
