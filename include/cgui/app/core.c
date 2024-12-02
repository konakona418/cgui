//
// Created on: 2024/11/7, 14:54.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "core.h"

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

    core->wndFactory = cgui_createWindowFactory();
    core->wndClassFactory = cgui_createWindowClassFactory();

    core->wndManager = cgui_createWindowManager(ctx->hInstance);
    core->wndClassManager = cgui_createWindowClassManager();

    core->compManager = cgui_createComponentManager();

    return create_ok(core);
}

void cgui_destroyCore(CGUI_Core* core) {
    cgui_destroyWindowManager(core->wndManager);
    dbg_printf("WindowManager destroyed.\n");
    cgui_destroyWindowClassManager(core->wndClassManager);
    dbg_printf("WindowClassManager destroyed.\n");
    cgui_destroyWindowFactory(core->wndFactory);
    dbg_printf("WindowFactory destroyed.\n");
    cgui_destroyWindowClassFactory(core->wndClassFactory);
    dbg_printf("WindowClassFactory destroyed.\n");

    cgui_destroyComponentManager(core->compManager);
    dbg_printf("ComponentManager destroyed, components released.\n");

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