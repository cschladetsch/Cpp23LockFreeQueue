cmake_minimum_required(VERSION 3.14)

project(LockFreeQueue VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Download Rang
FetchContent_Declare(
  rang
  GIT_REPOSITORY https://github.com/agauniyal/rang.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(rang)

# Modify Rang's CMakeLists.txt to use a newer CMake version
file(WRITE ${rang_SOURCE_DIR}/CMakeLists.txt "cmake_minimum_required(VERSION 3.14)\n\
project(rang VERSION 3.2.0)\n\
add_library(rang INTERFACE)\n\
target_include_directories(rang INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)")

# Find threads package
find_package(Threads REQUIRED)

# Add option for using Boost
option(USE_BOOST "Use Boost libraries" OFF)

# If USE_BOOST is ON, find Boost package
if(USE_BOOST)
    find_package(Boost 1.74 REQUIRED COMPONENTS system thread)
    add_definitions(-DUSE_BOOST)
endif()

# Add main executable
add_executable(lock_free_queue_driver main.cpp)
target_link_libraries(lock_free_queue_driver PRIVATE Threads::Threads rang)

# Add comparison executable
add_executable(performance_comparison performance_comparison.cpp)
target_link_libraries(performance_comparison PRIVATE Threads::Threads rang)

if(USE_BOOST)
    target_link_libraries(performance_comparison PRIVATE Boost::system Boost::thread)
endif()

# Install
install(TARGETS lock_free_queue_driver performance_comparison
        RUNTIME DESTINATION bin)
