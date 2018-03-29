#!/bin/sh

if [ -d "../build/unitTest" ]; then
  rm -r ../build/unitTest
fi

mkdir ../build/unitTest
cp -al ../../resources ../build/unitTest

cd ../build/unitTest
cmake -DUNIT_TEST=TRUE -DCMAKE_BUILD_TYPE=FALSE -DAPPLICATION=TRUE -DDEMO=FALSE -DWARNINGS=TRUE ../..
cd ../../scripts
