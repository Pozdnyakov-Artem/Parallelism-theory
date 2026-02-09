cmake_minimum_required(VERSION 3.1)

project(first_task)
set(CMAKE_CXX_STANDARD 11)

add_executable(Main)

target_sources(Main
    PRIVATE
    main.cpp
)

target_link_libraries(Main PRIVATE m)