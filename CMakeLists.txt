cmake_minimum_required (VERSION 3.11)

project(raylibstarter)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory ("src")
