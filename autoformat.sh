#!/bin/bash

#
# Format source code
#

dirs="./tests/ ./cppboot/"
find $dirs -name '*.cc' -o -name '*.cpp' -o -name '*.hpp' -o -name '*.c' -o -name '*.h' | xargs clang-format -i