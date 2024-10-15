#!/bin/bash

unset CC
unset CXX

# Allow building in Release or Debug mode via an argument
BUILD_TYPE=${1:-Release}

# Create and enter build directory
mkdir -p build
cd build

# Run CMake with the selected build type
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..

# Build the project
cmake --build .

# Return to the original directory
cd ..

