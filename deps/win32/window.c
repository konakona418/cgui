//
// Created on: 2024/11/5, 23:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "window.h"
#include "../util/error.h"

#define HASH_TABLE_BUCKET_SIZE 16

char* cgui_generateRandomWindowIdentifier() {
    char* identifier = (char*) malloc(sizeof(char) * 32);
    sprintf(identifier, "wnd_%d", random(10000, 65536));
    return identifier;
}

char* cgui_generateWindowIdentifier(LPCSTR windowClassname) {
    char* identifier = (char*) malloc(sizeof(char) * 32);
    sprintf(identifier, "wnd_%s_%d", windowClassname, random(10000, 65536));
    return identifier;
}

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

    factory->resetWindowFactory = cgui_resetWindowFactory;
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

    CGUI_Window* window = cgui_createWindow(hwnd, factory->lpWindowName, factory->lpClassName);
    if (hwnd == NULL) {
        free(window);
        factory->resetWindowFactory(factory);
        return create_err(CGUI_Error_WindowCreateFailed());
    } else {
        factory->resetWindowFactory(factory);
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

void cgui_resetWindowFactory(CGUI_WindowFactory* self) {
    self->dwExStyle = 0;
    self->dwStyle = WS_OVERLAPPEDWINDOW;

    self->hInstance = NULL;
    self->lpParam = NULL;
    self->menu = NULL;
    self->parent = NULL;

    self->lpClassName = NULL;
    self->lpWindowName = NULL;

    self->x = CW_USEDEFAULT;
    self->y = CW_USEDEFAULT;
    self->width = CW_USEDEFAULT;
    self->height = CW_USEDEFAULT;
}

CGUI_Window* cgui_createWindow(HWND hwnd, LPCSTR wndName, LPCSTR wndClassName) {
    CGUI_Window* window = (CGUI_Window*) malloc(sizeof(CGUI_Window));

    window->hwnd = hwnd;
    window->swState = SW_HIDE;
    window->wndName = wndName;
    window->wndClassName = wndClassName;

    window->wndIdentifier = cgui_generateWindowIdentifier(wndClassName);

    window->show = cgui_window_show;
    window->hide = cgui_window_hide;
    window->update = cgui_window_update;
    window->close = cgui_window_close;
    window->destroy = cgui_window_destroy;

    window->setEnabled = cgui_window_setEnabled;

    window->setState = cgui_window_setState;
    window->postMessage = cgui_window_postMessage;

    window->setWindowName = cgui_window_setWindowName;
    window->getWindowName = cgui_window_getWindowName;

    window->setWindowStyle = cgui_window_setWindowStyle;
    window->getWindowStyle = cgui_window_getWindowStyle;
    window->setWindowGeometry = cgui_window_setWindowGeometry;
    window->setWindowGeometryRect = cgui_window_setWindowGeometryRect;
    window->setWindowPosition = cgui_window_setWindowPosition;
    window->setWindowSize = cgui_window_setWindowSize;

    window->setWindowProperty = cgui_window_setWindowProperty;

    window->getWindowHandle = cgui_window_getWindowHandle;

    return window;
}

CGUI_Result cgui_destroyWindow(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    if (IsWindow(self->hwnd)) {
        DestroyWindow(self->hwnd);
    } else {
        // todo: this does not raise any error, but that's just a temporary compromise.
        // in case that the window was already destroyed by user.
    }
    free(self);
    return create_ok(NULL);
}

CGUI_Result cgui_destroyWindowInstance(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    if (IsWindow(self->hwnd)) {
        DestroyWindow(self->hwnd);
    }
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

CGUI_Result cgui_window_update(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    InvalidateRect(self->hwnd, NULL, TRUE);
    UpdateWindow(self->hwnd);
    return create_ok(NULL);
}

CGUI_Result cgui_window_close(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    CloseWindow(self->hwnd);
    return create_ok(NULL);
}

CGUI_Result cgui_window_destroy(CGUI_Window* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    DestroyWindow(self->hwnd);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setEnabled(CGUI_Window* self, bool enabled) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    EnableWindow(self->hwnd, enabled);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setState(CGUI_Window* self, int swState) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    ShowWindow(self->hwnd, swState);
    return create_ok(NULL);
}

CGUI_Result cgui_window_postMessage(CGUI_Window* self, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    PostMessage(self->hwnd, msg, wParam, lParam);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setWindowName(CGUI_Window* self, LPCSTR wndName) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    SetWindowText(self->hwnd, wndName);
    return create_ok(NULL);
}

LPCSTR cgui_window_getWindowName(CGUI_Window* self) {
    if (self == NULL) {
        return NULL;
    }
    int bufSize = GetWindowTextLength(self->hwnd) + 1;
    char* buf = (char*) malloc(bufSize);
    GetWindowText(self->hwnd, buf, bufSize);
    return buf;
}

CGUI_Result cgui_window_setWindowStyle(CGUI_Window* self, LONG_PTR dwStyle) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    SetWindowLongPtr(self->hwnd, GWL_STYLE, dwStyle);
    return create_ok(NULL);
}

LONG_PTR cgui_window_getWindowStyle(CGUI_Window* self) {
    if (self == NULL) {
        return 0;
    }
    return GetWindowLongPtr(self->hwnd, GWL_STYLE);
}

CGUI_Result cgui_window_setWindowGeometry(CGUI_Window* self, int x, int y, int width, int height) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    MoveWindow(self->hwnd, x, y, width, height, TRUE);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setWindowGeometryRect(CGUI_Window* self, CGUI_Rectangle* rect) {
    return cgui_window_setWindowGeometry(self, rect->x, rect->y, rect->width, rect->height);
}

CGUI_Result cgui_window_setWindowPosition(CGUI_Window* self, int x, int y) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    MoveWindow(self->hwnd, x, y, 0, 0, TRUE);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setWindowSize(CGUI_Window* self, int width, int height) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    MoveWindow(self->hwnd, 0, 0, width, height, TRUE);
    return create_ok(NULL);
}

CGUI_Result cgui_window_setWindowProperty(CGUI_Window* self, int gwlIndex, LONG_PTR gwlValue) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    SetWindowLongPtr(self->hwnd, gwlIndex, gwlValue);
    return create_ok(NULL);
}

HWND cgui_window_getWindowHandle(CGUI_Window* self) {
    if (self == NULL) {
        return NULL;
    }
    return self->hwnd;
}

CGUI_WindowClassFactory* cgui_createWindowClassFactory() {
    CGUI_WindowClassFactory* factory = (CGUI_WindowClassFactory*) malloc(sizeof(CGUI_WindowClassFactory));
    WNDCLASS w = {};
    factory->wc = w;

    factory->wc.style = 0;
    factory->wc.lpfnWndProc = NULL;
    factory->wc.cbClsExtra = 0;
    factory->wc.cbWndExtra = 0;
    factory->wc.hInstance = NULL;
    factory->wc.hIcon = NULL;
    factory->wc.hCursor = NULL;
    factory->wc.hbrBackground = NULL;
    factory->wc.lpszMenuName = NULL;
    factory->wc.lpszClassName = NULL;

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
    factory->resetWindowClassFactory = cgui_resetWindowClassFactory;

    return factory;
}

void cgui_destroyWindowClassFactory(CGUI_WindowClassFactory* factory) {
    free(factory);
}

void cgui_resetWindowClassFactory(CGUI_WindowClassFactory* self) {
    WNDCLASS w = {};
    self->wc = w;

    self->wc.style = 0;
    self->wc.lpfnWndProc = NULL;
    self->wc.cbClsExtra = 0;
    self->wc.cbWndExtra = 0;
    self->wc.hInstance = NULL;
    self->wc.hIcon = NULL;
    self->wc.hCursor = NULL;
    self->wc.hbrBackground = NULL;
    self->wc.lpszMenuName = NULL;
    self->wc.lpszClassName = NULL;
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

CGUI_Result cgui_windowClassFactory_createWindowClass(CGUI_WindowClassFactory* factory) {
    CGUI_WindowClass* wc = (CGUI_WindowClass*) malloc(sizeof(CGUI_WindowClass));
    wc->wc = factory->wc;
    factory->resetWindowClassFactory(factory);
    return create_ok(wc);
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

CGUI_WindowManager* cgui_createWindowManager(HINSTANCE hInstance) {
    CGUI_WindowManager* manager = (CGUI_WindowManager*) malloc(sizeof(CGUI_WindowManager));
    manager->hInstance = hInstance;
    manager->windows = create_hash_table(HASH_TABLE_BUCKET_SIZE);

    manager->addWindow = cgui_windowManager_addWindow;
    manager->getWindow = cgui_windowManager_getWindow;
    manager->getWindowByHwnd = cgui_windowManager_getWindowByHwnd;
    manager->removeWindow = cgui_windowManager_removeWindow;
    manager->removeWindowByIdentifier = cgui_windowManager_removeWindowByIdentifier;
    manager->destroyAllWindows = cgui_windowManager_destroyAllWindows;
    return manager;
}

void cgui_destroyWindowManager(CGUI_WindowManager* self) {
    unwrap(cgui_windowManager_destroyAllWindows(self));
    destroy_hash_table(self->windows);
    free(self);
}

CGUI_Result cgui_windowManager_addWindow(CGUI_WindowManager* self, CGUI_Window* window) {
    if (self == NULL || window == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->windows->insert(self->windows, window->wndIdentifier, window);
    return create_ok(NULL);
}

CGUI_Option cgui_windowManager_getWindow(CGUI_WindowManager* self, const char* wndIdentifier) {
    if (self == NULL || wndIdentifier == NULL) {
        return create_none();
    }
    CGUI_Window* wnd = self->windows->find(self->windows, wndIdentifier);
    if (wnd == NULL) {
        return create_none();
    }
    return create_some(wnd);
}

IterPredicateResult cgui_windowManager_predicateHwnd(const char* key, void* value, void* target) {
    CGUI_Window* window = (CGUI_Window*) value;
    return window->hwnd == deref(HWND, target);
}

CGUI_Option cgui_windowManager_getWindowByHwnd(CGUI_WindowManager* self, HWND hwnd) {
    if (self == NULL || hwnd == NULL) {
        return create_none();
    }
    CGUI_Window* wc = self->windows->find_if(self->windows, &hwnd, cgui_windowManager_predicateHwnd);
    if (wc == NULL) {
        return create_none();
    }
    return create_some(wc);
}

CGUI_Result cgui_windowManager_removeWindow(CGUI_WindowManager* self, CGUI_Window* window) {
    if (self == NULL || window == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    CGUI_Window* ptr = self->windows->remove(self->windows, window->wndIdentifier);
    cgui_destroyWindow(ptr);
    return create_ok(NULL);
}

CGUI_Result cgui_windowManager_removeWindowByIdentifier(CGUI_WindowManager* manager, const char* wndIdentifier) {
    if (manager == NULL || wndIdentifier == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    CGUI_Window* ptr = manager->windows->remove(manager->windows, wndIdentifier);
    cgui_destroyWindow(ptr);
    return create_ok(NULL);
}

CGUI_Result cgui_windowManager_destroyAllWindows(CGUI_WindowManager* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    return self->windows->iter_value_result(self->windows, (CGUI_Result (*)(void*)) cgui_destroyWindow);
}

CGUI_WindowClassManager* cgui_createWindowClassManager() {
    CGUI_WindowClassManager* manager = (CGUI_WindowClassManager*) malloc(sizeof(CGUI_WindowClassManager));
    manager->windowClasses = create_hash_table(HASH_TABLE_BUCKET_SIZE);

    manager->addWindowClass = cgui_windowClassManager_addWindowClass;
    manager->addWindowClassAndRegister = cgui_windowClassManager_addWindowClassAndRegister;
    manager->destroyAllWindowClasses = cgui_windowClassManager_destroyAllWindowClasses;
    manager->getWindowClass = cgui_windowClassManager_getWindowClass;
    manager->removeWindowClass = cgui_windowClassManager_removeWindowClass;
    manager->removeWindowClassByName = cgui_windowClassManager_removeWindowClassByName;
    return manager;
}

void cgui_destroyWindowClassManager(CGUI_WindowClassManager* self) {
    if (self == NULL) {
        return;
    }
    unwrap(self->destroyAllWindowClasses(self));
    destroy_hash_table(self->windowClasses);
    free(self);
}

CGUI_Result cgui_windowClassManager_addWindowClass(CGUI_WindowClassManager* self, CGUI_WindowClass* wc) {
    if (self == NULL || wc == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    if (self->windowClasses->contains(self->windowClasses, wc->wc.lpszClassName)) {
        return create_err(CGUI_Error_WindowClassAlreadyExist());
    }
    self->windowClasses->insert(self->windowClasses, wc->wc.lpszClassName, wc);
    return create_ok(NULL);
}

CGUI_Result cgui_windowClassManager_addWindowClassAndRegister(CGUI_WindowClassManager* self, CGUI_WindowClass* wc) {
    if (self == NULL || wc == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    CGUI_Result reg = cgui_registerWindowClass(wc);
    if (is_err(&reg)) {
        return reg;
    }
    CGUI_Result r = cgui_windowClassManager_addWindowClass(self, wc);
    return r;
}

CGUI_Option cgui_windowClassManager_getWindowClass(CGUI_WindowClassManager* self, const char* lpszClassName) {
    if (self == NULL || lpszClassName == NULL) {
        return create_none();
    }
    CGUI_WindowClass* wc = self->windowClasses->find(self->windowClasses, lpszClassName);
    if (wc == NULL) {
        return create_none();
    }
    return create_some(wc);
}

CGUI_Result cgui_windowClassManager_removeWindowClass(CGUI_WindowClassManager* self, CGUI_WindowClass* wc) {
    if (self == NULL || wc == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    CGUI_WindowClass* ptr = self->windowClasses->remove(self->windowClasses, wc->wc.lpszClassName);
    if (ptr == NULL) {
        return create_err(CGUI_Error_WindowClassNotFound());
    }
    cgui_destroyWindowClass(ptr);
    return create_ok(NULL);
}

CGUI_Result cgui_windowClassManager_removeWindowClassByName(CGUI_WindowClassManager* self, const char* lpszClassName) {
    if (self == NULL || lpszClassName == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    CGUI_WindowClass* ptr = self->windowClasses->remove(self->windowClasses, lpszClassName);
    if (ptr == NULL) {
        return create_err(CGUI_Error_WindowClassNotFound());
    }
    cgui_destroyWindowClass(ptr);
    return create_ok(NULL);
}

CGUI_Result cgui_windowClassManager_destroyAllWindowClasses(CGUI_WindowClassManager* self) {
    if (self == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    return self->windowClasses->iter_value_result(self->windowClasses,
                                                  (CGUI_Result (*)(void*)) cgui_destroyWindowClass);
}
