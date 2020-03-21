cmake_minimum_required(VERSION 3.15)
project(intersect_dll)

set(CMAKE_CXX_STANDARD 14)

add_library(dll SHARED src/Line.cpp src/Line.h)