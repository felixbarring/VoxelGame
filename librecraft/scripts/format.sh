find ../src -name "*.h" -o -name "*.cpp" | grep -v './dependencies' |xargs clang-format -i -style=file

