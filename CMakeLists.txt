# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(LockFreeQueueProject VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost library (needed for Boost Lockfree queue)
find_package(Boost 1.82 REQUIRED)

# Enable Thread Sanitizer if required
option(ENABLE_THREAD_SANITIZER "Enable ThreadSanitizer" OFF)

# Add include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Define source files for lock_free_queue_test
set(LOCK_FREE_QUEUE_TEST_SOURCES
    src/lock_free_queue_test.cpp
)

# Define source files for run_tests
set(RUN_TESTS_SOURCES
    src/run_tests.cpp
)

# Create executable for lock_free_queue_test
add_executable(lock_free_queue_test ${LOCK_FREE_QUEUE_TEST_SOURCES})

# Link Boost and pthread libraries for lock_free_queue_test
find_package(Threads REQUIRED)
target_link_libraries(lock_free_queue_test PRIVATE Threads::Threads)

# Enable Thread Sanitizer if the option is turned on
if(ENABLE_THREAD_SANITIZER)
    message(STATUS "Enabling ThreadSanitizer for lock_free_queue_test")
    target_compile_options(lock_free_queue_test PRIVATE -fsanitize=thread)
    target_link_libraries(lock_free_queue_test PRIVATE -fsanitize=thread)
endif()

# Create executable for run_tests
add_executable(run_tests ${RUN_TESTS_SOURCES})

# Link Boost and pthread libraries for run_tests
target_link_libraries(run_tests PRIVATE Threads::Threads Boost::boost)

# Enable Thread Sanitizer if the option is turned on
if(ENABLE_THREAD_SANITIZER)
    message(STATUS "Enabling ThreadSanitizer for run_tests")
    target_compile_options(run_tests PRIVATE -fsanitize=thread)
    target_link_libraries(run_tests PRIVATE -fsanitize=thread)
endif()

# Create a test target (for lock_free_queue_test)
enable_testing()
add_test(NAME LockFreeQueueTest COMMAND lock_free_queue_test)

