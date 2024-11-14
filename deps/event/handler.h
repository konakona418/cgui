//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_HANDLER_H
#define CGUI_HANDLER_H

#include <windows.h>
#include "args.h"

typedef struct EventHandler CGUI_EventHandler;

typedef struct EventHandler {
    void (*onClick)         (CGUI_MouseEventArgs args);
    void (*onDoubleClick)   (CGUI_MouseEventArgs args);

    void (*onMouseMove)     (CGUI_MouseEventArgs args);
    void (*onMouseDown)     (CGUI_MouseEventArgs args);
    void (*onMouseUp)       (CGUI_MouseEventArgs args);
    void (*onMouseWheel)    (CGUI_MouseEventArgs args);
    void (*onMouseLeave)    (CGUI_MouseEventArgs args);
    void (*onMouseHover)    (CGUI_MouseEventArgs args);

    void (*onKeyDown)       (CGUI_KeyEventArgs args);
    void (*onKeyUp)         (CGUI_KeyEventArgs args);

    void (*onFocus)         (CGUI_EventArgs args);

    void (*onSize)          (CGUI_EventArgs args);

    void (*onCreate)        (CGUI_EventArgs args);
    void (*onClose)         (CGUI_EventArgs args);
    void (*onDestroy)       (CGUI_EventArgs args);
    void (*onPaint)         (CGUI_EventArgs args);

    void (*handleEvent)     (CGUI_EventHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
} CGUI_EventHandler;

CGUI_EventHandler* cgui_createEventHandler();

void cgui_destroyEventHandler(CGUI_EventHandler* handler);

void cgui_eventHandler_handleEvent(CGUI_EventHandler* handler, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif //CGUI_HANDLER_H
