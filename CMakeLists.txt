cmake_minimum_required(VERSION 3.10)

project(LockFreeQueueComparison)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.82 REQUIRED COMPONENTS lockfree)

add_executable(run_tests src/run_tests.cpp)
target_link_libraries(run_tests Boost::lockfree)

