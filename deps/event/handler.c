//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <stdlib.h>
#include "handler.h"
#include "../component/trait.h"

#include "../component/ui.h"
void cgui_eventHandler_defaultOnPaint(CGUI_EventArgs args) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) args.component;
    if (impl(component->implFlag, CGUI_Trait_UIDrawable)) {
        if (component->drawableImpl->refresh) {
            component->drawableImpl->refresh(component);
        }
    }
}

CGUI_EventHandler* cgui_createEventHandler(void* localHandler, LocalHandlerFlag handlerFlag) {
    CGUI_EventHandler* handler = (CGUI_EventHandler*) malloc(sizeof(CGUI_EventHandler));
    handler->component = NULL;

    handler->localHandler = localHandler;
    handler->handlerFlag = handlerFlag;

    handler->handleEvent = cgui_eventHandler_handleEvent;
    handler->setComponent = cgui_eventHandler_setComponent;

    return handler;
}

void cgui_eventHandler_setComponent(CGUI_EventHandler* handler, void* component) {
    handler->component = component;
}

void cgui_destroyEventHandler(CGUI_EventHandler* handler) {
    free(handler);
}

void cgui_routeToLocalHandler(CGUI_EventHandler* handler, void* localHandler, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (handle(handler->handlerFlag, CGUI_LocalHandler_WindowRoot)) {
        into(CGUI_WindowHandler, handler->localHandler)->handleEventLocal(localHandler, handler, hwnd, msg, wParam, lParam);
    }
    else {
        panic("Invalid handler type.");
    }
}

void cgui_eventHandler_handleEvent(CGUI_EventHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, CGUI_ApplicationMessageCallback callback) {
    if (msg == WM_COMMAND) {
        HWND hComponent = (HWND) lParam;
        if (unlikely(hwnd != hComponent)) {
            if (unlikely(hComponent == NULL)) {
                // todo: [IMPORTANT] when this value is NULL, it usually means that this message originates from a menu option.
                // add something to handle this.
            }

            if (likely(IsWindow(hComponent))) {
                callback(hComponent, msg, wParam, lParam);
                return;
            } else {
                printf("WM_COMMAND: Child component not a window. Ignoring.\n");
            }
        } else {
            printf("WM_COMMAND: child component hwnd eq hwnd.\n");
        }
    }
    cgui_routeToLocalHandler(self, self->localHandler, hwnd, msg, wParam, lParam);
}


typedef struct ButtonEventHandler CGUI_ButtonEventHandler;

typedef struct ButtonEventHandler {

} CGUI_ButtonEventHandler;

CGUI_WindowHandler* cgui_createWindowHandler() {
    CGUI_WindowHandler* handler = malloc(sizeof(CGUI_WindowHandler));

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

    handler->handleEventLocal = cgui_windowHandler_handleEventLocal;

    return handler;
}

void cgui_destroyWindowHandler(CGUI_WindowHandler* handler) {
    free(handler);
}

void cgui_windowHandler_handleEventLocal(CGUI_WindowHandler* self, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                         WPARAM wParam, LPARAM lParam) {
    CGUI_EventArgs args = cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam);
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
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onMouseMove(mouseArgs);
            }
            break;
        case WM_LBUTTONDOWN:
            if (self->onMouseDown) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != LeftButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = LeftButton;
                self->onMouseDown(mouseArgs);
            }
            break;
        case WM_LBUTTONUP:
            if (self->onMouseUp) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != LeftButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = LeftButton;
                self->onMouseUp(mouseArgs);
            }
            break;
        case WM_RBUTTONDOWN:
            if (self->onMouseDown) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != RightButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = RightButton;
                self->onMouseDown(mouseArgs);
            }
            break;
        case WM_RBUTTONUP:
            if (self->onMouseUp) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button != RightButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button = RightButton;
                self->onMouseUp(mouseArgs);
            }
            break;
        case WM_MOUSEWHEEL:
            if (self->onMouseWheel) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onMouseWheel(mouseArgs);
            }
            break;
        case WM_KEYDOWN:
            if (self->onKeyDown) {
                CGUI_KeyEventArgs keyArgs = cgui_createKeyEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onKeyDown(keyArgs);
            }
            break;
        case WM_KEYUP:
            if (self->onKeyUp) {
                CGUI_KeyEventArgs keyArgs = cgui_createKeyEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onKeyUp(keyArgs);
            }
            break;
        case WM_COMMAND:
            panic("WM_COMMAND: Unhandled command message!");
        default:
            break;
    }
}
