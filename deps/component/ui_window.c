//
// Created on: 2024/11/10, 19:10.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <windows.h>
#include "ui_window.h"

CGUI_UINativeWindow* cgui_createUINativeWindowFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    CGUI_UINativeWindow* window = (CGUI_UINativeWindow*) malloc(sizeof(CGUI_UINativeWindow));

    LPCSTR wndIdentifier = nativeWindow ? nativeWindow->wndIdentifier : "(anonymous)";
    LONG_PTR wndId = (LONG_PTR) (nativeWindow ? nativeWindow->getWindowHandle(nativeWindow) : 0);
    window->component = cgui_createUIComponent(
            cgui_concatString("window", wndIdentifier),
            (LONG_PTR) internalId,
            parent,
            CGUI_Trait_UIComponent | CGUI_Trait_UILayout | CGUI_Trait_UIDrawable
            | CGUI_Trait_UIState | CGUI_Trait_UIDisposable | CGUI_Trait_UIWin32);

    window->component->layoutImpl = cgui_createUILayout();
    window->component->drawableImpl = cgui_createUIDrawable(
            NULL,
            cgui_uiNativeWindow_drawCallback,
            cgui_uiNativeWindow_refreshCallback);
    window->component->stateImpl = cgui_createUIState();
    window->component->disposableImpl = cgui_createUIDisposable(window, cgui_uiNativeWindow_destroyCallback);
    window->component->win32Impl = cgui_createUIWin32(cgui_uiNativeWindow_getWindowHandle, internalId);

    window->window = nativeWindow;

    /* methods */
    window->bindWindowInstance = cgui_uiNativeWindow_bindWindowInstance;
    window->show = cgui_uiNativeWindow_show;
    window->hide = cgui_uiNativeWindow_hide;
    window->update = cgui_uiNativeWindow_update;
    window->close = cgui_uiNativeWindow_close;
    window->destroy = cgui_uiNativeWindow_destroy;
    window->setWindowName = cgui_uiNativeWindow_setWindowName;
    window->setState = cgui_uiNativeWindow_setState;
    window->postMessage = cgui_uiNativeWindow_postMessage;

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

CGUI_UINativeWindow* cgui_createUINativeWindow(CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    return cgui_createUINativeWindowFromWindow(NULL, parent, internalId);
}

CGUI_Result cgui_uiNativeWindow_bindWindowInstance(CGUI_UINativeWindow* self, CGUI_Window* nativeWindow) {
    if (self == NULL || nativeWindow == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->window = nativeWindow;

    LPCSTR wndIdentifier = nativeWindow->wndIdentifier;
    self->component->name = cgui_concatString("window", wndIdentifier);
    self->component->id = (LONG_PTR) nativeWindow->getWindowHandle(nativeWindow);

    return create_ok(NULL);
}

void cgui_destroyUINativeWindow(CGUI_UINativeWindow* window) {
    // note: the native window is freed HERE.
    cgui_destroyWindowInstance(window->window);
    free(window);
}

void cgui_uiNativeWindow_drawCallback(CGUI_UIComponent* component) {
    // todo: temporary implementation.
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            cgui_uiNativeWindow_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeWindow_refreshCallback(CGUI_UIComponent* component) {
    // todo: refresh the native window.
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            cgui_uiNativeWindow_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeWindow_destroyCallback(CGUI_UIComponent* component) {
    // note: the UIComponent and UIDisposable are freed by the parent.
    cgui_destroyUIDrawable(component->drawableImpl);
    cgui_destroyUILayout(component->layoutImpl);
    cgui_destroyUIState(component->stateImpl);
    cgui_destroyUIWin32(component->win32Impl);

    cgui_destroyUINativeWindow((CGUI_UINativeWindow*)component->disposableImpl->upperLevel);
}

CGUI_Result cgui_uiNativeWindow_setState(CGUI_UINativeWindow* self, int swState) {
    return self->window->setState(self->window, swState);
}

CGUI_Result cgui_uiNativeWindow_postMessage(CGUI_UINativeWindow* self, UINT msg, WPARAM wParam, LPARAM lParam) {
    return self->window->postMessageAsync(self->window, msg, wParam, lParam);
}

CGUI_Result cgui_uiNativeWindow_show(CGUI_UINativeWindow* self) {
    return self->window->show(self->window);
}

CGUI_Result cgui_uiNativeWindow_hide(CGUI_UINativeWindow* self) {
    return self->window->hide(self->window);
}

CGUI_Result cgui_uiNativeWindow_update(CGUI_UINativeWindow* self) {
    return self->window->update(self->window);
}

CGUI_Result cgui_uiNativeWindow_close(CGUI_UINativeWindow* self) {
    return self->window->close(self->window);
}

CGUI_Result cgui_uiNativeWindow_destroy(CGUI_UINativeWindow* self) {
    return self->window->destroy(self->window);
}

CGUI_Result cgui_uiNativeWindow_setWindowName(CGUI_UINativeWindow* self, LPCSTR wndName) {
    return self->window->setWindowName(self->window, wndName);
}

void cgui_uiNativeWindow_addChild(CGUI_UINativeWindow* self, CGUI_UIComponent* child) {
    self->component->addChild(self->component, child);
    self->update(self); // refresh the window.
}

CGUI_UIComponent* cgui_uiNativeWindow_removeChild(CGUI_UINativeWindow* self, CGUI_UIComponent* child) {
    return self->component->removeChild(self->component, child);
}

CGUI_UIComponent* cgui_uiNativeWindow_removeChildById(CGUI_UINativeWindow* self, LONG_PTR id) {
    return self->component->removeChildById(self->component, id);
}

CGUI_Result cgui_uiNativeWindow_setComponentName(CGUI_UINativeWindow* self, LPCSTR name) {
    return self->component->setComponentName(self->component, name);
}

LPCSTR cgui_uiNativeWindow_getComponentName(CGUI_UINativeWindow* self) {
    return self->component->getComponentName(self->component);
}

void cgui_uiNativeWindow_setEventHandler(CGUI_UINativeWindow* self, CGUI_EventHandler* handler) {
    self->component->setEventHandler(self->component, handler);
}

void cgui_uiNativeWindow_draw(CGUI_UINativeWindow* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->draw(self->component);
    }
}

void cgui_uiNativeWindow_refresh(CGUI_UINativeWindow* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->refresh(self->component);
    }
}

CGUI_Result cgui_uiNativeWindow_setGeometry(CGUI_UINativeWindow* self, CGUI_Rectangle geometry) {
    CGUI_Result result;
    if (impl(self->component->implFlag, CGUI_Trait_UILayout)) {
        result = self->component->layoutImpl->setGeometry(self->component, geometry);
        if (is_err(&result)) {
            return result;
        }
        result = self->window->setWindowGeometryRect(self->window, &geometry);
        return result;
    }
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Result cgui_uiNativeWindow_setLayout(CGUI_UINativeWindow* self, CGUI_Layout layout) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Rectangle* cgui_uiNativeWindow_getGeometry(CGUI_UINativeWindow* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UILayout)) {
        return self->component->layoutImpl->getGeometry(self->component);
    }
    return NULL;
}

CGUI_Layout* cgui_uiNativeWindow_getLayout(CGUI_UINativeWindow* self) {
    return NULL;
}

bool cgui_uiNativeWindow_isVisible(CGUI_UINativeWindow* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isVisible(self->component);
    } else {
        return false;
    }
}

bool cgui_uiNativeWindow_isEnabled(CGUI_UINativeWindow* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isEnabled(self->component);
    } else {
        return false;
    }
}

CGUI_Result cgui_uiNativeWindow_setVisible(CGUI_UINativeWindow* self, bool visible) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setVisible(self->component, visible);
        if (visible) {
            return self->window->show(self->window);
        } else {
            return self->window->hide(self->window);
        }
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

CGUI_Result cgui_uiNativeWindow_setEnabled(CGUI_UINativeWindow* self, bool enabled) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setEnabled(self->component, enabled);
        return self->window->setEnabled(self->window, enabled);
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

HWND cgui_uiNativeWindow_getWindowHandle(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeWindow* nativeWindow = (CGUI_UINativeWindow*)component->disposableImpl->upperLevel;
        return nativeWindow->window->getWindowHandle(nativeWindow->window);
    } else {
        return NULL;
    }
}

