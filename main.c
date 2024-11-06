#include <stdio.h>
#include "deps/util/common.h"
#include "deps/win32/window.h"

int main(void) {
    CGUI_WindowClassFactory* wndClassFactory = cgui_createWindowClassFactory();
    wndClassFactory->setWindowClassName(wndClassFactory, "CGUI_Window");
    wndClassFactory->setWindowInstance(wndClassFactory, GetModuleHandle(NULL));
    wndClassFactory->setWindowProc(wndClassFactory, DefWindowProc);
    CGUI_WindowClass* wndClass = wndClassFactory->createWindowClass(wndClassFactory);
    cgui_registerWindowClass(wndClass);
    cgui_destroyWindowClassFactory(wndClassFactory);

    CGUI_WindowFactory* wndFactory = cgui_createWindowFactory();
    wndFactory->setWindowClass(wndFactory, wndClass);
    wndFactory->setWindowName(wndFactory, "CGUI Window");
    wndFactory->setWindowStyle(wndFactory, WS_OVERLAPPEDWINDOW);
    wndFactory->setWindowSize(wndFactory, 640, 480);
    wndFactory->setWindowPosition(wndFactory, 100, 100);

    CGUI_Result(CGUI_Window) result = wndFactory->createWindow(wndFactory);
    if (cgui_is_err(result)) {
        panic(result.error->message);
    }
    cgui_destroyWindowFactory(wndFactory);

    CGUI_Window* window = cgui_unwrap(result);
    window->show(window);
    return 0;
}
