//
// Created on: 2024/11/27, 18:03.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_LABEL_H
#define CGUI_UI_LABEL_H

#include "ui.h"
#include "trait.h"
#include "../win32/window.h"
#include "../util/common.h"
#include "../util/misc.h"
#include "../win32/gdi.h"

typedef struct UINativeLabel CGUI_UINativeLabel;
typedef CGUI_UINativeLabel CGUI_UINativeImageLabel;

/**
 * Secondary encapsulation of native win32 label control instance.
 */
typedef struct UINativeLabel {
    CGUI_UIComponent* component;

    CGUI_Window* window;

    CGUI_GDITextContext* gdiTextContext;
    obsolete bool _gdiRefreshFlag;

    void                (* show)                 (CGUI_UINativeLabel* self);
    void                (* hide)                 (CGUI_UINativeLabel* self);
    void                (* update)               (CGUI_UINativeLabel* self);
    CGUI_Result         (* close)                (CGUI_UINativeLabel* self);
    CGUI_Result         (* postMessage)          (CGUI_UINativeLabel* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

    void                (* setText)              (CGUI_UINativeLabel* self, LPCSTR text);
    LPCSTR              (* getText)              (CGUI_UINativeLabel* self);
    void                (* setTextDisplay)       (CGUI_UINativeLabel* self, CGUI_GDITextContext* gdiTextContext);
    CGUI_GDITextContext (* getTextDisplay)       (CGUI_UINativeLabel* self);
    void                (* setImage)             (CGUI_UINativeLabel* self, CGUI_ImageLike imageLike);

    /* implementation for CGUI_UIComponent */
    void                (* addChild)            (CGUI_UINativeLabel* self, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChild)         (CGUI_UINativeLabel* self, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChildById)     (CGUI_UINativeLabel* self, LONG_PTR id);

    CGUI_Result (* setComponentName)    (CGUI_UINativeLabel* self, LPCSTR name);
    LPCSTR      (* getComponentName)    (CGUI_UINativeLabel* self);

    void        (* setEventHandler)     (CGUI_UINativeLabel* self, CGUI_EventHandler* handler);

    /* implementation for CGUI_UIDrawable */
    void (* draw)       (CGUI_UINativeLabel* self);
    void (* refresh)    (CGUI_UINativeLabel* self);

    /* implementation for CGUI_UILayout */
    CGUI_Result       (* setGeometry) (CGUI_UINativeLabel* self, CGUI_Rectangle geometry);
    CGUI_Result       (* setLayout)   (CGUI_UINativeLabel* self, CGUI_Layout layout);
    CGUI_Rectangle*   (* getGeometry) (CGUI_UINativeLabel* self);
    CGUI_Layout*      (* getLayout)   (CGUI_UINativeLabel* self);

    /* implementation for CGUI_UIState */
    CGUI_Result (* setVisible)  (CGUI_UINativeLabel* self, bool visible);
    CGUI_Result (* setEnabled)  (CGUI_UINativeLabel* self, bool enabled);
    bool        (* isVisible)   (CGUI_UINativeLabel* self);
    bool        (* isEnabled)   (CGUI_UINativeLabel* self);

} CGUI_UINativeLabel;

void cgui_uiNativeLabel_show(CGUI_UINativeLabel* self);

void cgui_uiNativeLabel_hide(CGUI_UINativeLabel* self);

void cgui_uiNativeLabel_update(CGUI_UINativeLabel* self);

CGUI_Result cgui_uiNativeLabel_close(CGUI_UINativeLabel* self);

CGUI_Result cgui_uiNativeLabel_postMessage(CGUI_UINativeLabel* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

void cgui_uiNativeLabel_setText(CGUI_UINativeLabel* self, LPCSTR text);

void cgui_uiNativeLabel_setTextDisplay(CGUI_UINativeLabel* self, CGUI_GDITextContext* gdiTextContext);

LPCSTR cgui_uiNativeLabel_getText(CGUI_UINativeLabel* self);

CGUI_GDITextContext cgui_uiNativeLabel_getTextDisplay(CGUI_UINativeLabel* self);

void cgui_uiNativeLabel_setImage(CGUI_UINativeLabel* self, CGUI_ImageLike imageLike);

/**
 * Create a native label component.
 * @warning this will not automatically create a native win32 label instance.
 * @warning set the window instance using @p cgui_uiNativeLabel_bindWindowInstance
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeLabel* cgui_createUINativeLabel(CGUI_UIComponent* parent, CGUI_InternalID internalId);

/**
 * Create a native window component from a native win32 label instance.
 * @param nativeWindow the REAL implementation of native window.
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeLabel* cgui_createUINativeLabelFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId);

CGUI_Result cgui_uiNativeLabel_bindWindowInstance(CGUI_UINativeLabel* self, CGUI_Window* nativeWindow);

/**
 * Destroy a native label component.
 * @warning this will @b destroy the native label, which may lead to potential double free if the native window is registered to a manager.
 * @note if the component is the child of another component, the child component will be destroyed automatically.
 * @note however if the component is the root component, it has to be destroyed manually.
 * @param window the component to destroy. */
void cgui_destroyUINativeLabel(CGUI_UINativeLabel* label);

void cgui_uiNativeLabel_addChild(CGUI_UINativeLabel* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeLabel_removeChild(CGUI_UINativeLabel* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeLabel_removeChildById(CGUI_UINativeLabel* self, LONG_PTR id);

CGUI_Result cgui_uiNativeLabel_setComponentName(CGUI_UINativeLabel* self, LPCSTR name);

LPCSTR cgui_uiNativeLabel_getComponentName(CGUI_UINativeLabel* self);

void cgui_uiNativeLabel_setEventHandler(CGUI_UINativeLabel* self, CGUI_EventHandler* handler);

void cgui_uiNativeLabel_draw(CGUI_UINativeLabel* self);

void cgui_uiNativeLabel_refresh(CGUI_UINativeLabel* self);

CGUI_Result cgui_uiNativeLabel_setGeometry(CGUI_UINativeLabel* self, CGUI_Rectangle geometry);

CGUI_Result cgui_uiNativeLabel_setLayout(CGUI_UINativeLabel* self, CGUI_Layout layout);

CGUI_Rectangle* cgui_uiNativeLabel_getGeometry(CGUI_UINativeLabel* self);

CGUI_Layout* cgui_uiNativeLabel_getLayout(CGUI_UINativeLabel* self);

bool cgui_uiNativeLabel_isVisible(CGUI_UINativeLabel* self);

bool cgui_uiNativeLabel_isEnabled(CGUI_UINativeLabel* self);

CGUI_Result cgui_uiNativeLabel_setVisible(CGUI_UINativeLabel* self, bool visible);

CGUI_Result cgui_uiNativeLabel_setEnabled(CGUI_UINativeLabel* self, bool enabled);

HWND cgui_uiNativeLabel_getWindowHandle(CGUI_UIComponent* component);

void cgui_uiNativeLabel_readyCallback(CGUI_UIComponent* component);

void cgui_uiNativeLabel_drawCallback(CGUI_UIComponent* component);

void cgui_uiNativeLabel_refreshCallback(CGUI_UIComponent* component);

void cgui_uiNativeLabel_destroyCallback(CGUI_UIComponent* component);

#endif //CGUI_UI_LABEL_H
