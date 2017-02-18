#!/bin/sh

cd ..
rm -r build
mkdir build
cd build
mkdir data
cd data
touch meta
cd ..
cmake -DCMAKE_BUILD_TYPE=Release -DDEMO=TRUE -DWARNINGS=FALSE ..
cd ../scripts
