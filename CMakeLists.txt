cmake_minimum_required(VERSION 3.29)
project(cgui C)

set(CMAKE_C_STANDARD 11)

add_executable(cgui main.c
        deps/win32/window.c
        deps/win32/window.h
        deps/win32/window.c
        deps/win32/window.h
        deps/win32/window.c
        deps/win32/window.h
        deps/geo/rect.c
        deps/geo/rect.h
        deps/util/common.c
        deps/util/common.h
        deps/util/error.h
        deps/util/error_msg.h
        deps/util/obsolete.c
        deps/util/obsolete.h
        deps/util/hashmap.c
        deps/util/hashmap.h
        deps/util/iterator.c
        deps/util/iterator.h
        deps/core.c
        deps/core.h
        deps/win32/message.c
        deps/win32/message.h)
