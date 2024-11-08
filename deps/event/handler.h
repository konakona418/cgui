//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_HANDLER_H
#define CGUI_HANDLER_H

typedef struct EventHandler CGUI_EventHandler;

typedef struct EventHandler {

} CGUI_EventHandler;

CGUI_EventHandler* cgui_createEventHandler();

void cgui_destroyEventHandler(CGUI_EventHandler* handler);

#endif //CGUI_HANDLER_H
