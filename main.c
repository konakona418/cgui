#include <stdio.h>
#include "deps/util/common.h"
#include "deps/util/hashmap.h"
#include "deps/util/vector.h"
#include "deps/win32/window.h"
#include "deps/app/core.h"
#include "deps/app/application.h"
#include "deps/component/ui_factory.h"
#include "deps/component/ui_window.h"

// todo: fix core.c: L42

void onClose(CGUI_EventArgs args) {
    CGUI_UINativeWindow* wnd = into(CGUI_UIWindow, ((CGUI_UIComponent*) args.component)->disposableImpl->upperLevel);
    wnd->close(wnd);
    wnd->destroy(wnd);
    cgui_application_stop(into(CGUI_Application, unwrap_option(cgui_getApplicationInstance())));
}

void onClick(CGUI_MouseEventArgs args) {
    printf("Clicked @ (%ld,%ld)!\n", args.point.x, args.point.y);
}

int main(void) {
    CGUI_RuntimeContext* ctx = unwrap(cgui_createRuntimeContext(GetModuleHandle(NULL), GetCommandLine(), SW_SHOW));
    CGUI_Application* app = cgui_createApplication(ctx);
    CGUI_UIFactoryCluster* uiFactory = cgui_createUIFactoryCluster();

    CGUI_WindowClassOptions options = {
            .className = "CGUI_Window",
            .cursor = LoadCursor(NULL, IDC_ARROW),
            .geometry = {
                    .x = 100,
                    .y = 100,
                    .width = 640,
                    .height = 480
            },
            .icon = LoadIcon(NULL, IDI_APPLICATION),
            .menuName = "test",
            .title = "CGUI Window",
    };
    CGUI_UINativeWindow* wnd = unwrap(uiFactory->createComponent(uiFactory, "Window", 1, &options));
    wnd->show(wnd);
    CGUI_WindowHandler* wndHandler = into(CGUI_WindowHandler, wnd->component->eventHandler->localHandler);
    wndHandler->onClose = onClose;
    wndHandler->onMouseUp = onClick;
    app->run(app, false);


    /*CGUI_Core* core = unwrap(cgui_createCoreFromContext(ctx));
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
    HWND hwnd = wnd->hwnd;
    wnd = unwrap_option(core->wndManager->getWindowByHwnd(core->wndManager, hwnd));
    wnd->show(wnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    cgui_destroyWindowInstance(wnd);

    cgui_destroyCore(core);*/
    return 0;
}
