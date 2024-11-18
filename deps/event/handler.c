//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <stdlib.h>
#include "handler.h"
#include "../component/trait.h"

CGUI_EventHandler* cgui_createEventHandler() {
    CGUI_EventHandler* handler = (CGUI_EventHandler*) malloc(sizeof(CGUI_EventHandler));
    handler->component = NULL;

    handler->handleEvent = cgui_eventHandler_handleEvent;
    handler->setComponent = cgui_eventHandler_setComponent;

    handler->onCreate = NULL;
    handler->onDestroy = NULL;
    handler->onClose = NULL;

    handler->onPaint = cgui_eventHandler_defaultOnPaint;

    handler->onSize = NULL;
    handler->onFocus = NULL;

    handler->onMouseDown = NULL;
    handler->onMouseUp = NULL;
    handler->onMouseMove = NULL;
    handler->onMouseHover = NULL;
    handler->onMouseLeave = NULL;
    handler->onMouseWheel = NULL;

    handler->onClick = NULL;
    handler->onDoubleClick = NULL;
    handler->onKeyDown = NULL;
    handler->onKeyUp = NULL;

    return handler;
}

void cgui_eventHandler_setComponent(CGUI_EventHandler* handler, void* component) {
    handler->component = component;
}

#include "../component/ui.h"
void cgui_eventHandler_defaultOnPaint(CGUI_EventArgs args) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) args.component;
    if (impl(component->implFlag, CGUI_Trait_UIDrawable)) {
        if (component->drawableImpl->refresh) {
            component->drawableImpl->refresh(component);
        }
    }
}

void cgui_destroyEventHandler(CGUI_EventHandler* handler) {
    free(handler);
}

void cgui_eventHandler_handleEvent(CGUI_EventHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    CGUI_EventArgs args = cgui_createEventArgs(self->component, hwnd, msg, wParam, lParam);
    switch (msg) {
        case WM_CREATE:
            if (self->onCreate) {
                self->onCreate(args);
            }
            break;
        case WM_DESTROY:
            if (self->onDestroy) {
                self->onDestroy(args);
            }
            break;
        case WM_CLOSE:
            if (self->onClose) {
                self->onClose(args);
            }
            break;
        case WM_PAINT:
            if (self->onPaint) {
                self->onPaint(args);
            }
            break;
        case WM_SIZE:
            if (self->onSize) {
                self->onSize(args);
            }
            break;
        case WM_SETFOCUS:
            if (self->onFocus) {
                self->onFocus(args);
            }
            break;
        case WM_MOUSEMOVE:
            if (self->onMouseMove) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(self->component, hwnd, msg, wParam, lParam);
                self->onMouseMove(mouseArgs);
            }
            break;
        case WM_LBUTTONDOWN:
            if (self->onMouseDown) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(self->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != LeftButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = LeftButton;
                self->onMouseDown(mouseArgs);
            }
            break;
        case WM_LBUTTONUP:
            if (self->onMouseUp) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(self->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != LeftButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = LeftButton;
                self->onMouseUp(mouseArgs);
            }
            break;
        case WM_RBUTTONDOWN:
            if (self->onMouseDown) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(self->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != RightButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = RightButton;
                self->onMouseDown(mouseArgs);
            }
            break;
        case WM_RBUTTONUP:
            if (self->onMouseUp) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(self->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != RightButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = RightButton;
                self->onMouseUp(mouseArgs);
            }
            break;
        case WM_MOUSEWHEEL:
            if (self->onMouseWheel) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(self->component, hwnd, msg, wParam, lParam);
                self->onMouseWheel(mouseArgs);
            }
            break;
        case WM_KEYDOWN:
            if (self->onKeyDown) {
                CGUI_KeyEventArgs keyArgs = cgui_createKeyEventArgs(self->component, hwnd, msg, wParam, lParam);
                self->onKeyDown(keyArgs);
            }
            break;
        case WM_KEYUP:
            if (self->onKeyUp) {
                CGUI_KeyEventArgs keyArgs = cgui_createKeyEventArgs(self->component, hwnd, msg, wParam, lParam);
                self->onKeyUp(keyArgs);
            }
            break;
        default:
            break;
    }
}
