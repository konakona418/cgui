//
// Created on: 2024/11/7, 19:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "message.h"

CGUI_DispatcherThreadStart* cgui_createDispatcherThreadStart(CGUI_MessageDispatcher* dispatcher) {
    CGUI_DispatcherThreadStart* threadStart = (CGUI_DispatcherThreadStart*) malloc(sizeof(CGUI_DispatcherThreadStart));
    threadStart->dispatcher = dispatcher;
    threadStart->hwnd = dispatcher->hwnd;
    threadStart->filterMax = dispatcher->filterMax;
    threadStart->filterMin = dispatcher->filterMin;

    return threadStart;
}

void cgui_destroyDispatcherThreadStart(CGUI_DispatcherThreadStart* threadStart) {
    free(threadStart);
}

CGUI_MessageDispatcher* cgui_createMessageDispatcher(HWND hwnd, UINT filterMin, UINT filterMax) {
    CGUI_MessageDispatcher* dispatcher = (CGUI_MessageDispatcher*) malloc(sizeof(CGUI_MessageDispatcher));

    dispatcher->hwnd = hwnd;
    dispatcher->filterMin = filterMin;
    dispatcher->filterMax = filterMax;

    dispatcher->isDispatching = false;
    dispatcher->thread = NULL;

    dispatcher->_dispatch = cgui_messageDispatcher_dispatch;
    dispatcher->beginDispatch = cgui_messageDispatcher_beginDispatch;
    dispatcher->beginDispatchAsync = cgui_messageDispatcher_beginDispatchAsync;
    dispatcher->stop = cgui_messageDispatcher_stop;

    return dispatcher;
}

void cgui_destroyMessageDispatcher(CGUI_MessageDispatcher* dispatcher) {
    free(dispatcher);
}

void* cgui_messageDispatcher_dispatch(void* ts) {
    CGUI_DispatcherThreadStart* threadStart = (CGUI_DispatcherThreadStart*) ts;
    threadStart->dispatcher->isDispatching = true;
    MSG msg = {};
    while (GetMessage(&msg, threadStart->hwnd, threadStart->filterMin, threadStart->filterMax) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if (!threadStart->dispatcher->isDispatching) {
            break;
        }
    }
    threadStart->dispatcher->isDispatching = false;

    cgui_destroyDispatcherThreadStart(threadStart);
    return NULL;
}

void cgui_messageDispatcher_beginDispatch(CGUI_MessageDispatcher* self) {
    CGUI_DispatcherThreadStart* threadStart = cgui_createDispatcherThreadStart(self);
    self->_dispatch(threadStart);
}

void cgui_messageDispatcher_beginDispatchAsync(CGUI_MessageDispatcher* self) {
    CGUI_DispatcherThreadStart* threadStart = cgui_createDispatcherThreadStart(self);

    CGUI_Thread* thread = cgui_createThread(cgui_messageDispatcher_dispatch, threadStart);
    self->thread = thread;
    self->isDispatching = true;
}

void cgui_messageDispatcher_stop(CGUI_MessageDispatcher* self, bool force) {
    if (self->thread && self->isDispatching) {
        if (force) {
            cgui_thread_forceKill(self->thread);
            cgui_destroyThread(self->thread);
        } else {
            cgui_thread_joinThread(self->thread);
            cgui_destroyThread(self->thread);
        }
    }
    self->isDispatching = false;
}

void cgui_messageHandler_initSingleton(CGUI_ApplicationMessageCallback callback) {
    CGUI_MessageHandler* handler = (CGUI_MessageHandler*) malloc(sizeof(CGUI_MessageHandler));

    handler->_winProc = cgui_messageHandler_winProc;
    handler->applicationCallback = callback;

    handler->routeToApplication = cgui_messageHandler_routeToApplication;
    handler->getWindowProc = cgui_messageHandler_getWindowProc;

    if (cgui_messageHandlerSingleton == NULL) {
        panic("Fatal! Message handler singleton is not initialized.");
    }

    cgui_initSingleton(cgui_messageHandlerSingleton, handler);
}

CGUI_MessageHandler* cgui_getMessageHandlerInstance() {
    if (cgui_messageHandlerSingleton == NULL) {
        return NULL;
    } else {
        if (!cgui_isSingletonInitialized(cgui_messageHandlerSingleton)) {
            return NULL;
        }
    }
    return into(CGUI_MessageHandler, cgui_getSingletonValue(cgui_messageHandlerSingleton));
}

CGUI_MessageHandler* cgui_createMessageHandler(CGUI_ApplicationMessageCallback callback) {
    if (cgui_messageHandlerSingleton == NULL) {
        cgui_messageHandlerSingleton = cgui_createSingleton();
        cgui_messageHandler_initSingleton(callback);
    } else {
        if (!cgui_isSingletonInitialized(cgui_messageHandlerSingleton)) {
            cgui_messageHandler_initSingleton(callback);
        }
    }
    return cgui_getMessageHandlerInstance();
}

void cgui_destroyMessageHandler(CGUI_MessageHandler* handler) {
    free(handler);
}

LRESULT CALLBACK
cgui_messageHandler_winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    CGUI_MessageHandler* instance = cgui_getMessageHandlerInstance();
    if (instance == NULL) {
        printf("Message handler instance is not initialized. Use DefWindowProc instead.");
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    CGUI_ComponentQuery query = cgui_createComponentQuery(hwnd, CGUI_COMPONENT_QUERY_NO_ID);
    return instance->routeToApplication(instance, query, msg, wParam, lParam);
}

long long int
cgui_messageHandler_routeToApplication(CGUI_MessageHandler* self, CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (self->applicationCallback == NULL) {
        panic("Fatal! Application message callback is not set.");
    }
    return self->applicationCallback(query, msg, wParam, lParam);
}

CGUI_WindowProc cgui_messageHandler_getWindowProc(CGUI_MessageHandler* handler) {
    return handler->_winProc;
}
