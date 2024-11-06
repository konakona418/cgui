//
// Created on: 2024/11/5, 23:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_WINDOW_H
#define CGUI_WINDOW_H

#include <windows.h>
#include <assert.h>

#include "../util/common.h"

#include "../geo/rect.h"

/* Prototype of WindowFactory and Window. */
typedef struct WindowFactory CGUI_WindowFactory;
typedef struct Window CGUI_Window;

typedef struct WindowClassFactory CGUI_WindowClassFactory;
typedef struct WindowClass CGUI_WindowClass;

/* Result and Option of Window. */

/* Result of Window creation. */
CGUI_Result_Prototype(CGUI_Window, CGUI_Error)
CGUI_Result_Define(CGUI_Window, CGUI_Error);


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

    CGUI_Result(CGUI_Window) (* createWindow)(CGUI_WindowFactory* self);
} CGUI_WindowFactory;

/* Constructors and Destructors of WindowFactory. */

/* Creates a WindowFactory. */
CGUI_WindowFactory* cgui_createWindowFactory();

/* Destroys a WindowFactory. */
void cgui_destroyWindowFactory(struct WindowFactory* factory);

/* Setters of WindowFactory. */

/* Sets the style of the window. */
void cgui_windowFactory_setWindowStyle(CGUI_WindowFactory* factory, DWORD dwStyle);

/* Sets the extended style of the window.
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

/* Creates a window with the given WindowFactory. */
CGUI_Result(CGUI_Window) cgui_windowFactory_createWindow(CGUI_WindowFactory* factory);


/* Structure of Window. */
typedef struct Window {
    HWND hwnd;
    int swState;

    CGUI_Result(void) (* show)(CGUI_Window* self);
    CGUI_Result(void) (* hide)(CGUI_Window* self);
    CGUI_Result(void) (* setState)(CGUI_Window* self, int swState);
} CGUI_Window;

/* Constructors and Destructors of Window. */

/* Creates a Window struct.
 * Note that this function is not intended for Window creation. Use WindowFactory instead. */
CGUI_Window* cgui_createWindow(HWND hwnd);

/* Destroys a Window struct. */
CGUI_Result(void) cgui_destroyWindow(CGUI_Window* self);

/* Methods of Window. */
CGUI_Result(void) cgui_window_show(CGUI_Window* self);

/* Hides the window. */
CGUI_Result(void) cgui_window_hide(CGUI_Window* self);

/* Sets the state of the window. e.g. SW_SHOW to show the window. */
CGUI_Result(void) cgui_window_setState(CGUI_Window* self, int swState);


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
    CGUI_WindowClass* (* createWindowClass)(CGUI_WindowClassFactory* self);
} CGUI_WindowClassFactory;

/* Constructors and Destructors of WindowClassFactory. */
CGUI_WindowClassFactory* cgui_createWindowClassFactory();
void cgui_destroyWindowClassFactory(CGUI_WindowClassFactory* factory);

/* Setters of WindowClassFactory. */
void cgui_windowClassFactory_setWindowClassStyle(CGUI_WindowClassFactory* factory, UINT style);
void cgui_windowClassFactory_setWindowProc(CGUI_WindowClassFactory* factory, WNDPROC lpfnWndProc);
void cgui_windowClassFactory_setWindowClassExtraSize(CGUI_WindowClassFactory* factory, int cbClsExtra);
void cgui_windowClassFactory_setWindowExtraSize(CGUI_WindowClassFactory* factory, int cbWndExtra);
void cgui_windowClassFactory_setWindowInstance(CGUI_WindowClassFactory* factory, HINSTANCE hInstance);
void cgui_windowClassFactory_setWindowIcon(CGUI_WindowClassFactory* factory, HICON hIcon);
void cgui_windowClassFactory_setWindowCursor(CGUI_WindowClassFactory* factory, HCURSOR hCursor);
void cgui_windowClassFactory_setWindowBackgroundBrush(CGUI_WindowClassFactory* factory, HBRUSH hbrBackground);
void cgui_windowClassFactory_setWindowMenuName(CGUI_WindowClassFactory* factory, LPCSTR lpszMenuName);
void cgui_windowClassFactory_setWindowClassName(CGUI_WindowClassFactory* factory, LPCSTR lpszClassName);
CGUI_WindowClass* cgui_windowClassFactory_createWindowClass(CGUI_WindowClassFactory* factory);

/* Structure of WindowClass. */
typedef struct WindowClass {
    WNDCLASS wc;
} CGUI_WindowClass;

/* Registers a window class. */
CGUI_Result(void) cgui_registerWindowClass(CGUI_WindowClass* wc);

/* Destroys a window class. */
CGUI_Result(void) cgui_destroyWindowClass(CGUI_WindowClass* self);

#endif //CGUI_WINDOW_H
