cmake_minimum_required(VERSION 3.10)
project(LockFreeQueueTest)

set(CMAKE_CXX_STANDARD 20)

# Find the Boost library (no specific components needed)
find_package(Boost REQUIRED)

# Add Boost include directories
include_directories(${Boost_INCLUDE_DIRS} include)

# Add executables for the tests
add_executable(run_tests src/run_tests.cpp)
target_link_libraries(run_tests ${Boost_LIBRARIES})

