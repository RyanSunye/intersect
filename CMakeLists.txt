cmake_minimum_required(VERSION 3.15)
project(intersect)

set(CMAKE_CXX_STANDARD 14)

add_executable(intersect src/main.cpp src/Line.h src/Line.cpp)

