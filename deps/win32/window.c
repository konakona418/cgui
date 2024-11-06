//
// Created on: 2024/11/5, 23:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "window.h"
#include "../util/error.h"

CGUI_WindowFactory* cgui_createWindowFactory() {
    CGUI_WindowFactory* factory = (struct WindowFactory*) malloc(sizeof(struct WindowFactory));

    factory->dwExStyle = 0;
    factory->dwStyle = WS_OVERLAPPEDWINDOW;

    factory->hInstance = NULL;
    factory->lpParam = NULL;
    factory->menu = NULL;
    factory->parent = NULL;

    factory->lpClassName = NULL;
    factory->lpWindowName = NULL;

    factory->x = CW_USEDEFAULT;
    factory->y = CW_USEDEFAULT;
    factory->width = CW_USEDEFAULT;
    factory->height = CW_USEDEFAULT;


    factory->createWindow = cgui_windowFactory_createWindow;
    factory->setWindowExStyle = cgui_windowFactory_setWindowExStyle;
    factory->setWindowStyle = cgui_windowFactory_setWindowStyle;
    factory->setWindowClass = cgui_windowFactory_setWindowClass;
    factory->setWindowClassName = cgui_windowFactory_setWindowClassName;
    factory->setWindowGeometry = cgui_windowFactory_setWindowGeometry;
    factory->setWindowGeometryRect = cgui_windowFactory_setWindowGeometryRect;
    factory->setWindowInstance = cgui_windowFactory_setWindowInstance;
    factory->setWindowMenu = cgui_windowFactory_setWindowMenu;
    factory->setWindowParam = cgui_windowFactory_setWindowParam;
    factory->setWindowParent = cgui_windowFactory_setWindowParent;
    factory->setWindowPosition = cgui_windowFactory_setWindowPosition;
    factory->setWindowSize = cgui_windowFactory_setWindowSize;
    factory->setWindowName = cgui_windowFactory_setWindowName;
    return factory;
}

CGUI_Result cgui_windowFactory_createWindow(CGUI_WindowFactory* factory) {
    HWND hwnd = CreateWindowEx(
            factory->dwExStyle,
            factory->lpClassName,
            factory->lpWindowName,
            factory->dwStyle,
            factory->x,
            factory->y,
            factory->width,
            factory->height,
            factory->parent,
            factory->menu,
            factory->hInstance,
            factory->lpParam
    );

    CGUI_Window* window = cgui_createWindow(hwnd);
    if (hwnd == NULL) {
        free(window);
        return create_err(CGUI_Error_WindowCreateFailed());
    } else {
        return create_ok(window);
    }
}

void cgui_windowFactory_setWindowExStyle(CGUI_WindowFactory* factory, DWORD dwExStyle) {
    factory->dwExStyle = dwExStyle;
}

void cgui_windowFactory_setWindowStyle(CGUI_WindowFactory* factory, DWORD dwStyle) {
    factory->dwStyle = dwStyle;
}

void cgui_windowFactory_setWindowClass(CGUI_WindowFactory* factory, CGUI_WindowClass* wndClass) {
    factory->lpClassName = wndClass->wc.lpszClassName;
    factory->hInstance = wndClass->wc.hInstance;
}

void cgui_windowFactory_setWindowClassName(CGUI_WindowFactory* factory, LPCSTR windowClassName) {
    factory->lpClassName = windowClassName;
}

void cgui_windowFactory_setWindowGeometry(CGUI_WindowFactory* factory, int x, int y, int width, int height) {
    factory->x = x;
    factory->y = y;
    factory->width = width;
    factory->height = height;
}

void cgui_windowFactory_setWindowGeometryRect(CGUI_WindowFactory* factory, struct Rectangle* rect) {
    cgui_windowFactory_setWindowGeometry(factory, rect->x, rect->y, rect->width, rect->height);
}

void cgui_windowFactory_setWindowInstance(CGUI_WindowFactory* factory, HINSTANCE hInstance) {
    factory->hInstance = hInstance;
}

void cgui_windowFactory_setWindowMenu(CGUI_WindowFactory* factory, HMENU menu) {
    factory->menu = menu;
}

void cgui_windowFactory_setWindowParam(CGUI_WindowFactory* factory, LPVOID lpParam) {
    factory->lpParam = lpParam;
}

void cgui_windowFactory_setWindowParent(CGUI_WindowFactory* factory, HWND parent) {
    factory->parent = parent;
}

void cgui_windowFactory_setWindowPosition(CGUI_WindowFactory* factory, int x, int y) {
    factory->x = x;
    factory->y = y;
}

void cgui_windowFactory_setWindowSize(CGUI_WindowFactory* factory, int width, int height) {
    factory->width = width;
    factory->height = height;
}

void cgui_windowFactory_setWindowName(CGUI_WindowFactory* factory, LPCSTR windowName) {
    factory->lpWindowName = windowName;
}

void cgui_destroyWindowFactory(struct WindowFactory* factory) {
    free(factory);
}

CGUI_Window* cgui_createWindow(HWND hwnd) {
    CGUI_Window* window = (CGUI_Window*) malloc(sizeof(CGUI_Window));

    window->hwnd = hwnd;
    window->swState = SW_HIDE;

    // todo: load function pointers.

    window->show = cgui_window_show;
    window->hide = cgui_window_hide;
    window->setState = cgui_window_setState;

    return window;
}

CGUI_Result cgui_destroyWindow(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    DestroyWindow(self->hwnd);
    free(self);
    return create_ok(NULL);
}

CGUI_Result cgui_window_show(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->swState = SW_SHOW;
    ShowWindow(self->hwnd, SW_SHOW);
    return create_ok(NULL);
}

CGUI_Result cgui_window_hide(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->swState = SW_HIDE;
    ShowWindow(self->hwnd, SW_HIDE);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setState(CGUI_Window* self, int swState) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    ShowWindow(self->hwnd, swState);
    return create_ok(NULL);
}

CGUI_WindowClassFactory* cgui_createWindowClassFactory() {
    CGUI_WindowClassFactory* factory = (CGUI_WindowClassFactory*) malloc(sizeof(CGUI_WindowClassFactory));
    WNDCLASS w = { };
    factory->wc = w;

    factory->setWindowClassStyle = cgui_windowClassFactory_setWindowClassStyle;
    factory->setWindowProc = cgui_windowClassFactory_setWindowProc;
    factory->setWindowClassExtraSize = cgui_windowClassFactory_setWindowClassExtraSize;
    factory->setWindowExtraSize = cgui_windowClassFactory_setWindowExtraSize;
    factory->setWindowInstance = cgui_windowClassFactory_setWindowInstance;
    factory->setWindowIcon = cgui_windowClassFactory_setWindowIcon;
    factory->setWindowCursor = cgui_windowClassFactory_setWindowCursor;
    factory->setWindowBackgroundBrush = cgui_windowClassFactory_setWindowBackgroundBrush;
    factory->setWindowMenuName = cgui_windowClassFactory_setWindowMenuName;
    factory->setWindowClassName = cgui_windowClassFactory_setWindowClassName;
    factory->createWindowClass = cgui_windowClassFactory_createWindowClass;

    return factory;
}

void cgui_destroyWindowClassFactory(CGUI_WindowClassFactory* factory) {
    free(factory);
}

void cgui_windowClassFactory_setWindowClassStyle(CGUI_WindowClassFactory* factory, UINT style) {
    factory->wc.style = style;
}

void cgui_windowClassFactory_setWindowProc(CGUI_WindowClassFactory* factory, WNDPROC lpfnWndProc) {
    factory->wc.lpfnWndProc = lpfnWndProc;
}

void cgui_windowClassFactory_setWindowClassExtraSize(CGUI_WindowClassFactory* factory, int cbClsExtra) {
    factory->wc.cbClsExtra = cbClsExtra;
}

void cgui_windowClassFactory_setWindowExtraSize(CGUI_WindowClassFactory* factory, int cbWndExtra) {
    factory->wc.cbWndExtra = cbWndExtra;
}

void cgui_windowClassFactory_setWindowInstance(CGUI_WindowClassFactory* factory, HINSTANCE hInstance) {
    factory->wc.hInstance = hInstance;
}

void cgui_windowClassFactory_setWindowIcon(CGUI_WindowClassFactory* factory, HICON hIcon) {
    factory->wc.hIcon = hIcon;
}

void cgui_windowClassFactory_setWindowCursor(CGUI_WindowClassFactory* factory, HCURSOR hCursor) {
    factory->wc.hCursor = hCursor;
}

void cgui_windowClassFactory_setWindowBackgroundBrush(CGUI_WindowClassFactory* factory, HBRUSH hbrBackground) {
    factory->wc.hbrBackground = hbrBackground;
}

void cgui_windowClassFactory_setWindowMenuName(CGUI_WindowClassFactory* factory, LPCSTR lpszMenuName) {
    factory->wc.lpszMenuName = lpszMenuName;
}

void cgui_windowClassFactory_setWindowClassName(CGUI_WindowClassFactory* factory, LPCSTR lpszClassName) {
    factory->wc.lpszClassName = lpszClassName;
}

CGUI_WindowClass* cgui_windowClassFactory_createWindowClass(CGUI_WindowClassFactory* factory) {
    CGUI_WindowClass* wc = (CGUI_WindowClass*) malloc(sizeof(CGUI_WindowClass));
    wc->wc = factory->wc;
    return wc;
}

CGUI_Result cgui_registerWindowClass(CGUI_WindowClass* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    ATOM atom = RegisterClass(&self->wc);
    if (atom == 0) {
        return create_err(CGUI_Error_WindowClassRegistrationFailed());
    } else {
        return create_ok(NULL);
    }
}

CGUI_Result cgui_destroyWindowClass(CGUI_WindowClass* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    UnregisterClass(self->wc.lpszClassName, self->wc.hInstance);
    free(self);
    return create_ok(NULL);
}
