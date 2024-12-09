cmake_minimum_required(VERSION 3.29)
project(cgui C)

set(CMAKE_C_STANDARD 11)

include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE CGUI_SOURCES "${CMAKE_SOURCE_DIR}/include/cgui/*.c")
file(GLOB_RECURSE DEMO_SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(cgui main.c ${DEMO_SOURCES} ${CGUI_SOURCES}
        src/main_window.h)

