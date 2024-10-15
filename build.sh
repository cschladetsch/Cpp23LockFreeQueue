#!/bin/bash

# Unset CC and CXX environment variables
unset CC
unset CXX

# Create and enter build directory
mkdir -p build
cd build

# Run CMake
cmake ..

# Build the project
cmake --build .

# Return to the original directory
cd ..
