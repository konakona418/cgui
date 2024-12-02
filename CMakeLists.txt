cmake_minimum_required(VERSION 3.29)
project(include/cgui C)

set(CMAKE_C_STANDARD 11)

add_executable(cgui main.c
        include/cgui/win32/window.c
        include/cgui/win32/window.h
        include/cgui/geo/rect.c
        include/cgui/geo/rect.h
        include/cgui/util/common.c
        include/cgui/util/common.h
        include/cgui/util/error.h
        include/cgui/util/error_msg.h
        include/cgui/util/obsolete.c
        include/cgui/util/obsolete.h
        include/cgui/util/hashmap.c
        include/cgui/util/hashmap.h
        include/cgui/util/iterator.c
        include/cgui/util/iterator.h
        include/cgui/app/core.c
        include/cgui/app/core.h
        include/cgui/win32/message.c
        include/cgui/win32/message.h
        include/cgui/app/application.c
        include/cgui/app/application.h
        include/cgui/component/ui.c
        include/cgui/component/ui.h
        include/cgui/util/vector.c
        include/cgui/util/vector.h
        include/cgui/event/handler.c
        include/cgui/event/handler.h
        include/cgui/geo/layout.c
        include/cgui/geo/layout.h
        include/cgui/component/ui_window.c
        include/cgui/component/ui_window.h
        include/cgui/event/args.c
        include/cgui/event/args.h
        include/cgui/threading/thread.c
        include/cgui/threading/thread.h
        include/cgui/threading/threadpool.c
        include/cgui/threading/threadpool.h
        include/cgui/win32/gdi.c
        include/cgui/win32/gdi.h
        include/cgui/component/trait.c
        include/cgui/component/trait.h
        include/cgui/component/ui_factory.c
        include/cgui/component/ui_factory.h
        include/cgui/util/misc.h
        include/cgui/util/misc.c
        include/cgui/util/misc.c
        include/cgui/component/ui_label.c
        include/cgui/component/ui_label.h
        include/cgui/component/ui_textbox.c
        include/cgui/component/ui_textbox.h
        include/cgui/component/ui_button.c
        include/cgui/component/ui_button.h
        include/cgui/component/ui_list.c
        include/cgui/component/ui_list.h
        include/cgui/threading/lock.c
        include/cgui/threading/lock.h
)
