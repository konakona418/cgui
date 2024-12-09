//
// Created on: 2024/11/29, 11:06.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_LIST_H
#define CGUI_UI_LIST_H

#include "ui.h"
#include "trait.h"
#include "../win32/window.h"
#include "../util/common.h"
#include "../util/misc.h"
#include "../win32/gdi.h"

typedef struct UINativeListView CGUI_UINativeListView;

/**
 * Secondary encapsulation of native win32 listView control instance.
 */
typedef struct UINativeListView {
    CGUI_UIComponent* component;

    CGUI_Window* window;

    CGUI_ListViewSelectionMode selectionMode;

    CGUI_GDITextContext* gdiTextContext;
    obsolete bool _gdiRefreshFlag;

    void (* show)(CGUI_UINativeListView* self);

    void (* hide)(CGUI_UINativeListView* self);

    void (* update)(CGUI_UINativeListView* self);

    CGUI_Result (* close)(CGUI_UINativeListView* self);

    CGUI_Result (* postMessage)(CGUI_UINativeListView* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

    void (* setTextDisplay)(CGUI_UINativeListView* self, CGUI_GDITextContext* gdiTextContext);

    CGUI_GDITextContext (* getTextDisplay)(CGUI_UINativeListView* self);

    CGUI_Result (* appendItem)(CGUI_UINativeListView* self, LPCSTR text);

    CGUI_Result (* insertItem)(CGUI_UINativeListView* self, LPCSTR text, CGUI_ListViewItemSelector itemSelector);

    CGUI_Result (* removeItem)(CGUI_UINativeListView* self, CGUI_ListViewItemSelector itemSelector);

    CGUI_ListViewItems (* getSelectedItem)(CGUI_UINativeListView* self);

    /* implementation for CGUI_UIComponent */
    void (* addChild)(CGUI_UINativeListView* self, CGUI_UIComponent* child);

    CGUI_UIComponent* (* removeChild)(CGUI_UINativeListView* self, CGUI_UIComponent* child);

    CGUI_UIComponent* (* removeChildById)(CGUI_UINativeListView* self, LONG_PTR id);

    CGUI_Result (* setComponentName)(CGUI_UINativeListView* self, LPCSTR name);

    LPCSTR (* getComponentName)(CGUI_UINativeListView* self);

    void (* setEventHandler)(CGUI_UINativeListView* self, CGUI_EventHandler* handler);

    /* implementation for CGUI_UIDrawable */
    void (* draw)(CGUI_UINativeListView* self);

    void (* refresh)(CGUI_UINativeListView* self);

    /* implementation for CGUI_UILayout */
    CGUI_Result (* setGeometry)(CGUI_UINativeListView* self, CGUI_Rectangle geometry);

    CGUI_Result (* setLayout)(CGUI_UINativeListView* self, CGUI_Layout layout);

    CGUI_Rectangle* (* getGeometry)(CGUI_UINativeListView* self);

    CGUI_Layout* (* getLayout)(CGUI_UINativeListView* self);

    /* implementation for CGUI_UIState */
    CGUI_Result (* setVisible)(CGUI_UINativeListView* self, bool visible);

    CGUI_Result (* setEnabled)(CGUI_UINativeListView* self, bool enabled);

    bool (* isVisible)(CGUI_UINativeListView* self);

    bool (* isEnabled)(CGUI_UINativeListView* self);

} CGUI_UINativeListView;

void cgui_uiNativeListView_show(CGUI_UINativeListView* self);

void cgui_uiNativeListView_hide(CGUI_UINativeListView* self);

void cgui_uiNativeListView_update(CGUI_UINativeListView* self);

CGUI_Result cgui_uiNativeListView_close(CGUI_UINativeListView* self);

CGUI_Result
cgui_uiNativeListView_postMessage(CGUI_UINativeListView* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

void cgui_uiNativeListView_setTextDisplay(CGUI_UINativeListView* self, CGUI_GDITextContext* gdiTextContext);

unavailable CGUI_GDITextContext cgui_uiNativeListView_getTextDisplay(CGUI_UINativeListView* self);

CGUI_Result cgui_uiNativeListView_appendItem(CGUI_UINativeListView* self, LPCSTR text);

CGUI_Result
cgui_uiNativeListView_insertItem(CGUI_UINativeListView* self, LPCSTR text, CGUI_ListViewItemSelector itemSelector);

CGUI_Result cgui_uiNativeListView_removeItem(CGUI_UINativeListView* self, CGUI_ListViewItemSelector itemSelector);

CGUI_ListViewItems cgui_uiNativeListView_getSelectedItem(CGUI_UINativeListView* self);

/**
 * Create a native listView component.
 * @warning this will not automatically create a native win32 listView instance.
 * @warning set the window instance using @p cgui_uiNativeListView_bindWindowInstance
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeListView* cgui_createUINativeListView(CGUI_UIComponent* parent, CGUI_InternalID internalId);

/**
 * Create a native window component from a native win32 listView instance.
 * @param nativeWindow the REAL implementation of native window.
 * @param parent the parent component.
 * @return the created component. */
CGUI_UINativeListView*
cgui_createUINativeListViewFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId);

CGUI_Result cgui_uiNativeListView_bindWindowInstance(CGUI_UINativeListView* self, CGUI_Window* nativeWindow);

/**
 * Destroy a native listView component.
 * @warning this will @b destroy the native listView, which may lead to potential double free if the native window is registered to a manager.
 * @note if the component is the child of another component, the child component will be destroyed automatically.
 * @note however if the component is the root component, it has to be destroyed manually.
 * @param window the component to destroy. */
void cgui_destroyUINativeListView(CGUI_UINativeListView* listView);

void cgui_uiNativeListView_addChild(CGUI_UINativeListView* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeListView_removeChild(CGUI_UINativeListView* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeListView_removeChildById(CGUI_UINativeListView* self, LONG_PTR id);

CGUI_Result cgui_uiNativeListView_setComponentName(CGUI_UINativeListView* self, LPCSTR name);

LPCSTR cgui_uiNativeListView_getComponentName(CGUI_UINativeListView* self);

void cgui_uiNativeListView_setEventHandler(CGUI_UINativeListView* self, CGUI_EventHandler* handler);

void cgui_uiNativeListView_draw(CGUI_UINativeListView* self);

void cgui_uiNativeListView_refresh(CGUI_UINativeListView* self);

CGUI_Result cgui_uiNativeListView_setGeometry(CGUI_UINativeListView* self, CGUI_Rectangle geometry);

CGUI_Result cgui_uiNativeListView_setLayout(CGUI_UINativeListView* self, CGUI_Layout layout);

CGUI_Rectangle* cgui_uiNativeListView_getGeometry(CGUI_UINativeListView* self);

CGUI_Layout* cgui_uiNativeListView_getLayout(CGUI_UINativeListView* self);

bool cgui_uiNativeListView_isVisible(CGUI_UINativeListView* self);

bool cgui_uiNativeListView_isEnabled(CGUI_UINativeListView* self);

CGUI_Result cgui_uiNativeListView_setVisible(CGUI_UINativeListView* self, bool visible);

CGUI_Result cgui_uiNativeListView_setEnabled(CGUI_UINativeListView* self, bool enabled);

HWND cgui_uiNativeListView_getWindowHandle(CGUI_UIComponent* component);

void cgui_uiNativeListView_readyCallback(CGUI_UIComponent* component);

void cgui_uiNativeListView_drawCallback(CGUI_UIComponent* component);

void cgui_uiNativeListView_refreshCallback(CGUI_UIComponent* component);

void cgui_uiNativeListView_destroyCallback(CGUI_UIComponent* component);

#endif //CGUI_UI_LIST_H
