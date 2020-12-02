cmake_minimum_required(VERSION 3.17)
project(Project3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project3
        avl.h
        main.cpp)
