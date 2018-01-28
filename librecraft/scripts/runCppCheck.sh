#!/bin/sh

cppcheck -j 8 --enable=all -i../src/dependencies ../src | grep -e error -e warning -e style -e performance -e portability -e information
