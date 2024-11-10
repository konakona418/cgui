//
// Created on: 2024/11/10, 19:10.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_WINDOW_H
#define CGUI_UI_WINDOW_H

#include "ui.h"
#include "../win32/window.h"

#define CGUI_UIWindow CGUI_UINativeWindow
#define UIWindow UINativeWindow

typedef struct UINativeWindow CGUI_UINativeWindow;

typedef struct UINativeWindow {
    CGUI_UIComponent* component;

    CGUI_Window* window;

    /* methods for CGUI_Window */
    CGUI_Result (* setState)(CGUI_UINativeWindow* self, int swState);
    CGUI_Result (* hide)(CGUI_UINativeWindow* self);
    CGUI_Result (* show)(CGUI_UINativeWindow* self);
    CGUI_Result (* close)(CGUI_UINativeWindow* self);
    CGUI_Result (* setWindowName)(CGUI_UINativeWindow* self, LPCSTR wndName);

    /* implementation for CGUI_UIComponent */
    void                (* addChild)            (CGUI_UINativeWindow* component, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChild)         (CGUI_UINativeWindow* component, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChildById)     (CGUI_UINativeWindow* component, LONG_PTR id);

    CGUI_Result (* setComponentName)    (CGUI_UINativeWindow* component, LPCSTR name);
    LPCSTR      (* getComponentName)    (CGUI_UINativeWindow* component);

    void        (* setEventHandler)     (CGUI_UINativeWindow* component, CGUI_EventHandler* handler);

    /* implementation for CGUI_UIDrawable */
    void (* draw)(CGUI_UINativeWindow* component);
    void (* refresh)(CGUI_UINativeWindow* component);

    /* implementation for CGUI_UILayout */
    CGUI_Result       (* setGeometry) (CGUI_UINativeWindow* component, CGUI_Rectangle geometry);
    CGUI_Result       (* setLayout)   (CGUI_UINativeWindow* component, CGUI_Layout layout);
    CGUI_Rectangle*   (* getGeometry) (CGUI_UINativeWindow* component);
    CGUI_Layout*      (* getLayout)   (CGUI_UINativeWindow* component);

    /* implementation for CGUI_UIState */
    CGUI_Result (* setVisible)  (CGUI_UINativeWindow* component, bool visible);
    CGUI_Result (* setEnabled)  (CGUI_UINativeWindow* component, bool enabled);
    bool        (* isVisible)   (CGUI_UINativeWindow* component);
    bool        (* isEnabled)   (CGUI_UINativeWindow* component);
} CGUI_UINativeWindow;

/**
 * Create a native window component.
 * @param nativeWindow the REAL implementation of native window.
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeWindow* cgui_createUINativeWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent);

/**
 * Destroy a native window component.
 * @warning this will @b destroy the native window, which may lead to potential double free if the native window is registered to a manager.
 * @note if the component is the child of another component, the child component will be destroyed automatically.
 * @note however if the component is the root component, it has to be destroyed manually.
 * @param window the component to destroy. */
void cgui_destroyUINativeWindow(CGUI_UINativeWindow* window);

// todo: window method implementation.

void cgui_uiNativeWindow_destroyCallback(CGUI_UIComponent* component);

void cgui_uiNativeWindow_drawCallback(CGUI_UIComponent* component);

void cgui_uiNativeWindow_refreshCallback(CGUI_UIComponent* component);

CGUI_Result cgui_uiNativeWindow_setState(CGUI_UINativeWindow* self, int swState);

CGUI_Result cgui_uiNativeWindow_hide(CGUI_UINativeWindow* self);

CGUI_Result cgui_uiNativeWindow_show(CGUI_UINativeWindow* self);

CGUI_Result cgui_uiNativeWindow_close(CGUI_UINativeWindow* self);

CGUI_Result cgui_uiNativeWindow_setWindowName(CGUI_UINativeWindow* self, LPCSTR wndName);

// todo: trait implementation.

void cgui_uiNativeWindow_addChild(CGUI_UINativeWindow* component, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeWindow_removeChild(CGUI_UINativeWindow* component, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeWindow_removeChildById(CGUI_UINativeWindow* component, LONG_PTR id);

CGUI_Result cgui_uiNativeWindow_setComponentName(CGUI_UINativeWindow* component, LPCSTR name);

LPCSTR cgui_uiNativeWindow_getComponentName(CGUI_UINativeWindow* component);

void cgui_uiNativeWindow_setEventHandler(CGUI_UINativeWindow* component, CGUI_EventHandler* handler);

void cgui_uiNativeWindow_draw(CGUI_UINativeWindow* component);

void cgui_uiNativeWindow_refresh(CGUI_UINativeWindow* component);

CGUI_Result cgui_uiNativeWindow_setGeometry(CGUI_UINativeWindow* component, CGUI_Rectangle geometry);

CGUI_Result cgui_uiNativeWindow_setLayout(CGUI_UINativeWindow* component, CGUI_Layout layout);

CGUI_Rectangle* cgui_uiNativeWindow_getGeometry(CGUI_UINativeWindow* component);

CGUI_Layout* cgui_uiNativeWindow_getLayout(CGUI_UINativeWindow* component);

bool cgui_uiNativeWindow_isVisible(CGUI_UINativeWindow* component);

bool cgui_uiNativeWindow_isEnabled(CGUI_UINativeWindow* component);

CGUI_Result cgui_uiNativeWindow_setVisible(CGUI_UINativeWindow* component, bool visible);

CGUI_Result cgui_uiNativeWindow_setEnabled(CGUI_UINativeWindow* component, bool enabled);

#endif //CGUI_UI_WINDOW_H
