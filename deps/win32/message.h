//
// Created on: 2024/11/7, 19:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_MESSAGE_H
#define CGUI_MESSAGE_H

#include <windows.h>

#include "../util/common.h"
#include "../util/misc.h"
#include "../threading/thread.h"

typedef struct MessageDispatcher CGUI_MessageDispatcher;

typedef struct MessageHandler CGUI_MessageHandler;

typedef struct DispatcherThreadStart CGUI_DispatcherThreadStart;

typedef struct DispatcherThreadStart {
    CGUI_MessageDispatcher* dispatcher;
    HWND hwnd;
    UINT filterMin;
    UINT filterMax;
} CGUI_DispatcherThreadStart;

CGUI_DispatcherThreadStart* cgui_createDispatcherThreadStart(CGUI_MessageDispatcher* dispatcher);
void cgui_destroyDispatcherThreadStart(CGUI_DispatcherThreadStart* threadStart);

/* Structure of MessageDispatcher */
typedef struct MessageDispatcher {
    HWND hwnd;
    UINT filterMin;
    UINT filterMax;

    bool isDispatching;
    CGUI_Thread* thread;

    void* (* _dispatch)(void* ts);

    void (* beginDispatch)(CGUI_MessageDispatcher* self);

    void (* beginDispatchAsync)(CGUI_MessageDispatcher* self);

    void (* stop)(CGUI_MessageDispatcher* self, bool force);
} CGUI_MessageDispatcher;

#define CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MIN 0
#define CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MAX 0

/* Constructor and Destructor */
CGUI_MessageDispatcher* cgui_createMessageDispatcher(HWND hwnd, UINT filterMin, UINT filterMax);

void cgui_destroyMessageDispatcher(CGUI_MessageDispatcher* dispatcher);

/* The default message dispatcher */
void* cgui_messageDispatcher_dispatch(void* ts);

void cgui_messageDispatcher_beginDispatch(CGUI_MessageDispatcher* self);

void cgui_messageDispatcher_beginDispatchAsync(CGUI_MessageDispatcher* self);

void cgui_messageDispatcher_stop(CGUI_MessageDispatcher* self, bool force);

typedef LRESULT CALLBACK (* CGUI_WindowProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/* Singleton of MessageHandler */
static CGUI_Singleton* cgui_messageHandlerSingleton = NULL;

/* Structure of MessageHandler */
typedef struct MessageHandler {
    LRESULT CALLBACK (* _winProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    CGUI_ApplicationMessageCallback applicationCallback;

    int (* routeToApplication)(CGUI_MessageHandler* self, CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam);

    CGUI_WindowProc (* getWindowProc)(CGUI_MessageHandler* handler);
} CGUI_MessageHandler;

/* Constructor and Destructor */
CGUI_MessageHandler* cgui_createMessageHandler(CGUI_ApplicationMessageCallback callback);

void cgui_destroyMessageHandler(CGUI_MessageHandler* handler);

/* The default message handler & router. */
LRESULT CALLBACK
cgui_messageHandler_winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/* Route the message to the application. */
int cgui_messageHandler_routeToApplication(CGUI_MessageHandler* self, CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam);

CGUI_WindowProc cgui_messageHandler_getWindowProc(CGUI_MessageHandler* handler);

#endif //CGUI_MESSAGE_H
