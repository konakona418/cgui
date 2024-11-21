//
// Created on: 2024/11/7, 21:18.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "application.h"

CGUI_Application* cgui_createApplication(CGUI_RuntimeContext* ctx) {
    if (CGUI_APP_INSTANCE != NULL) {
        return CGUI_APP_INSTANCE;
    }

    if (ctx == NULL) {
        panic("Fatal! Runtime context is null, unable to create application.");
    }
    CGUI_Application* app = (CGUI_Application*) malloc(sizeof(CGUI_Application));
    app->ctx = ctx;
    app->core = unwrap(cgui_createCoreFromContext(ctx));

    app->dispatcher = cgui_createMessageDispatcher(
            NULL, CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MIN,
            CGUI_DEFAULT_MESSAGE_DISPATCHER_FILTER_MAX);

    app->handler = cgui_createMessageHandler(cgui_application_messageCallback);

    app->run = cgui_application_run;
    app->stop = cgui_application_stop;

    CGUI_APP_INSTANCE = app;
    return app;
}

void cgui_destroyApplication(CGUI_Application* app) {
    if (app == NULL || CGUI_APP_INSTANCE == NULL) {
        panic("Fatal! Application is null, unable to destroy application.");
    }
    cgui_destroyMessageDispatcher(app->dispatcher);
    cgui_destroyCore(app->core);
    cgui_destroyRuntimeContext(app->ctx);
    free(app);

    CGUI_APP_INSTANCE = NULL;
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

CGUI_WindowProc cgui_application_getWindowProc(CGUI_Application* app) {
    return app->handler->getWindowProc(app->handler);
}

IterPredicateResult cgui_application_predicateHwnd(const char* key, void* value, void* target) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) value;
    if (impl(component->implFlag, CGUI_Trait_UIWin32)) {
        HWND hwnd = component->win32Impl->getWindowHandle(component);
        return hwnd == deref(HWND, target);
    }
    return false;
}

void cgui_application_messageCallback(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (CGUI_APP_INSTANCE == NULL) return;
    CGUI_Application* app = CGUI_APP_INSTANCE;
    CGUI_Result result = app->core->compManager->getComponentPredicate(app->core->compManager, &hwnd, cgui_application_predicateHwnd);
    if (is_ok(&result)) {
        CGUI_UIComponent* component = take(&result);
        // todo: this may lead to infinite recursive calls. proceed with caution.
        component->eventHandler->handleEvent(component->eventHandler, hwnd, msg, wParam, lParam, cgui_application_messageCallback);
    }
}

CGUI_Option cgui_getApplicationInstance() {
    if (CGUI_APP_INSTANCE != NULL) {
        return create_some(CGUI_APP_INSTANCE);
    } else {
        return create_none();
    }
}
