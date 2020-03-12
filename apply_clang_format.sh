#!/bin/bash
find -maxdepth 3 -type f \( -name "*.h" -o -name "*.cpp" \) -print -exec clang-format -i -style=Google {} \;

