cmake_minimum_required(VERSION 3.15)
project(intersect)

set(CMAKE_CXX_STANDARD 14)

link_directories(bin)

add_executable(intersect src/main.cpp src/Line.h)

target_link_libraries(intersect libdll.dll)