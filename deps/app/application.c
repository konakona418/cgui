//
// Created on: 2024/11/7, 21:18.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "application.h"

CGUI_Application* cgui_createApplication(CGUI_RuntimeContext* ctx) {
    if (ctx == NULL) {
        panic("Fatal! Runtime context is null, unable to create application.");
    }
    CGUI_Application* app = (CGUI_Application*) malloc(sizeof(CGUI_Application));
    app->ctx = ctx;
    app->core = unwrap(cgui_createCoreFromContext(ctx));

    app->dispatcher = cgui_createMessageDispatcher(
            NULL, CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MIN,
            CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MAX);

    app->run = cgui_application_run;
    app->stop = cgui_application_stop;
    return app;
}

void cgui_destroyApplication(CGUI_Application* app) {
    if (app == NULL) {
        panic("Fatal! Application is null, unable to destroy application.");
    }
    cgui_destroyMessageDispatcher(app->dispatcher);
    cgui_destroyCore(app->core);
    cgui_destroyRuntimeContext(app->ctx);
    free(app);
}

void cgui_application_run(CGUI_Application* app, bool isAsync) {
    if (app == NULL) return;
    if (isAsync) {
        app->dispatcher->beginDispatchAsync(app->dispatcher);
    } else {
        app->dispatcher->beginDispatch(app->dispatcher);
    }
}

void cgui_application_stop(CGUI_Application* app) {
    if (app == NULL) return;
    app->dispatcher->stop(app->dispatcher, true);
}
