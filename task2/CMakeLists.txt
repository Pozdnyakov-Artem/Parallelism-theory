cmake_minimum_required(VERSION 3.5)

project(2.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(first)

target_sources(first
    PRIVATE
    1.cc
)