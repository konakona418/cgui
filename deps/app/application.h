//
// Created on: 2024/11/7, 21:18.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_APPLICATION_H
#define CGUI_APPLICATION_H

#include "core.h"
#include "../win32/message.h"
#include "../util/misc.h"

typedef struct Application CGUI_Application;

typedef struct Application {
    CGUI_RuntimeContext* ctx;

    CGUI_Core* core;
    CGUI_MessageDispatcher* dispatcher;
    CGUI_MessageHandler* handler;

    void (* run)(bool isAsync);
    void (* stop)(void);
} CGUI_Application;

#define app_ctor(_ctx) cgui_createApplication(_ctx)
#define app_dtor() cgui_destroyApplication()

CGUI_Application* cgui_createApplication(CGUI_RuntimeContext* ctx);
void cgui_destroyApplication();

void cgui_application_run(bool isAsync);
void cgui_application_stop(void);

CGUI_WindowProc cgui_application_getWindowProc();
long long int cgui_application_messageCallback(CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam);

CGUI_Option cgui_tryGetApplicationInstance();

static CGUI_Application* CGUI_APP_INSTANCE = NULL;

#endif //CGUI_APPLICATION_H
