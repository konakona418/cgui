//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <stdlib.h>
#include "handler.h"

CGUI_EventHandler* cgui_createEventHandler() {
    CGUI_EventHandler* handler = (CGUI_EventHandler*) malloc(sizeof(CGUI_EventHandler));
    handler->handleEvent = cgui_eventHandler_handleEvent;
    return handler;
}

void cgui_destroyEventHandler(CGUI_EventHandler* handler) {
    free(handler);
}

void cgui_eventHandler_handleEvent(CGUI_EventHandler* self, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    panic("Not implemented yet.");
    // todo: handle events.
}
