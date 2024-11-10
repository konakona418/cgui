#include <stdio.h>
#include "deps/util/common.h"
#include "deps/util/hashmap.h"
#include "deps/util/vector.h"
#include "deps/win32/window.h"
#include "deps/app/core.h"

// todo: fix core.c: L42

int main(void) {
    CGUI_RuntimeContext* ctx = unwrap(cgui_createRuntimeContext(GetModuleHandle(NULL), GetCommandLine(), SW_SHOW));
    CGUI_Core* core = unwrap(cgui_createCoreFromContext(ctx));
    core->wndClassFactory->setWindowClassStyle(core->wndClassFactory, CS_HREDRAW | CS_VREDRAW);
    core->wndClassFactory->setWindowClassName(core->wndClassFactory, "CGUI_Window");
    core->wndClassFactory->setWindowInstance(core->wndClassFactory, GetModuleHandle(NULL));
    core->wndClassFactory->setWindowProc(core->wndClassFactory, DefWindowProc);
    core->wndClassFactory->setWindowBackgroundBrush(core->wndClassFactory, (HBRUSH)(COLOR_WINDOW + 1));
    CGUI_WindowClass* wndClass = unwrap(core->wndClassFactory->createWindowClass(core->wndClassFactory));

    core->wndClassManager->addWindowClassAndRegister(core->wndClassManager, wndClass);

    core->wndFactory->setWindowClass(core->wndFactory, wndClass);
    core->wndFactory->setWindowName(core->wndFactory, "CGUI Window");
    core->wndFactory->setWindowStyle(core->wndFactory, WS_OVERLAPPEDWINDOW);
    core->wndFactory->setWindowSize(core->wndFactory, 640, 480);
    core->wndFactory->setWindowPosition(core->wndFactory, 100, 100);
    CGUI_Window* wnd = unwrap(core->wndFactory->createWindow(core->wndFactory));

    core->wndManager->addWindow(core->wndManager, wnd);
    wnd->show(wnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyWindow(wnd->hwnd);

    cgui_destroyCore(core);
    return 0;
}
