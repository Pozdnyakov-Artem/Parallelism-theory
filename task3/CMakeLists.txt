cmake_minimum_required(VERSION 3.15)

project(laba)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(first)

target_sources(first
    PRIVATE
    task1/1.cc
)

target_link_libraries(first PRIVATE Threads::Threads)