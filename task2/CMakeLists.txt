# export OMP_NUM_THREADS

cmake_minimum_required(VERSION 3.5)

project(2.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_executable(first)
add_executable(first2)
add_executable(first_lin)

add_executable(second)
add_executable(second2)
add_executable(second_lin)

add_executable(third)
add_executable(third2)
add_executable(third_lin)

target_sources(first
    PRIVATE
    parallel_programs/1.cc
)

target_sources(first2
    PRIVATE
    parallel_programs/1_2.cc
)

target_sources(first_lin
    PRIVATE
    linear_programs/linear_prog1.cc
)

target_sources(second
    PRIVATE
    parallel_programs/2.cc
)

target_sources(second2
    PRIVATE
    parallel_programs/2_2.cc
)

target_sources(second_lin
    PRIVATE
    linear_programs/linear_prog2.cc
)

target_sources(third
    PRIVATE
    parallel_programs/3.cc
)

target_sources(third2
    PRIVATE
    parallel_programs/3_2.cc
)

target_sources(third_lin
    PRIVATE
    linear_programs/linear_prog3.cc
)