#!/bin/sh

rm -r ../build
mkdir ../build
touch ../build/data/meta
cd ../build
cmake ..
cd ../scripts
