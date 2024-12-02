//
// Created on: 2024/11/29, 11:05.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_textbox.h"
#include "../util/error.h"

CGUI_UINativeTextBox* cgui_createUINativeTextboxFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    CGUI_UINativeTextBox* textbox = (CGUI_UINativeTextBox*) malloc(sizeof(CGUI_UINativeTextBox));

    LPCSTR wndIdentifier = nativeWindow ? nativeWindow->wndIdentifier : "(anonymous)";
    LONG_PTR wndId = (LONG_PTR) (nativeWindow ? nativeWindow->getWindowHandle(nativeWindow) : 0);
    textbox->component = cgui_createUIComponent(cgui_concatString("textbox", wndIdentifier),
                                              (LONG_PTR) internalId,
                                              parent,
                                              CGUI_Trait_UIComponent | CGUI_Trait_UILayout | CGUI_Trait_UIDrawable
                                              | CGUI_Trait_UIState | CGUI_Trait_UIDisposable | CGUI_Trait_UIWin32);

    textbox->component->layoutImpl = cgui_createUILayout();
    textbox->component->drawableImpl = cgui_createUIDrawable(
            cgui_uiNativeTextbox_readyCallback,
            cgui_uiNativeTextbox_drawCallback,
            cgui_uiNativeTextbox_refreshCallback);
    textbox->component->stateImpl = cgui_createUIState();
    textbox->component->disposableImpl = cgui_createUIDisposable(textbox, cgui_uiNativeTextbox_destroyCallback);
    textbox->component->win32Impl = cgui_createUIWin32(cgui_uiNativeTextbox_getWindowHandle, internalId);

    textbox->window = nativeWindow;

    textbox->gdiTextContext = NULL;
    textbox->_gdiRefreshFlag = false;

    textbox->show = cgui_uiNativeTextbox_show;
    textbox->hide = cgui_uiNativeTextbox_hide;
    textbox->close = cgui_uiNativeTextbox_close;
    textbox->update = cgui_uiNativeTextbox_update;
    textbox->postMessage = cgui_uiNativeTextbox_postMessage;

    textbox->setText = cgui_uiNativeTextbox_setText;
    textbox->setTextDisplay = cgui_uiNativeTextbox_setTextDisplay;
    textbox->getText = cgui_uiNativeTextbox_getText;
    textbox->getTextDisplay = cgui_uiNativeTextbox_getTextDisplay;

    textbox->addChild = cgui_uiNativeTextbox_addChild;
    textbox->removeChild = cgui_uiNativeTextbox_removeChild;
    textbox->removeChildById = cgui_uiNativeTextbox_removeChildById;
    textbox->setComponentName = cgui_uiNativeTextbox_setComponentName;
    textbox->getComponentName = cgui_uiNativeTextbox_getComponentName;
    textbox->setEventHandler = cgui_uiNativeTextbox_setEventHandler;

    textbox->draw = cgui_uiNativeTextbox_draw;
    textbox->refresh = cgui_uiNativeTextbox_refresh;

    textbox->getGeometry = cgui_uiNativeTextbox_getGeometry;
    textbox->getLayout = cgui_uiNativeTextbox_getLayout;
    textbox->getGeometry = cgui_uiNativeTextbox_getGeometry;
    textbox->setGeometry = cgui_uiNativeTextbox_setGeometry;
    textbox->setLayout = cgui_uiNativeTextbox_setLayout;

    textbox->isEnabled = cgui_uiNativeTextbox_isEnabled;
    textbox->isVisible = cgui_uiNativeTextbox_isVisible;
    textbox->setEnabled = cgui_uiNativeTextbox_setEnabled;
    textbox->setVisible = cgui_uiNativeTextbox_setVisible;

    return textbox;
}

CGUI_UINativeTextBox* cgui_createUINativeTextbox(CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    return cgui_createUINativeTextboxFromWindow(NULL, parent, internalId);
}

void cgui_destroyUINativeTextbox(CGUI_UINativeTextBox* textbox) {
    cgui_destroyWindowInstance(textbox->window);
    free(textbox);
}

CGUI_Result cgui_uiNativeTextbox_bindWindowInstance(CGUI_UINativeTextBox* self, CGUI_Window* nativeWindow) {
    if (self == NULL || nativeWindow == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->window = nativeWindow;

    LPCSTR wndIdentifier = nativeWindow->wndIdentifier;
    self->component->name = cgui_concatString("textbox", wndIdentifier);
    self->component->id = (LONG_PTR) nativeWindow->getWindowHandle(nativeWindow);

    return create_ok(NULL);
}

void cgui_uiNativeTextbox_show(CGUI_UINativeTextBox* self) {
    self->update(self);
    self->window->show(self->window);
}

void cgui_uiNativeTextbox_hide(CGUI_UINativeTextBox* self) {
    self->window->hide(self->window);
}

void cgui_uiNativeTextbox_update(CGUI_UINativeTextBox* self) {
    self->window->update(self->window);
}

CGUI_Result cgui_uiNativeTextbox_close(CGUI_UINativeTextBox* self) {
    return self->window->close(self->window);
}

CGUI_Result cgui_uiNativeTextbox_postMessage(CGUI_UINativeTextBox* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (isAsync) {
        return self->window->postMessageAsync(self->window, msg, wParam, lParam);
    } else {
        return self->window->postMessageSync(self->window, msg, wParam, lParam);
    }
}

void cgui_uiNativeTextbox_setText(CGUI_UINativeTextBox* self, LPCSTR text) {
    self->window->setWindowName(self->window, text);
    self->update(self);
}

void cgui_uiNativeTextbox_setTextDisplay(CGUI_UINativeTextBox* self, CGUI_GDITextContext* gdiTextContext) {
    self->gdiTextContext = gdiTextContext;
    self->_gdiRefreshFlag = true;

    CGUI_Win32ESParam esParam = cgui_textAlignIntoEditStyle(gdiTextContext);
    self->window->setWindowStyle(self->window, self->window->getWindowStyle(self->window) | esParam);

    HFONT hFont = cgui_createFont(gdiTextContext);
    self->postMessage(self, false, WM_SETFONT, (WPARAM) hFont, 0);
    self->update(self);
}

LPCSTR cgui_uiNativeTextbox_getText(CGUI_UINativeTextBox* self) {
    return self->window->getWindowName(self->window);
}

CGUI_GDITextContext cgui_uiNativeTextbox_getTextDisplay(CGUI_UINativeTextBox* self) {
    // todo: get text display
    unwrap(create_err(CGUI_Error_NotImplemented()));
    return (CGUI_GDITextContext) {0};
}

void cgui_uiNativeTextbox_addChild(CGUI_UINativeTextBox* self, CGUI_UIComponent* child) {
    self->component->addChild(self->component, child);
    self->update(self);
}

CGUI_UIComponent* cgui_uiNativeTextbox_removeChild(CGUI_UINativeTextBox* self, CGUI_UIComponent* child) {
    return self->component->removeChild(self->component, child);
}

CGUI_UIComponent* cgui_uiNativeTextbox_removeChildById(CGUI_UINativeTextBox* self, LONG_PTR id) {
    return self->component->removeChildById(self->component, id);
}

CGUI_Result cgui_uiNativeTextbox_setComponentName(CGUI_UINativeTextBox* self, LPCSTR name) {
    return self->component->setComponentName(self->component, name);
}

LPCSTR cgui_uiNativeTextbox_getComponentName(CGUI_UINativeTextBox* self) {
    return self->component->getComponentName(self->component);
}

void cgui_uiNativeTextbox_setEventHandler(CGUI_UINativeTextBox* self, CGUI_EventHandler* handler) {
    self->component->setEventHandler(self->component, handler);
}

void cgui_uiNativeTextbox_draw(CGUI_UINativeTextBox* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->draw(self->component);
    }
}

void cgui_uiNativeTextbox_refresh(CGUI_UINativeTextBox* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->refresh(self->component);
    }
}

CGUI_Result cgui_uiNativeTextbox_setGeometry(CGUI_UINativeTextBox* self, CGUI_Rectangle geometry) {
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

CGUI_Result cgui_uiNativeTextbox_setLayout(CGUI_UINativeTextBox* self, CGUI_Layout layout) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Rectangle* cgui_uiNativeTextbox_getGeometry(CGUI_UINativeTextBox* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UILayout)) {
        return self->component->layoutImpl->getGeometry(self->component);
    }
    return NULL;
}

CGUI_Layout* cgui_uiNativeTextbox_getLayout(CGUI_UINativeTextBox* self) {
    return NULL;
}

bool cgui_uiNativeTextbox_isVisible(CGUI_UINativeTextBox* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isVisible(self->component);
    } else {
        return false;
    }
}

bool cgui_uiNativeTextbox_isEnabled(CGUI_UINativeTextBox* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isEnabled(self->component);
    } else {
        return false;
    }
}

CGUI_Result cgui_uiNativeTextbox_setVisible(CGUI_UINativeTextBox* self, bool visible) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setVisible(self->component, visible);
        if (visible) {
            self->update(self);
            return self->window->show(self->window);
        } else {
            return self->window->hide(self->window);
        }
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

CGUI_Result cgui_uiNativeTextbox_setEnabled(CGUI_UINativeTextBox* self, bool enabled) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setEnabled(self->component, enabled);
        return self->window->setEnabled(self->window, enabled);
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

HWND cgui_uiNativeTextbox_getWindowHandle(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeTextBox* nativeWindow = (CGUI_UINativeTextBox*)component->disposableImpl->upperLevel;
        return nativeWindow->window->getWindowHandle(nativeWindow->window);
    } else {
        return NULL;
    }
}

void cgui_uiNativeTextbox_readyCallback(CGUI_UIComponent* component) {
    dbg_printf("uiNativeLabel ready: %s\n", component->name);
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeTextBox* self = (CGUI_UINativeTextBox*) component->disposableImpl->upperLevel;
        self->update(self);
    }
}

void cgui_uiNativeTextbox_drawCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeTextbox_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeTextbox_refreshCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeTextbox_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeTextbox_destroyCallback(CGUI_UIComponent* component) {
    // note: the UIComponent and UIDisposable are freed by the parent.
    cgui_destroyUIDrawable(component->drawableImpl);
    cgui_destroyUILayout(component->layoutImpl);
    cgui_destroyUIState(component->stateImpl);
    cgui_destroyUIWin32(component->win32Impl);

    cgui_destroyUINativeTextbox((CGUI_UINativeTextBox*)component->disposableImpl->upperLevel);
}

