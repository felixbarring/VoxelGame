#!/bin/sh
# Used to count lines of code
find . -name "*.h" -or -name "*.cpp" | grep -v './dependencies' | xargs wc -l
