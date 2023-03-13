cmake_minimum_required(VERSION 3.12)

project(VI-RT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wsign-conversion -pedantic-errors -O2 -DNDEBUG")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")
include_directories(include lib/tiny_obj_loader/include)
add_executable(program ${SOURCES})
