#!/bin/zsh

# Define defaults for command line arguments
BUILD_TYPE=${1:-Release}        # Default to Release build
NUM_OPERATIONS=${2:-1000000}    # Default number of operations
NUM_RUNS=${3:-10}               # Default number of times to run the tests

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[0;33m'
RESET='\033[0m'

echo -e "${BLUE}Building the project...${RESET}"

# Build the project
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} .. && cmake --build .
if [[ $? -ne 0 ]]; then
    echo -e "${RED}Build failed.${RESET}"
    exit 1
fi

echo -e "${GREEN}Build completed successfully.${RESET}"

# Run tests
echo -e "${YELLOW}Running the performance comparison...${RESET}"

./run_tests ${NUM_OPERATIONS} ${NUM_RUNS}
if [[ $? -ne 0 ]]; then
    echo -e "${RED}Test execution failed.${RESET}"
    exit 1
fi

# Generate summary (run_tests.cpp will take care of summarizing the results)
