//
// Created on: 2024/11/12, 10:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_ARGS_H
#define CGUI_ARGS_H

#include <windows.h>
#include "../util/common.h"

typedef struct EventArgs CGUI_EventArgs;

typedef struct MouseEventArgs CGUI_MouseEventArgs;

typedef struct MouseButton CGUI_MouseButton;

typedef enum MouseButtonState CGUI_MouseButtonState;

typedef struct MouseButtonKeyState CGUI_MouseButtonKeyState;

typedef enum KeyState CGUI_KeyState;

typedef enum MouseButtonState {
    LeftButton,
    RightButton,
    MiddleButton,
    XButton1,
    XButton2,
} CGUI_MouseButtonState;

typedef struct MouseButtonKeyState {
    bool isShiftDown;
    bool isCtrlDown;
} CGUI_MouseButtonKeyState;

typedef struct MouseButton {
    CGUI_MouseButtonState state;
    CGUI_MouseButtonKeyState keyState;
} CGUI_MouseButton;

CGUI_MouseButton cgui_asMouseButton(WPARAM wParam);

typedef enum KeyState {
    KeyDown,
    KeyUp
} CGUI_KeyState;

typedef struct EventArgs {
    void* component;

    HWND hwnd;
    UINT msg;

    WPARAM wParam;
    LPARAM lParam;

    void* argv;
    UINT argc;
} CGUI_EventArgs;

CGUI_EventArgs cgui_createEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct MouseEventArgs {
    CGUI_EventArgs base;
    POINT point;

    CGUI_MouseButton button;
} CGUI_MouseEventArgs;

CGUI_MouseEventArgs cgui_createMouseEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct KeyEventArgs CGUI_KeyEventArgs;

typedef struct KeyEventArgs {
    CGUI_EventArgs base;

    UINT keyCode;
    CGUI_KeyState keyState;
} CGUI_KeyEventArgs;

CGUI_KeyEventArgs cgui_createKeyEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct TextBoxEventArgs CGUI_TextBoxEventArgs;

typedef struct TextBoxEventArgs {
    CGUI_EventArgs base;

    LPCSTR text;
    UINT textLength;
} CGUI_TextBoxEventArgs;

CGUI_TextBoxEventArgs cgui_createTextBoxEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct GdiReadyEventArgs CGUI_GdiReadyEventArgs;

typedef struct GdiReadyEventArgs {
    CGUI_EventArgs base;

    HDC hdc;
    HWND hwnd;
} CGUI_GdiReadyEventArgs;

CGUI_GdiReadyEventArgs cgui_createGdiReadyEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif //CGUI_ARGS_H
