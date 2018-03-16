#!/bin/sh

cd ../build

if [ -d "release" ]; then
  cd release
  make -j8 |& tee -a build.out
  cd ..
fi

cd ../scripts
