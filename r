#!/bin/bash

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default arguments
DEFAULT_P=3
DEFAULT_C=2
DEFAULT_I=10000

VERBOSE=false

# Function to display usage
usage() {
    echo -e "${YELLOW}Usage: $0 [-v|--verbose] [p] [c] [i]${NC}"
    echo -e "  ${BLUE}p: number of producers (default: $DEFAULT_P)${NC}"
    echo -e "  ${BLUE}c: number of consumers (default: $DEFAULT_C)${NC}"
    echo -e "  ${BLUE}i: items per producer (default: $DEFAULT_I)${NC}"
    exit 1
}

# Parse verbose flag
if [[ "$1" == "-v" || "$1" == "--verbose" ]]; then
    VERBOSE=true
    shift
fi

# Build the project
if $VERBOSE; then
    echo -e "${YELLOW}Building the project...${NC}"
fi
BUILD_OUTPUT=$(./build.sh 2>&1)
BUILD_STATUS=$?

# Check if build was successful
if [ $BUILD_STATUS -ne 0 ]; then
    echo -e "${RED}Build failed. Run with -v for more information.${NC}"
    if $VERBOSE; then
        echo "$BUILD_OUTPUT"
    fi
    exit 1
fi

# Set arguments
P=${1:-$DEFAULT_P}
C=${2:-$DEFAULT_C}
I=${3:-$DEFAULT_I}

# Validate arguments
if ! [[ $P =~ ^[0-9]+$ ]] || ! [[ $C =~ ^[0-9]+$ ]] || ! [[ $I =~ ^[0-9]+$ ]]; then
    echo -e "${RED}Error: Arguments must be positive integers.${NC}"
    usage
fi

# Run the program
if $VERBOSE; then
    echo -e "${GREEN}Running with:${NC}"
    echo -e "  ${BLUE}p:${NC} $P producers"
    echo -e "  ${BLUE}c:${NC} $C consumers"
    echo -e "  ${BLUE}i:${NC} $I items per producer"
fi

./lock_free_queue_driver $P $C $I

# Display brief summary
echo -e "${GREEN}Run completed.${NC}"
if $VERBOSE; then
    echo -e "${YELLOW}For more details, check the output above.${NC}"
fi
