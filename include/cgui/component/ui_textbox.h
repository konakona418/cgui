//
// Created on: 2024/11/29, 11:05.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_TEXTBOX_H
#define CGUI_UI_TEXTBOX_H

#include "ui.h"
#include "trait.h"
#include "../win32/window.h"
#include "../util/common.h"
#include "../util/misc.h"
#include "../win32/gdi.h"

typedef struct UINativeTextBox CGUI_UINativeTextBox;

/**
 * Secondary encapsulation of native win32 textbox control instance.
 */
typedef struct UINativeTextBox {
    CGUI_UIComponent* component;

    CGUI_Window* window;

    CGUI_GDITextContext* gdiTextContext;
    obsolete bool _gdiRefreshFlag;

    void                (* show)                 (CGUI_UINativeTextBox* self);
    void                (* hide)                 (CGUI_UINativeTextBox* self);
    void                (* update)               (CGUI_UINativeTextBox* self);
    CGUI_Result         (* close)                (CGUI_UINativeTextBox* self);
    CGUI_Result         (* postMessage)          (CGUI_UINativeTextBox* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

    void                (* setText)              (CGUI_UINativeTextBox* self, LPCSTR text);
    LPCSTR              (* getText)              (CGUI_UINativeTextBox* self);
    void                (* setTextDisplay)       (CGUI_UINativeTextBox* self, CGUI_GDITextContext* gdiTextContext);
    CGUI_GDITextContext (* getTextDisplay)       (CGUI_UINativeTextBox* self);

    /* implementation for CGUI_UIComponent */
    void                (* addChild)            (CGUI_UINativeTextBox* self, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChild)         (CGUI_UINativeTextBox* self, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChildById)     (CGUI_UINativeTextBox* self, LONG_PTR id);

    CGUI_Result (* setComponentName)    (CGUI_UINativeTextBox* self, LPCSTR name);
    LPCSTR      (* getComponentName)    (CGUI_UINativeTextBox* self);

    void        (* setEventHandler)     (CGUI_UINativeTextBox* self, CGUI_EventHandler* handler);

    /* implementation for CGUI_UIDrawable */
    void (* draw)       (CGUI_UINativeTextBox* self);
    void (* refresh)    (CGUI_UINativeTextBox* self);

    /* implementation for CGUI_UILayout */
    CGUI_Result       (* setGeometry) (CGUI_UINativeTextBox* self, CGUI_Rectangle geometry);
    CGUI_Result       (* setLayout)   (CGUI_UINativeTextBox* self, CGUI_Layout layout);
    CGUI_Rectangle*   (* getGeometry) (CGUI_UINativeTextBox* self);
    CGUI_Layout*      (* getLayout)   (CGUI_UINativeTextBox* self);

    /* implementation for CGUI_UIState */
    CGUI_Result (* setVisible)  (CGUI_UINativeTextBox* self, bool visible);
    CGUI_Result (* setEnabled)  (CGUI_UINativeTextBox* self, bool enabled);
    bool        (* isVisible)   (CGUI_UINativeTextBox* self);
    bool        (* isEnabled)   (CGUI_UINativeTextBox* self);

} CGUI_UINativeTextBox;

void cgui_uiNativeTextbox_show(CGUI_UINativeTextBox* self);

void cgui_uiNativeTextbox_hide(CGUI_UINativeTextBox* self);

void cgui_uiNativeTextbox_update(CGUI_UINativeTextBox* self);

CGUI_Result cgui_uiNativeTextbox_close(CGUI_UINativeTextBox* self);

CGUI_Result cgui_uiNativeTextbox_postMessage(CGUI_UINativeTextBox* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

void cgui_uiNativeTextbox_setText(CGUI_UINativeTextBox* self, LPCSTR text);

void cgui_uiNativeTextbox_setTextDisplay(CGUI_UINativeTextBox* self, CGUI_GDITextContext* gdiTextContext);

LPCSTR cgui_uiNativeTextbox_getText(CGUI_UINativeTextBox* self);

CGUI_GDITextContext cgui_uiNativeTextbox_getTextDisplay(CGUI_UINativeTextBox* self);

/**
 * Create a native textbox component.
 * @warning this will not automatically create a native win32 textbox instance.
 * @warning set the window instance using @p cgui_uiNativeTextbox_bindWindowInstance
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeTextBox* cgui_createUINativeTextbox(CGUI_UIComponent* parent, CGUI_InternalID internalId);

/**
 * Create a native window component from a native win32 textbox instance.
 * @param nativeWindow the REAL implementation of native window.
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeTextBox* cgui_createUINativeTextboxFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId);

CGUI_Result cgui_uiNativeTextbox_bindWindowInstance(CGUI_UINativeTextBox* self, CGUI_Window* nativeWindow);

/**
 * Destroy a native textbox component.
 * @warning this will @b destroy the native textbox, which may lead to potential double free if the native window is registered to a manager.
 * @note if the component is the child of another component, the child component will be destroyed automatically.
 * @note however if the component is the root component, it has to be destroyed manually.
 * @param window the component to destroy. */
void cgui_destroyUINativeTextbox(CGUI_UINativeTextBox* textbox);

void cgui_uiNativeTextbox_addChild(CGUI_UINativeTextBox* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeTextbox_removeChild(CGUI_UINativeTextBox* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeTextbox_removeChildById(CGUI_UINativeTextBox* self, LONG_PTR id);

CGUI_Result cgui_uiNativeTextbox_setComponentName(CGUI_UINativeTextBox* self, LPCSTR name);

LPCSTR cgui_uiNativeTextbox_getComponentName(CGUI_UINativeTextBox* self);

void cgui_uiNativeTextbox_setEventHandler(CGUI_UINativeTextBox* self, CGUI_EventHandler* handler);

void cgui_uiNativeTextbox_draw(CGUI_UINativeTextBox* self);

void cgui_uiNativeTextbox_refresh(CGUI_UINativeTextBox* self);

CGUI_Result cgui_uiNativeTextbox_setGeometry(CGUI_UINativeTextBox* self, CGUI_Rectangle geometry);

CGUI_Result cgui_uiNativeTextbox_setLayout(CGUI_UINativeTextBox* self, CGUI_Layout layout);

CGUI_Rectangle* cgui_uiNativeTextbox_getGeometry(CGUI_UINativeTextBox* self);

CGUI_Layout* cgui_uiNativeTextbox_getLayout(CGUI_UINativeTextBox* self);

bool cgui_uiNativeTextbox_isVisible(CGUI_UINativeTextBox* self);

bool cgui_uiNativeTextbox_isEnabled(CGUI_UINativeTextBox* self);

CGUI_Result cgui_uiNativeTextbox_setVisible(CGUI_UINativeTextBox* self, bool visible);

CGUI_Result cgui_uiNativeTextbox_setEnabled(CGUI_UINativeTextBox* self, bool enabled);

HWND cgui_uiNativeTextbox_getWindowHandle(CGUI_UIComponent* component);

void cgui_uiNativeTextbox_readyCallback(CGUI_UIComponent* component);

void cgui_uiNativeTextbox_drawCallback(CGUI_UIComponent* component);

void cgui_uiNativeTextbox_refreshCallback(CGUI_UIComponent* component);

void cgui_uiNativeTextbox_destroyCallback(CGUI_UIComponent* component);

#endif //CGUI_UI_TEXTBOX_H
