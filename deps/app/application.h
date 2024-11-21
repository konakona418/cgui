//
// Created on: 2024/11/7, 21:18.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_APPLICATION_H
#define CGUI_APPLICATION_H

#include "core.h"
#include "../win32/message.h"

typedef struct Application CGUI_Application;

typedef struct Application {
    CGUI_RuntimeContext* ctx;

    CGUI_Core* core;
    CGUI_MessageDispatcher* dispatcher;
    CGUI_MessageHandler* handler;

    void (* run)(CGUI_Application* app, bool isAsync);
    void (* stop)(CGUI_Application* app);
} CGUI_Application;

CGUI_Application* cgui_createApplication(CGUI_RuntimeContext* ctx);
void cgui_destroyApplication(CGUI_Application* app);

void cgui_application_run(CGUI_Application* app, bool isAsync);
void cgui_application_stop(CGUI_Application* app);

CGUI_WindowProc cgui_application_getWindowProc(CGUI_Application* app);
void cgui_application_messageCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

CGUI_Option cgui_getApplicationInstance();

static CGUI_Application* CGUI_APP_INSTANCE = NULL;

#endif //CGUI_APPLICATION_H
