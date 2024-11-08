//
// Created on: 2024/11/7, 14:54.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "core.h"

#include "../util/error.h"

CGUI_Result cgui_createCore(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) {
    CGUI_Result result = cgui_createRuntimeContext(hInstance, lpCmdLine, nCmdShow);
    if (is_err(&result)) {
        return result;
    }
    return cgui_createCoreFromContext(take(&result));
}

CGUI_Result cgui_createCoreFromContext(CGUI_RuntimeContext* ctx) {
    CGUI_Core* core = (CGUI_Core*) malloc(sizeof(CGUI_Core));
    if (ctx == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }

    core->ctx = ctx;

    core->wndFactory = cgui_createWindowFactory();
    core->wndClassFactory = cgui_createWindowClassFactory();

    core->wndManager = cgui_createWindowManager(ctx->hInstance);
    core->wndClassManager = cgui_createWindowClassManager();

    return create_ok(core);
}

void cgui_destroyCore(CGUI_Core* core) {
    cgui_destroyWindowManager(core->wndManager);
    cgui_destroyWindowClassManager(core->wndClassManager);
    cgui_destroyWindowFactory(core->wndFactory);
    cgui_destroyWindowClassFactory(core->wndClassFactory);

    // todo: use some other struct to store the context.
    // for instance, Application.
    // make sure the context can survive the whole application.
    cgui_destroyRuntimeContext(core->ctx);
    free(core);
}

CGUI_Result cgui_createRuntimeContext(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow) {
    CGUI_RuntimeContext* ctx = (CGUI_RuntimeContext*) malloc(sizeof(CGUI_RuntimeContext));
    if (ctx == NULL) {
        return create_err(CGUI_Error_MemAllocFailed());
    }
    ctx->hInstance = hInstance;
    ctx->lpCmdLine = lpCmdLine;
    ctx->nCmdShow = nCmdShow;
    return create_ok(ctx);
}

void cgui_destroyRuntimeContext(CGUI_RuntimeContext* ctx) {
    free(ctx);
}