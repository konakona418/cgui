cmake_minimum_required(VERSION 3.29)
project(cgui C)

set(CMAKE_C_STANDARD 11)

include_directories("${CMAKE_SOURCE_DIR}/include/cgui")
file(GLOB_RECURSE CGUI_SOURCES "${CMAKE_SOURCE_DIR}/include/cgui/.c")

add_executable(cgui main.c ${CGUI_SOURCES})

