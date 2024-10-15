#!/bin/bash

# Build the project with debug symbols
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Run the program with GDB
#gdb -ex "run" -ex "bt full" -ex "quit" --args ./performance_comparison
./performance_comparison

cd ..
