#!/bin/bash

# Clear the terminal
clear

# Step 1: Build the project
echo -e "\033[1;34mBuilding the project...\033[0m"
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. || exit 1
cmake --build . || exit 1

# Step 2: Run the performance comparison tests
echo -e "\033[1;34mRunning the performance comparison...\033[0m"
./run_tests || exit 1

# Step 3: Display the results
echo -e "\033[1;32mAll tests completed successfully.\033[0m"

# Optional: Clean up the build (uncomment if needed)
# echo -e "\033[1;31mCleaning up...\033[0m"
# rm -rf build

# Return to the root directory
cd ..

