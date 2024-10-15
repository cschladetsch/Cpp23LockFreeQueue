# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(LockFreeQueueProject VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Thread Sanitizer if required
option(ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer" OFF)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Define the source files
set(SOURCES
    src/lock_free_queue_test.cpp
)

# Define the source files for performance comparison
set(PERFORMANCE_SOURCES
    src/performance_comparison.cpp
)

# Add the executable target for testing
add_executable(lock_free_queue_test ${SOURCES})

# Add the executable target for performance comparison
add_executable(performance_comparison ${PERFORMANCE_SOURCES})

# Add pthread linking
find_package(Threads REQUIRED)
target_link_libraries(lock_free_queue_test PRIVATE Threads::Threads)
target_link_libraries(performance_comparison PRIVATE Threads::Threads)

# Enable Thread Sanitizer if the option is turned on
if(ENABLE_THREAD_SANITIZER)
    message(STATUS "Enabling ThreadSanitizer")
    target_compile_options(lock_free_queue_test PRIVATE -fsanitize=thread)
    target_link_libraries(lock_free_queue_test PRIVATE -fsanitize=thread)
    target_compile_options(performance_comparison PRIVATE -fsanitize=thread)
    target_link_libraries(performance_comparison PRIVATE -fsanitize=thread)
endif()

# If you want to add sanitizers globally, uncomment the following lines:
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
# set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=thread")

# Create a test target
enable_testing()
add_test(NAME LockFreeQueueTest COMMAND lock_free_queue_test)

