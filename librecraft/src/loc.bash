#!/bin/sh
# Used to count lines of code
find . -name "*.h" -or -name "*.cpp" | xargs wc -l
