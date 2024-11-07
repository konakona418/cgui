#include <stdio.h>
#include "deps/util/common.h"
#include "deps/util/hashmap.h"
#include "deps/win32/window.h"
#include "deps/core.h"

int main(void) {
    CGUI_RuntimeContext* ctx = unwrap(cgui_createRuntimeContext(GetModuleHandle(NULL), GetCommandLine(), SW_SHOW));
    CGUI_Core* core = unwrap(cgui_createCoreFromContext(ctx));
    core->wndClassFactory->setWindowClassName(core->wndClassFactory, "CGUI_Window");
    core->wndClassFactory->setWindowInstance(core->wndClassFactory, GetModuleHandle(NULL));
    core->wndClassFactory->setWindowProc(core->wndClassFactory, DefWindowProc);
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

    cgui_destroyCore(core);
    return 0;
    CGUI_WindowFactory* wndFactory = cgui_createWindowFactory();
    wndFactory->setWindowClass(wndFactory, wndClass);
    wndFactory->setWindowName(wndFactory, "CGUI Window");
    wndFactory->setWindowStyle(wndFactory, WS_OVERLAPPEDWINDOW);
    wndFactory->setWindowSize(wndFactory, 640, 480);
    wndFactory->setWindowPosition(wndFactory, 100, 100);

    CGUI_Result result = wndFactory->createWindow(wndFactory);
    if (is_err(&result)) {
        panic(result.error->message);
    }
    cgui_destroyWindowFactory(wndFactory);

    CGUI_Window* window = unwrap(result);
    printf("Window created: %s.\n", window->wndIdentifier);
    window->show(window);

    CGUI_WindowManager* wndManager = cgui_createWindowManager(GetModuleHandle(NULL));
    CGUI_WindowClassManager* wndClassManager = cgui_createWindowClassManager();
    wndManager->addWindow(wndManager, window);
    wndClassManager->addWindowClass(wndClassManager, wndClass);
    unwrap_option(wndManager->getWindow(wndManager, window->wndIdentifier));

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
