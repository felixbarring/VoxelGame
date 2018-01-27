#!/bin/sh

cd ../build
make -j8 &> build.out

RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "Number of ${RED}errors${NC} =" $(grep -e 'error' build.out | wc -l)
echo -e "Number of ${YELLOW}warnings${NC} =" $(grep -v libnoise build.out | grep -e 'warning'  | wc -l)

cd ../scripts

