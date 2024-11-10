//
// Created on: 2024/11/10, 19:10.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <windows.h>
#include "ui_window.h"

CGUI_UINativeWindow* cgui_createUINativeWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent) {
    CGUI_UINativeWindow* window = (CGUI_UINativeWindow*) malloc(sizeof(CGUI_UINativeWindow));

    window->component = cgui_createUIComponent(
            "window",
            (LONG_PTR)nativeWindow->getWindowHandle(nativeWindow),
            parent,
            CGUI_Trait_UIComponent | CGUI_Trait_UILayout | CGUI_Trait_UIDrawable | CGUI_Trait_UIState | CGUI_Trait_UIDisposable);

    window->component->layoutImpl = cgui_createUILayout();
    window->component->drawableImpl = cgui_createUIDrawable(
            cgui_uiNativeWindow_drawCallback,
            cgui_uiNativeWindow_refreshCallback);
    window->component->stateImpl = cgui_createUIState();
    window->component->disposableImpl = cgui_createUIDisposable(window, cgui_uiNativeWindow_destroyCallback);

    window->window = nativeWindow;

    /* trait implementation for CGUI_UIComponent */
    window->addChild = cgui_uiNativeWindow_addChild;
    window->removeChild = cgui_uiNativeWindow_removeChild;
    window->removeChildById = cgui_uiNativeWindow_removeChildById;
    window->setComponentName = cgui_uiNativeWindow_setComponentName;
    window->getComponentName = cgui_uiNativeWindow_getComponentName;
    window->setEventHandler = cgui_uiNativeWindow_setEventHandler;

    /* trait implementation for CGUI_UIDrawable */
    window->draw = cgui_uiNativeWindow_draw;
    window->refresh = cgui_uiNativeWindow_refresh;

    /* trait implementation for CGUI_UILayout */
    window->getGeometry = cgui_uiNativeWindow_getGeometry;
    window->getLayout = cgui_uiNativeWindow_getLayout;
    window->setGeometry = cgui_uiNativeWindow_setGeometry;
    window->setLayout = cgui_uiNativeWindow_setLayout;

    /* trait implementation for CGUI_UIState */
    window->isEnabled = cgui_uiNativeWindow_isEnabled;
    window->isVisible = cgui_uiNativeWindow_isVisible;
    window->setEnabled = cgui_uiNativeWindow_setEnabled;
    window->setVisible = cgui_uiNativeWindow_setVisible;

    return window;
}

void cgui_destroyUINativeWindow(CGUI_UINativeWindow* window) {
    // note: the native window is freed HERE.
    cgui_destroyWindow(window->window);
    free(window);
}

void cgui_uiNativeWindow_destroyCallback(CGUI_UIComponent* component) {
    // note: the UIComponent and UIDisposable are freed by the parent.
    cgui_destroyUIDrawable(component->drawableImpl);
    cgui_destroyUILayout(component->layoutImpl);
    cgui_destroyUIState(component->stateImpl);

    cgui_destroyUINativeWindow((CGUI_UINativeWindow*)component->disposableImpl->upperLevel);
}
