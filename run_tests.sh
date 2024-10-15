#!/bin/bash

# Build the project with debug or release flags
BUILD_TYPE=${1:-Debug}

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
cmake --build .

# Optionally run with gdb for debugging
./performance_comparison

cd ..

