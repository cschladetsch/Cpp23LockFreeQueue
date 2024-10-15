#!/bin/bash

set -e

EXECUTABLE="lock_free_queue_driver"
MAIN_FILE="main.cpp"
RANG_FILE="rang.hpp"
RANG_URL="https://raw.githubusercontent.com/agauniyal/rang/master/include/rang.hpp"

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

if ! command -v g++ &> /dev/null
then
    echo -e "${RED}Error: g++ is not installed. Please install g++ and try again.${NC}"
    exit 1
fi

# Download Rang if not present
if [ ! -f "$RANG_FILE" ]; then
    echo -e "${YELLOW}Downloading Rang library...${NC}"
    if command -v curl &> /dev/null; then
        curl -s -o "$RANG_FILE" "$RANG_URL"
    elif command -v wget &> /dev/null; then
        wget -q -O "$RANG_FILE" "$RANG_URL"
    else
        echo -e "${RED}Error: Neither curl nor wget is installed. Please install one of them and try again.${NC}"
        exit 1
    fi
    echo -e "${GREEN}Rang library downloaded successfully.${NC}"
else
    echo -e "${GREEN}Rang library already present.${NC}"
fi

echo -e "${YELLOW}Starting build process for lock-free queue...${NC}"

echo -e "${YELLOW}Compiling $MAIN_FILE...${NC}"
g++ -std=c++23 -pthread -O3 -Wall -Wextra -pedantic $MAIN_FILE -o $EXECUTABLE

if [ $? -eq 0 ]; then
    echo -e "${GREEN}Build successful! Executable '$EXECUTABLE' created.${NC}"
else
    echo -e "${RED}Build failed. Please check the error messages above.${NC}"
    exit 1
fi

echo -e "\n${YELLOW}To run the program, use:${NC}"
echo -e "${GREEN}./$EXECUTABLE <p> <c> <i>${NC}"
echo -e "${YELLOW}Where:${NC}"
echo -e "  ${GREEN}p${NC} is the number of producers"
echo -e "  ${GREEN}c${NC} is the number of consumers"
echo -e "  ${GREEN}i${NC} is the number of items per producer"
echo -e "\n${YELLOW}For example:${NC}"
echo -e "${GREEN}./$EXECUTABLE 3 2 10000${NC}"
