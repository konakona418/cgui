//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <stdlib.h>
#include "handler.h"

CGUI_EventHandler* cgui_createEventHandler() {
    CGUI_EventHandler* handler = (CGUI_EventHandler*) malloc(sizeof(CGUI_EventHandler));
    // todo: handle events.
    return handler;
}

void cgui_destroyEventHandler(CGUI_EventHandler* handler) {
    free(handler);
}
