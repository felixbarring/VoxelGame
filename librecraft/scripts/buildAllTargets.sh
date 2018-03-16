#!/bin/sh

cd ../build

debugBuilt=false
releaseBuilt=false
unitTestBuilt=false

if [ -d "debug" ]; then
  debugBuilt=true
  cd debug
  make -j8 |& tee -a build.out
  cd ..
fi


if [ -d "release" ]; then
  releaseBuilt=true
  cd release
  make -j8 |& tee -a build.out
  cd ..
fi


if [ -d "unitTest" ]; then
  unitTestBuilt=true
  cd unitTest
  make -j8 |& tee -a build.out
  cd ..
fi

RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

if $debugBuilt; then
  echo "Debug"
  echo -e "Number of ${RED}errors${NC} =" $(grep -e 'error' debug/build.out | grep -v dependencies | wc -l)
  echo -e "Number of ${YELLOW}warnings${NC} =" $(grep -e 'warning' debug/build.out | grep -v dependencies | wc -l)
  echo
fi

if $releaseBuilt; then
  echo "Release"
  echo -e "Number of ${RED}errors${NC} =" $(grep -e 'error' release/build.out | grep -v dependencies | wc -l)
  echo -e "Number of ${YELLOW}warnings${NC} =" $(grep -e 'warning' release/build.out | grep -v dependencies | wc -l)
  echo
fi

if $unitTestBuilt; then
  echo "UnitTest"
  echo -e "Number of ${RED}errors${NC} =" $(grep -e 'error' unitTest/build.out | grep -v dependencies | wc -l)
  echo -e "Number of ${YELLOW}warnings${NC} =" $(grep -e 'warning' unitTest/build.out | grep -v dependencies | wc -l)
  echo
fi

cd ../scripts

