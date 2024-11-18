//
// Created on: 2024/11/12, 10:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "args.h"

CGUI_EventArgs cgui_createEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    CGUI_EventArgs args = {
        .argc = 0,
        .argv = NULL,
        .component = component,
        .hwnd = hwnd,
        .lParam = lParam,
        .wParam = wParam,
        .msg = msg,
    };
    return args;
}

CGUI_MouseButton cgui_asMouseButton(WPARAM wParam) {
    switch (wParam & 0x000F) {
        case 0:
            return LeftButton;
        case 1:
            return RightButton;
        case 2:
            return MiddleButton;
        default:
            return LeftButton;
    }
}

CGUI_MouseEventArgs cgui_createMouseEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // todo: remains to be checked
    return (CGUI_MouseEventArgs) {
        .base = cgui_createEventArgs(component, hwnd, msg, wParam, lParam),
        .button = cgui_asMouseButton(wParam),
        .point = {
            .x = (short) (LOWORD(lParam)),
            .y = (short) (HIWORD(lParam)),
        },
    };
}


CGUI_KeyEventArgs cgui_createKeyEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // todo: remains to be checked
    return (CGUI_KeyEventArgs) {
        .base = cgui_createEventArgs(component, hwnd, msg, wParam, lParam),
        .keyCode = (UINT) wParam,
        .keyState = (CGUI_KeyState) (lParam & 0x000F),
    };
}