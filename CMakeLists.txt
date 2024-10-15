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

# Link Threads library
find_package(Threads REQUIRED)

option(USE_BOOST "Use Boost libraries" OFF)

if(USE_BOOST)
    find_package(Boost 1.74 REQUIRED COMPONENTS system thread)
    add_definitions(-DUSE_BOOST)
endif()

add_executable(lock_free_queue_driver main.cpp)
target_link_libraries(lock_free_queue_driver PRIVATE Threads::Threads rang)

add_executable(performance_comparison performance_comparison.cpp)
target_link_libraries(performance_comparison PRIVATE Threads::Threads rang)

if(USE_BOOST)
    target_link_libraries(performance_comparison PRIVATE Boost::system Boost::thread)
endif()

install(TARGETS lock_free_queue_driver performance_comparison
        RUNTIME DESTINATION bin)

