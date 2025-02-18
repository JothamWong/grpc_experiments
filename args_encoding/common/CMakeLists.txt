cmake_minimum_required(VERSION 3.10)
project(common)

add_library(common
    src/Statistics.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)