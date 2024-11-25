//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_HANDLER_H
#define CGUI_HANDLER_H

#include <windows.h>
#include "args.h"
#include "../util/misc.h"

typedef struct EventHandler CGUI_EventHandler;

typedef struct WindowHandler CGUI_WindowHandler;

typedef struct ButtonHandler CGUI_ButtonHandler;

typedef struct TextBoxHandler CGUI_TextBoxHandler;

typedef struct ListBoxHandler CGUI_ListBoxHandler;

typedef unsigned int LocalHandlerFlag;

typedef void (*LocalEventHandler)    (void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define CGUI_LocalHandler_Default       0x0000
#define CGUI_LocalHandler_WindowRoot    0x0001

#define CGUI_LocalHandler_Button        0x0010
#define CGUI_LocalHandler_TextBox       0x0020

#define handle(_HandlerFlag, _Required) (((_HandlerFlag) & (_Required)) == (_Required))



typedef struct WindowHandler {
    void (*onClick)             (CGUI_MouseEventArgs args);
    void (*onDoubleClick)       (CGUI_MouseEventArgs args);

    void (*onMouseMove)         (CGUI_MouseEventArgs args);
    void (*onMouseDown)         (CGUI_MouseEventArgs args);
    void (*onMouseUp)           (CGUI_MouseEventArgs args);
    void (*onMouseWheel)        (CGUI_MouseEventArgs args);
    void (*onMouseLeave)        (CGUI_MouseEventArgs args);
    void (*onMouseHover)        (CGUI_MouseEventArgs args);

    void (*onKeyDown)           (CGUI_KeyEventArgs args);
    void (*onKeyUp)             (CGUI_KeyEventArgs args);

    void (*onFocus)             (CGUI_EventArgs args);

    void (*onSize)              (CGUI_EventArgs args);

    void (*onCreate)            (CGUI_EventArgs args);
    void (*onClose)             (CGUI_EventArgs args);
    void (*onDestroy)           (CGUI_EventArgs args);
    void (*onPaint)             (CGUI_EventArgs args); // todo: note that this event should call the paint function of the component (gdi WM_PAINT).

    LocalEventHandler           handleEventLocal;
} CGUI_WindowHandler;

CGUI_WindowHandler* cgui_createWindowHandler();

void cgui_destroyWindowHandler(CGUI_WindowHandler* handler);

void cgui_windowHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                         WPARAM wParam, LPARAM lParam);

typedef struct ButtonHandler {
    void (*onClick)        (CGUI_MouseEventArgs args);
    void (*onDoubleClick)  (CGUI_MouseEventArgs args);
    void (*onMouseDown)    (CGUI_MouseEventArgs args);
    void (*onMouseUp)      (CGUI_MouseEventArgs args);
    void (*onFocus)        (CGUI_EventArgs args);
    void (*onDefocus)      (CGUI_EventArgs args);

    LocalEventHandler      handleEventLocal;
} CGUI_ButtonHandler;

CGUI_ButtonHandler* cgui_createButtonHandler();

void cgui_destroyButtonHandler(CGUI_ButtonHandler* handler);

void cgui_buttonHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                         WPARAM wParam, LPARAM lParam);

typedef struct TextBoxHandler {
    void (*onTextChanged)   (CGUI_TextBoxEventArgs args);
    void (*onKeyDown)       (CGUI_KeyEventArgs args);
    void (*onKeyUp)         (CGUI_KeyEventArgs args);
    void (*onMouseDown)     (CGUI_MouseEventArgs args);
    void (*onMouseUp)       (CGUI_MouseEventArgs args);
    void (*onFocus)         (CGUI_EventArgs args);
    void (*onDefocus)       (CGUI_EventArgs args);

    LocalEventHandler    handleEventLocal;
} CGUI_TextBoxHandler;

CGUI_TextBoxHandler* cgui_createTextBoxHandler();

void cgui_destroyTextBoxHandler(CGUI_TextBoxHandler* handler);

void cgui_textBoxHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                          WPARAM wParam, LPARAM lParam);

typedef struct ListBoxHandler {
    void (*onItemSelected) (CGUI_EventArgs args);
    void (*onItemDoubleClicked) (CGUI_EventArgs args);
    void (*onMouseDown)   (CGUI_MouseEventArgs args);
    void (*onMouseUp)     (CGUI_MouseEventArgs args);
    void (*onMouseWheel) (CGUI_MouseEventArgs args);
    void (*onFocus) (CGUI_EventArgs args);
    void (*onDefocus) (CGUI_EventArgs args);

    LocalEventHandler handleEventLocal;
} CGUI_ListBoxHandler;

CGUI_ListBoxHandler* cgui_createListBoxHandler();

void cgui_destroyListBoxHandler(CGUI_ListBoxHandler* handler);

void cgui_listBoxHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                          WPARAM wParam, LPARAM lParam);

typedef struct EventHandler {
    void* component;

    LocalHandlerFlag handlerFlag;
    void* localHandler;

    void (*handleEvent)     (CGUI_EventHandler* self, CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam, CGUI_ApplicationMessageCallback callback);

    void (*setComponent)    (CGUI_EventHandler* self, void* component);
} CGUI_EventHandler;

CGUI_EventHandler* cgui_createEventHandler(void* localHandler, LocalHandlerFlag handlerFlag);

void cgui_eventHandler_setComponent(CGUI_EventHandler* handler, void* component);

void cgui_destroyEventHandler(CGUI_EventHandler* handler);

void cgui_eventHandler_handleEvent(CGUI_EventHandler* self, CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam, CGUI_ApplicationMessageCallback callback);

void cgui_eventHandler_defaultOnPaint(CGUI_EventArgs args);

#endif //CGUI_HANDLER_H
