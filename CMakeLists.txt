cmake_minimum_required(VERSION 3.29)
project(cgui C)

set(CMAKE_C_STANDARD 11)

add_executable(cgui main.c
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
        deps/app/core.c
        deps/app/core.h
        deps/win32/message.c
        deps/win32/message.h
        deps/app/application.c
        deps/app/application.h
        deps/component/ui.c
        deps/component/ui.h
        deps/util/vector.c
        deps/util/vector.h
        deps/event/handler.c
        deps/event/handler.h
        deps/geo/layout.c
        deps/geo/layout.h
        deps/component/ui_window.c
        deps/component/ui_window.h
        deps/component/ui_window_proto.c
        deps/component/ui_window_proto.h
        deps/event/args.c
        deps/event/args.h
        deps/threading/thread.c
        deps/threading/thread.h
        deps/threading/threadpool.c
        deps/threading/threadpool.h
        deps/win32/gdi.c
        deps/win32/gdi.h
        deps/component/trait.c
        deps/component/trait.h
)
