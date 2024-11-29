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

void cgui_application_run(bool isAsync) {
    CGUI_Application* app = CGUI_APP_INSTANCE;
    if (app == NULL) return;
    if (isAsync) {
        app->dispatcher->beginDispatchAsync(app->dispatcher);
    } else {
        app->dispatcher->beginDispatch(app->dispatcher);
    }
}

void cgui_application_stop() {
    CGUI_Application *app = CGUI_APP_INSTANCE;
    if (app == NULL) return;
    app->dispatcher->stop(app->dispatcher, true);
}

CGUI_WindowProc cgui_application_getWindowProc() {
    CGUI_Application* app = CGUI_APP_INSTANCE;
    if (app == NULL) {
        panic("Fatal! Application is null, unable to get window proc.");
    }
    return app->handler->getWindowProc(app->handler);
}

IterPredicateResult cgui_application_predicateInternalId(const char* key, void* value, void* target) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) value;
    if (impl(component->implFlag, CGUI_Trait_UIWin32)) {
        CGUI_InternalID internalId = deref(int, target);
        if (cgui_componentQueryIsIdLegal(internalId)) {
            return component->win32Impl->internalId == internalId;
        }
    }
    return false;
}

IterPredicateResult cgui_application_predicateHwnd(const char* key, void* value, void* target) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) value;
    if (impl(component->implFlag, CGUI_Trait_UIWin32)) {
        HWND hwnd = component->win32Impl->getWindowHandle(component);
        return hwnd == deref(HWND, target);
    }
    return false;
}

int cgui_application_messageCallback(CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (CGUI_APP_INSTANCE == NULL) return DefWindowProc(query.hwnd, msg, wParam, lParam);
    CGUI_Application* app = CGUI_APP_INSTANCE;
    CGUI_Result result = app->core->compManager->getComponentPredicate(app->core->compManager, &query.hwnd, cgui_application_predicateHwnd);
    if (is_ok(&result)) {
        CGUI_UIComponent* component = take(&result);
        query.component = component;
        // todo: this may lead to infinite recursive calls. proceed with caution.
        return component->eventHandler->handleEvent(component->eventHandler, query, msg, wParam, lParam, cgui_application_messageCallback);
    } else {
        return DefWindowProc(query.hwnd, msg, wParam, lParam);
    }
}

CGUI_Option cgui_tryGetApplicationInstance() {
    if (CGUI_APP_INSTANCE != NULL) {
        return create_some(CGUI_APP_INSTANCE);
    } else {
        return create_none();
    }
}
