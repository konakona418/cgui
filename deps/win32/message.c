//
// Created on: 2024/11/7, 19:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "message.h"

CGUI_MessageDispatcher* cgui_createMessageDispatcher(HWND hwnd, UINT filterMin, UINT filterMax) {
    CGUI_MessageDispatcher* dispatcher = (CGUI_MessageDispatcher*) malloc(sizeof(CGUI_MessageDispatcher));

    dispatcher->hwnd = hwnd;
    dispatcher->filterMin = filterMin;
    dispatcher->filterMax = filterMax;

    dispatcher->_dispatch = cgui_messageDispatcher_dispatch;
    dispatcher->beginDispatch = cgui_messageDispatcher_beginDispatch;

    return dispatcher;
}

void cgui_destroyMessageDispatcher(CGUI_MessageDispatcher* dispatcher) {
    free(dispatcher);
}

void cgui_messageDispatcher_dispatch(CGUI_MessageDispatcher* self, HWND hwnd, UINT filterMin, UINT filterMax) {
    MSG msg = { };
    while (GetMessage(&msg, hwnd, filterMin, filterMax) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void cgui_messageDispatcher_beginDispatch(CGUI_MessageDispatcher* self) {
    self->_dispatch(self, NULL, self->filterMin, self->filterMax);
}

CGUI_MessageHandler* cgui_createMessageHandler() {
    CGUI_MessageHandler* handler = (CGUI_MessageHandler*) malloc(sizeof(CGUI_MessageHandler));

    handler->_winProc = cgui_messageHandler_winProc;

    handler->onCreate = NULL;
    handler->onClose = NULL;
    handler->onDestroy = NULL;

    handler->onPaint = NULL;
    handler->onSize = NULL;

    return handler;
}

void cgui_destroyMessageHandler(CGUI_MessageHandler* handler) {
    free(handler);
}

LRESULT CALLBACK cgui_messageHandler_winProc(CGUI_MessageHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            if (self->onCreate) {
                self->onCreate(self, hwnd, wParam, lParam);
            }
            break;
        case WM_CLOSE:
            if (self->onClose) {
                self->onClose(self, hwnd, wParam, lParam);
            }
            break;
        case WM_DESTROY:
            if (self->onDestroy) {
                self->onDestroy(self, hwnd, wParam, lParam);
            }
            break;
        case WM_PAINT:
            if (self->onPaint) {
                self->onPaint(self, hwnd, wParam, lParam);
            }
        case WM_SIZE:
            if (self->onSize) {
                self->onSize(self, hwnd, wParam, lParam);
            }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}