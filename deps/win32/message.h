//
// Created on: 2024/11/7, 19:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_MESSAGE_H
#define CGUI_MESSAGE_H

#include <windows.h>

#include "../util/common.h"

typedef struct MessageDispatcher CGUI_MessageDispatcher;

typedef struct MessageHandler CGUI_MessageHandler;

/* Structure of MessageDispatcher */
typedef struct MessageDispatcher {
    HWND hwnd;
    UINT filterMin;
    UINT filterMax;

    void (* _dispatch)(CGUI_MessageDispatcher* self, HWND hwnd, UINT filterMin, UINT filterMax);

    void (* beginDispatch)(CGUI_MessageDispatcher* self);
} CGUI_MessageDispatcher;

#define CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MIN 0
#define CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MAX 0

/* Constructor and Destructor */
CGUI_MessageDispatcher* cgui_createMessageDispatcher(HWND hwnd, UINT filterMin, UINT filterMax);

void cgui_destroyMessageDispatcher(CGUI_MessageDispatcher* dispatcher);

/* The default message dispatcher */
void cgui_messageDispatcher_dispatch(CGUI_MessageDispatcher* self, HWND hwnd, UINT filterMin, UINT filterMax);

void cgui_messageDispatcher_beginDispatch(CGUI_MessageDispatcher* self);


/* Structure of MessageHandler */
typedef struct MessageHandler {
    LRESULT CALLBACK (* _winProc)(CGUI_MessageHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void (* onCreate)(CGUI_MessageHandler* self, HWND hwnd, WPARAM wParam, LPARAM lParam);

    void (* onClose)(CGUI_MessageHandler* self, HWND hwnd, WPARAM wParam, LPARAM lParam);

    void (* onDestroy)(CGUI_MessageHandler* self, HWND hwnd, WPARAM wParam, LPARAM lParam);

    void (* onPaint)(CGUI_MessageHandler* self, HWND hwnd, WPARAM wParam, LPARAM lParam);

    void (* onSize)(CGUI_MessageHandler* self, HWND hwnd, WPARAM wParam, LPARAM lParam);
    // todo: fill more callbacks.
} CGUI_MessageHandler;

/* Constructor and Destructor */
CGUI_MessageHandler* cgui_createMessageHandler();

void cgui_destroyMessageHandler(CGUI_MessageHandler* handler);

/* The default message handler & router. */
LRESULT CALLBACK
cgui_messageHandler_winProc(CGUI_MessageHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif //CGUI_MESSAGE_H
