cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 20)

add_executable(first)

target_sources(first
    PRIVATE
    task1/1.cc
)