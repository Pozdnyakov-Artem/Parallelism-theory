cmake_minimum_required(VERSION 4.2)

project(first_task)
set(CMAKE_CXX_STANDARD 20)

add_executable(Main)

target_sources(Main
    PRIVATE
    main.cpp
)