//
// Created on: 2024/11/5, 23:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_WINDOW_H
#define CGUI_WINDOW_H

#include <windows.h>
#include <assert.h>
#include <time.h>

#include "../util/common.h"
#include "../util/hashmap.h"

#include "../geo/rect.h"

/* Prototypes of relevant classes. */
typedef struct WindowFactory CGUI_WindowFactory;
typedef struct Window CGUI_Window;

typedef struct WindowClassFactory CGUI_WindowClassFactory;
typedef struct WindowClass CGUI_WindowClass;

typedef struct WindowManager CGUI_WindowManager;
typedef struct WindowClassManager CGUI_WindowClassManager;

/* Generating a random window identifier.
 * The identifier takes the form of:
 * wnd_<randint(10000, 65536)> */
char* cgui_generateRandomWindowIdentifier();

/* Generates a window identifier.
 * The identifier takes the form of:
 * wnd_<windowClassName>_<randint(10000, 65536)> */
char* cgui_generateWindowIdentifier(LPCSTR windowClassname);

/* Structure of WindowFactory. */
typedef struct WindowFactory {
    DWORD dwStyle;
    DWORD dwExStyle;

    LPCSTR lpClassName;
    LPCSTR lpWindowName;

    int x, y, width, height;

    HWND parent;
    HMENU menu;
    HINSTANCE hInstance;
    LPVOID lpParam;

    void (* setWindowStyle)(CGUI_WindowFactory* self, DWORD dwStyle);

    void (* setWindowExStyle)(CGUI_WindowFactory* self, DWORD dwExStyle);

    void (* setWindowClass)(CGUI_WindowFactory* self, CGUI_WindowClass* wndClass);

    void (* setWindowClassName)(CGUI_WindowFactory* self, LPCSTR windowClassName);

    void (* setWindowName)(CGUI_WindowFactory* self, LPCSTR windowText);

    void (* setWindowSize)(CGUI_WindowFactory* self, int width, int height);

    void (* setWindowPosition)(CGUI_WindowFactory* self, int x, int y);

    void (* setWindowGeometry)(CGUI_WindowFactory* self, int x, int y, int width, int height);

    void (* setWindowGeometryRect)(CGUI_WindowFactory* self, CGUI_Rectangle* rect);

    void (* setWindowParent)(CGUI_WindowFactory* self, HWND parent);

    void (* setWindowMenu)(CGUI_WindowFactory* self, HMENU menu);

    void (* setWindowInstance)(CGUI_WindowFactory* self, HINSTANCE hInstance);

    void (* setWindowParam)(CGUI_WindowFactory* self, LPVOID lpParam);

    CGUI_Result (* createWindow)(CGUI_WindowFactory* self);

    void (* resetWindowFactory)(CGUI_WindowFactory* self);
} CGUI_WindowFactory;

/* Constructors and Destructors of WindowFactory. */

/* Creates a WindowFactory. */
CGUI_WindowFactory* cgui_createWindowFactory();

/* Destroys a WindowFactory. */
void cgui_destroyWindowFactory(struct WindowFactory* factory);

/* Resets the WindowFactory parameters to default. */
void cgui_resetWindowFactory(CGUI_WindowFactory* self);

/* Setters of WindowFactory. */

/* Sets the styleImpl of the window. */
void cgui_windowFactory_setWindowStyle(CGUI_WindowFactory* factory, DWORD dwStyle);

/* Sets the extended styleImpl of the window.
 * See https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles. */
void cgui_windowFactory_setWindowExStyle(CGUI_WindowFactory* factory, DWORD dwExStyle);

/* [Optional] Sets the window class of the window. */
void cgui_windowFactory_setWindowClass(CGUI_WindowFactory* factory, CGUI_WindowClass* wndClass);

/* [Optional] Sets the class name of the window. */
void cgui_windowFactory_setWindowClassName(CGUI_WindowFactory* factory, LPCSTR windowClassName);

/* [Optional] Sets the name of the window. */
void cgui_windowFactory_setWindowName(CGUI_WindowFactory* factory, LPCSTR windowName);

/* Sets the size of the window. */
void cgui_windowFactory_setWindowSize(CGUI_WindowFactory* factory, int width, int height);

/* Sets the position of the window. */
void cgui_windowFactory_setWindowPosition(CGUI_WindowFactory* factory, int x, int y);

/* Sets the geometry of the window. */
void cgui_windowFactory_setWindowGeometry(CGUI_WindowFactory* factory, int x, int y, int width, int height);

/* Sets the geometry of the window with a `Rectangle`. */
void cgui_windowFactory_setWindowGeometryRect(CGUI_WindowFactory* factory, CGUI_Rectangle* rect);

/* [Optional] Sets the parent of the window. */
void cgui_windowFactory_setWindowParent(CGUI_WindowFactory* factory, HWND parent);

/* [Optional] Sets the menu of the window. */
void cgui_windowFactory_setWindowMenu(CGUI_WindowFactory* factory, HMENU menu);

/* [Optional] Sets the instance of the window. */
void cgui_windowFactory_setWindowInstance(CGUI_WindowFactory* factory, HINSTANCE hInstance);

/* [Optional] Sets the param of the window. */
void cgui_windowFactory_setWindowParam(CGUI_WindowFactory* factory, LPVOID lpParam);

/* Creates a window with the given WindowFactory.
 * This will automatically reset the parameters of the WindowFactory. */
CGUI_Result cgui_windowFactory_createWindow(CGUI_WindowFactory* factory);


/* Structure of Window.
 * This type, if registered to the `CGUI_WindowManager`,
 * shall not be deallocated manually. */
typedef struct Window {
    char* wndIdentifier;

    HWND hwnd;
    LPCSTR wndName;
    LPCSTR wndClassName;
    int swState;

    // todo: implement other methods.

    CGUI_Result (* show)(CGUI_Window* self);

    CGUI_Result (* hide)(CGUI_Window* self);

    CGUI_Result (* close)(CGUI_Window* self);

    CGUI_Result (* setState)(CGUI_Window* self, int swState);

    CGUI_Result (* setEnabled)(CGUI_Window* self, bool enabled);

    CGUI_Result (* postMessage)(CGUI_Window* self, UINT msg, WPARAM wParam, LPARAM lParam);

    CGUI_Result (* setWindowName)(CGUI_Window* self, LPCSTR wndName);

    CGUI_Result (* setWindowStyle)(CGUI_Window* self, LONG_PTR dwStyle);

    CGUI_Result (* setWindowGeometry)(CGUI_Window* self, int x, int y, int width, int height);

    CGUI_Result (* setWindowGeometryRect)(CGUI_Window* self, CGUI_Rectangle* rect);

    CGUI_Result (* setWindowPosition)(CGUI_Window* self, int x, int y);

    CGUI_Result (* setWindowSize)(CGUI_Window* self, int width, int height);

    CGUI_Result (* setWindowProperty)(CGUI_Window* self, int gwlIndex, LONG_PTR gwlValue);

    HWND (* getWindowHandle)(CGUI_Window* self);
} CGUI_Window;

/* Constructors and Destructors of Window. */

/* Creates a Window struct.
 * Note that this function is not intended for Window creation. Use WindowFactory instead. */
CGUI_Window* cgui_createWindow(HWND hwnd, LPCSTR wndName, LPCSTR wndClassName);

/** Destroys a Window struct.
 * @warning this will destroy the window instance (win32) and the CGUI_Window struct.
 * @warning do not call this function if the window is registered a manager.
 */
CGUI_Result cgui_destroyWindow(CGUI_Window* self);

/**
 * Destroys the window instance.
 * @warning this only destroys the window instance (win32), but CGUI_Window is not destroyed.
 */
CGUI_Result cgui_destroyWindowInstance(CGUI_Window* self);

/* Methods of Window. */
CGUI_Result cgui_window_show(CGUI_Window* self);

/* Hides the window. */
CGUI_Result cgui_window_hide(CGUI_Window* self);

/* Closes the window. */
CGUI_Result cgui_window_close(CGUI_Window* self);

/* Sets the stateImpl of the window. e.g. SW_SHOW to show the window. */
CGUI_Result cgui_window_setState(CGUI_Window* self, int swState);

/* Enables or disables the window. */
CGUI_Result cgui_window_setEnabled(CGUI_Window* self, bool enabled);

/* Posts a message to the window. */
CGUI_Result cgui_window_postMessage(CGUI_Window* self, UINT msg, WPARAM wParam, LPARAM lParam);

/* Sets the name of the window. */
CGUI_Result cgui_window_setWindowName(CGUI_Window* self, LPCSTR wndName);

/* Sets the styleImpl of the window. */
CGUI_Result cgui_window_setWindowStyle(CGUI_Window* self, LONG_PTR dwStyle);

/* Sets the geometry of the window. */
CGUI_Result cgui_window_setWindowGeometry(CGUI_Window* self, int x, int y, int width, int height);

/* Sets the geometry of the window with a `Rectangle`. */
CGUI_Result cgui_window_setWindowGeometryRect(CGUI_Window* self, CGUI_Rectangle* rect);

/* Sets the position of the window. */
CGUI_Result cgui_window_setWindowPosition(CGUI_Window* self, int x, int y);

/* Sets the size of the window. */
CGUI_Result cgui_window_setWindowSize(CGUI_Window* self, int width, int height);

/* Sets the property of the window.
 * See: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlongptra.*/
CGUI_Result cgui_window_setWindowProperty(CGUI_Window* self, int gwlIndex, LONG_PTR gwlValue);

/* Gets the handle of the window. */
HWND cgui_window_getWindowHandle(CGUI_Window* self);


/* Structure of WindowClassFactory.
 * It is used to create a WindowClass. */
typedef struct WindowClassFactory {
    WNDCLASS wc;

    void (* setWindowClassStyle)(CGUI_WindowClassFactory* self, UINT style);

    void (* setWindowProc)(CGUI_WindowClassFactory* self, WNDPROC lpfnWndProc);

    void (* setWindowClassExtraSize)(CGUI_WindowClassFactory* self, int cbClsExtra);

    void (* setWindowExtraSize)(CGUI_WindowClassFactory* self, int cbWndExtra);

    void (* setWindowInstance)(CGUI_WindowClassFactory* self, HINSTANCE hInstance);

    void (* setWindowIcon)(CGUI_WindowClassFactory* self, HICON hIcon);

    void (* setWindowCursor)(CGUI_WindowClassFactory* self, HCURSOR hCursor);

    void (* setWindowBackgroundBrush)(CGUI_WindowClassFactory* self, HBRUSH hbrBackground);

    void (* setWindowMenuName)(CGUI_WindowClassFactory* self, LPCSTR lpszMenuName);

    void (* setWindowClassName)(CGUI_WindowClassFactory* self, LPCSTR lpszClassName);

    CGUI_Result (* createWindowClass)(CGUI_WindowClassFactory* self);

    void (* resetWindowClassFactory)(CGUI_WindowClassFactory* self);
} CGUI_WindowClassFactory;

/* Constructors and Destructors of WindowClassFactory. */
CGUI_WindowClassFactory* cgui_createWindowClassFactory();

void cgui_destroyWindowClassFactory(CGUI_WindowClassFactory* factory);

void cgui_resetWindowClassFactory(CGUI_WindowClassFactory* self);

/* Setters of WindowClassFactory. */

/* Sets the styleImpl of the window class.
 * See https://docs.microsoft.com/en-us/windows/win32/winmsg/window-class-styles */
void cgui_windowClassFactory_setWindowClassStyle(CGUI_WindowClassFactory* factory, UINT style);

/* Sets the window procedure of the window class. */
void cgui_windowClassFactory_setWindowProc(CGUI_WindowClassFactory* factory, WNDPROC lpfnWndProc);

/* Sets the extra size of the window class. */
void cgui_windowClassFactory_setWindowClassExtraSize(CGUI_WindowClassFactory* factory, int cbClsExtra);

/* Sets the extra size of the window. */
void cgui_windowClassFactory_setWindowExtraSize(CGUI_WindowClassFactory* factory, int cbWndExtra);

/* Sets the instance of the window class.
 * Usually, this instance is passed into the program entry point function.*/
void cgui_windowClassFactory_setWindowInstance(CGUI_WindowClassFactory* factory, HINSTANCE hInstance);

/* Sets the icon of the window class. */
void cgui_windowClassFactory_setWindowIcon(CGUI_WindowClassFactory* factory, HICON hIcon);

/* Sets the cursor of the window class. */
void cgui_windowClassFactory_setWindowCursor(CGUI_WindowClassFactory* factory, HCURSOR hCursor);

/* Sets the background brush of the window class. */
void cgui_windowClassFactory_setWindowBackgroundBrush(CGUI_WindowClassFactory* factory, HBRUSH hbrBackground);

/* Sets the menu name of the window class. */
void cgui_windowClassFactory_setWindowMenuName(CGUI_WindowClassFactory* factory, LPCSTR lpszMenuName);

/* Sets the class name of the window class. */
void cgui_windowClassFactory_setWindowClassName(CGUI_WindowClassFactory* factory, LPCSTR lpszClassName);

/* Creates a WindowClass with the given WindowClassFactory.
 * Note that this function is not intended for WindowClass registration.
 * Also, the after calling this function, all the parameters of the WindowClassFactory will be reset. */
CGUI_Result cgui_windowClassFactory_createWindowClass(CGUI_WindowClassFactory* factory);

/* Structure of WindowClass.
 * This type, when registered to a `CGUI_WindowClassManager`,
 * shall not be released manually. */
typedef struct WindowClass {
    WNDCLASS wc;
} CGUI_WindowClass;

/* Registers a window class.
 * This should be called before creating a window. */
CGUI_Result cgui_registerWindowClass(CGUI_WindowClass* wc);

/* Destroys a window class.
 * Note that this function will unregister the window class as well. */
CGUI_Result cgui_destroyWindowClass(CGUI_WindowClass* self);


/* Structure of WindowManager. */
typedef struct WindowManager {
    HINSTANCE hInstance;
    HashTable* windows;

    CGUI_Result (* addWindow)(CGUI_WindowManager* self, CGUI_Window* window);

    CGUI_Option (* getWindow)(CGUI_WindowManager* self, const char* name);

    CGUI_Option (* getWindowByHwnd)(CGUI_WindowManager* self, HWND hwnd);

    CGUI_Result (* removeWindow)(CGUI_WindowManager* self, CGUI_Window* window);

    CGUI_Result (* removeWindowByIdentifier)(CGUI_WindowManager* self, const char* wndIdentifier);

    CGUI_Result (* destroyAllWindows)(CGUI_WindowManager* self);
} CGUI_WindowManager;

/* Constructors and Destructors of WindowManager. */
CGUI_WindowManager* cgui_createWindowManager(HINSTANCE hInstance);

/* Destroys a WindowManager.
 * Note that this should dispose all existing windows. */
void cgui_destroyWindowManager(CGUI_WindowManager* manager);

/* Methods of WindowManager. */

/* Adds a window to the manager.
 * DO NOT free the window passed in! It will be freed when the manager is disposed. */
CGUI_Result cgui_windowManager_addWindow(CGUI_WindowManager* manager, CGUI_Window* window);

/* Gets a window from the manager.
 * Note that the `wndIdentifier` parameter is **NOT** the name of the window.
 * Instead, it is the wndIdentifier property of CGUI_Window.
 * This property is randomly generated. */
CGUI_Option cgui_windowManager_getWindow(CGUI_WindowManager* self, const char* wndIdentifier);

/* Gets a window from the manager by its HWND. */
CGUI_Option cgui_windowManager_getWindowByHwnd(CGUI_WindowManager* manager, HWND hwnd);

/* Removes a window from the manager. */
CGUI_Result cgui_windowManager_removeWindow(CGUI_WindowManager* manager, CGUI_Window* window);

/* Removes a window from the manager by its CGUI_Window.wndIdentifier. */
CGUI_Result cgui_windowManager_removeWindowByIdentifier(CGUI_WindowManager* manager, const char* wndIdentifier);

/* Destroys all windows in the manager. */
CGUI_Result cgui_windowManager_destroyAllWindows(CGUI_WindowManager* manager);


/* Structure of WindowClassManager. */
typedef struct WindowClassManager {
    HashTable* windowClasses;

    CGUI_Result (* addWindowClass)(CGUI_WindowClassManager* self, CGUI_WindowClass* wc);

    CGUI_Result (* addWindowClassAndRegister)(CGUI_WindowClassManager* self, CGUI_WindowClass* wc);

    CGUI_Option (* getWindowClass)(CGUI_WindowClassManager* self, const char* name);

    CGUI_Result (* removeWindowClass)(CGUI_WindowClassManager* self, CGUI_WindowClass* wc);

    CGUI_Result (* removeWindowClassByName)(CGUI_WindowClassManager* self, const char* name);

    CGUI_Result (* destroyAllWindowClasses)(CGUI_WindowClassManager* self);
} CGUI_WindowClassManager;

/* Constructors and Destructors of WindowClassManager. */
CGUI_WindowClassManager* cgui_createWindowClassManager();

void cgui_destroyWindowClassManager(CGUI_WindowClassManager* manager);

/* Methods of WindowClassManager. */

/* Adds a window class to the manager. */
CGUI_Result cgui_windowClassManager_addWindowClass(CGUI_WindowClassManager* manager, CGUI_WindowClass* wc);

/* Adds a window class to the manager and registers it. */
CGUI_Result cgui_windowClassManager_addWindowClassAndRegister(CGUI_WindowClassManager* manager, CGUI_WindowClass* wc);

/* Gets a window class from the manager. */
CGUI_Option cgui_windowClassManager_getWindowClass(CGUI_WindowClassManager* manager, const char* name);

/* Removes a window class from the manager. */
CGUI_Result cgui_windowClassManager_removeWindowClass(CGUI_WindowClassManager* manager, CGUI_WindowClass* wc);

/* Removes a window class from the manager by its name. */
CGUI_Result cgui_windowClassManager_removeWindowClassByName(CGUI_WindowClassManager* manager, const char* name);

/* Destroys all window classes in the manager. */
CGUI_Result cgui_windowClassManager_destroyAllWindowClasses(CGUI_WindowClassManager* manager);

#endif //CGUI_WINDOW_H
