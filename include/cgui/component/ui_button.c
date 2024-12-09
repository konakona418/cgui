//
// Created on: 2024/11/29, 11:06.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_button.h"
#include "../util/error.h"
#include <windows.h>

CGUI_UINativeButton*
cgui_createUINativeButtonFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    CGUI_UINativeButton* button = (CGUI_UINativeButton*) malloc(sizeof(CGUI_UINativeButton));

    button->buttonStyle = CGUI_ButtonStyle_Default;
    button->buttonState = CGUI_ButtonState_Normal;
    button->gdiTextContext = NULL;
    button->_gdiRefreshFlag = false;

    LPCSTR wndIdentifier = nativeWindow ? nativeWindow->wndIdentifier : "(anonymous)";
    LONG_PTR wndId = (LONG_PTR) (nativeWindow ? nativeWindow->getWindowHandle(nativeWindow) : 0);
    button->component = cgui_createUIComponent(
            cgui_concatString("window", wndIdentifier),
            (LONG_PTR) internalId,
            parent,
            CGUI_Trait_UIComponent | CGUI_Trait_UILayout | CGUI_Trait_UIDrawable
            | CGUI_Trait_UIState | CGUI_Trait_UIDisposable | CGUI_Trait_UIWin32);

    button->component->layoutImpl = cgui_createUILayout();
    button->component->drawableImpl = cgui_createUIDrawable(
            cgui_uiNativeButton_readyCallback,
            cgui_uiNativeButton_drawCallback,
            cgui_uiNativeButton_refreshCallback);
    button->component->stateImpl = cgui_createUIState();
    button->component->disposableImpl = cgui_createUIDisposable(button, cgui_uiNativeButton_destroyCallback);
    button->component->win32Impl = cgui_createUIWin32(cgui_uiNativeButton_getWindowHandle, internalId);

    button->window = nativeWindow;

    /* methods */
    button->bindWindowInstance = cgui_uiNativeButton_bindWindowInstance;
    button->show = cgui_uiNativeButton_show;
    button->hide = cgui_uiNativeButton_hide;
    button->update = cgui_uiNativeButton_update;
    button->close = cgui_uiNativeButton_close;
    button->destroy = cgui_uiNativeButton_destroy;
    button->setState = cgui_uiNativeButton_setState;
    button->postMessage = cgui_uiNativeButton_postMessage;

    button->getText = cgui_uiNativeButton_getText;
    button->setText = cgui_uiNativeButton_setText;
    button->setTextDisplay = cgui_uiNativeButton_setTextDisplay;
    button->setButtonStyle = cgui_uiNativeButton_setButtonStyle;
    button->getButtonStyle = cgui_uiNativeButton_getButtonStyle;
    button->setButtonState = cgui_uiNativeButton_setButtonState;
    button->getButtonState = cgui_uiNativeButton_getButtonState;

    /* trait implementation for CGUI_UIComponent */
    button->addChild = cgui_uiNativeButton_addChild;
    button->removeChild = cgui_uiNativeButton_removeChild;
    button->removeChildById = cgui_uiNativeButton_removeChildById;
    button->setComponentName = cgui_uiNativeButton_setComponentName;
    button->getComponentName = cgui_uiNativeButton_getComponentName;
    button->setEventHandler = cgui_uiNativeButton_setEventHandler;

    /* trait implementation for CGUI_UIDrawable */
    button->draw = cgui_uiNativeButton_draw;
    button->refresh = cgui_uiNativeButton_refresh;

    /* trait implementation for CGUI_UILayout */
    button->getGeometry = cgui_uiNativeButton_getGeometry;
    button->getLayout = cgui_uiNativeButton_getLayout;
    button->setGeometry = cgui_uiNativeButton_setGeometry;
    button->setLayout = cgui_uiNativeButton_setLayout;

    /* trait implementation for CGUI_UIState */
    button->isEnabled = cgui_uiNativeButton_isEnabled;
    button->isVisible = cgui_uiNativeButton_isVisible;
    button->setEnabled = cgui_uiNativeButton_setEnabled;
    button->setVisible = cgui_uiNativeButton_setVisible;

    return button;
}

CGUI_UINativeButton* cgui_createUINativeButton(CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    return cgui_createUINativeButtonFromWindow(NULL, parent, internalId);
}

CGUI_Result cgui_uiNativeButton_bindWindowInstance(CGUI_UINativeButton* self, CGUI_Window* nativeWindow) {
    if (self == NULL || nativeWindow == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->window = nativeWindow;

    LPCSTR wndIdentifier = nativeWindow->wndIdentifier;
    self->component->name = cgui_concatString("window", wndIdentifier);
    self->component->id = (LONG_PTR) nativeWindow->getWindowHandle(nativeWindow);

    return create_ok(NULL);
}

void cgui_destroyUINativeButton(CGUI_UINativeButton* window) {
    cgui_destroyWindowInstance(window->window);
    cgui_destroyGdiTextContext(window->gdiTextContext);
    free(window);
}

void cgui_uiNativeButton_readyCallback(CGUI_UIComponent* component) {
    dbg_printf("uiNativeButton ready: %s\n", component->name);
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeButton* self = (CGUI_UINativeButton*) component->disposableImpl->upperLevel;
        self->update(self);
    }
}

void cgui_uiNativeButton_drawCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeButton_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeButton_refreshCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeButton_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeButton_destroyCallback(CGUI_UIComponent* component) {
    cgui_destroyUIDrawable(component->drawableImpl);
    cgui_destroyUILayout(component->layoutImpl);
    cgui_destroyUIState(component->stateImpl);
    cgui_destroyUIWin32(component->win32Impl);

    cgui_destroyUINativeButton((CGUI_UINativeButton*) component->disposableImpl->upperLevel);
}

CGUI_Result cgui_uiNativeButton_setState(CGUI_UINativeButton* self, int swState) {
    return self->window->setState(self->window, swState);
}

CGUI_Result
cgui_uiNativeButton_postMessage(CGUI_UINativeButton* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (isAsync) {
        return self->window->postMessageAsync(self->window, msg, wParam, lParam);
    } else {
        return self->window->postMessageSync(self->window, msg, wParam, lParam);
    }
}

CGUI_Result cgui_uiNativeButton_show(CGUI_UINativeButton* self) {
    return self->window->show(self->window);
}

CGUI_Result cgui_uiNativeButton_hide(CGUI_UINativeButton* self) {
    return self->window->hide(self->window);
}

CGUI_Result cgui_uiNativeButton_update(CGUI_UINativeButton* self) {
    return self->window->update(self->window);
}

CGUI_Result cgui_uiNativeButton_close(CGUI_UINativeButton* self) {
    return self->window->close(self->window);
}

CGUI_Result cgui_uiNativeButton_destroy(CGUI_UINativeButton* self) {
    return self->window->destroy(self->window);
}

void cgui_uiNativeButton_addChild(CGUI_UINativeButton* self, CGUI_UIComponent* child) {
    self->component->addChild(self->component, child);
    self->update(self); // refresh the window.
}

CGUI_UIComponent* cgui_uiNativeButton_removeChild(CGUI_UINativeButton* self, CGUI_UIComponent* child) {
    return self->component->removeChild(self->component, child);
}

CGUI_UIComponent* cgui_uiNativeButton_removeChildById(CGUI_UINativeButton* self, LONG_PTR id) {
    return self->component->removeChildById(self->component, id);
}

CGUI_Result cgui_uiNativeButton_setComponentName(CGUI_UINativeButton* self, LPCSTR name) {
    return self->component->setComponentName(self->component, name);
}

LPCSTR cgui_uiNativeButton_getComponentName(CGUI_UINativeButton* self) {
    return self->component->getComponentName(self->component);
}

void cgui_uiNativeButton_setEventHandler(CGUI_UINativeButton* self, CGUI_EventHandler* handler) {
    self->component->setEventHandler(self->component, handler);
}

void cgui_uiNativeButton_draw(CGUI_UINativeButton* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->draw(self->component);
    }
}

void cgui_uiNativeButton_refresh(CGUI_UINativeButton* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->refresh(self->component);
    }
}

CGUI_Result cgui_uiNativeButton_setGeometry(CGUI_UINativeButton* self, CGUI_Rectangle geometry) {
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

CGUI_Result cgui_uiNativeButton_setLayout(CGUI_UINativeButton* self, CGUI_Layout layout) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Rectangle* cgui_uiNativeButton_getGeometry(CGUI_UINativeButton* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UILayout)) {
        return self->component->layoutImpl->getGeometry(self->component);
    }
    return NULL;
}

CGUI_Layout* cgui_uiNativeButton_getLayout(CGUI_UINativeButton* self) {
    return NULL;
}

bool cgui_uiNativeButton_isVisible(CGUI_UINativeButton* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isVisible(self->component);
    } else {
        return false;
    }
}

bool cgui_uiNativeButton_isEnabled(CGUI_UINativeButton* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isEnabled(self->component);
    } else {
        return false;
    }
}

CGUI_Result cgui_uiNativeButton_setVisible(CGUI_UINativeButton* self, bool visible) {
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

CGUI_Result cgui_uiNativeButton_setEnabled(CGUI_UINativeButton* self, bool enabled) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setEnabled(self->component, enabled);
        return self->window->setEnabled(self->window, enabled);
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

HWND cgui_uiNativeButton_getWindowHandle(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeButton* nativeWindow = (CGUI_UINativeButton*) component->disposableImpl->upperLevel;
        return nativeWindow->window->getWindowHandle(nativeWindow->window);
    } else {
        return NULL;
    }
}

LPCSTR cgui_uiNativeButton_getText(CGUI_UINativeButton* self) {
    return self->window->getWindowName(self->window);
}

CGUI_Result cgui_uiNativeButton_setText(CGUI_UINativeButton* self, LPCSTR text) {
    self->window->setWindowName(self->window, text);
    return self->update(self);
}

CGUI_Result cgui_uiNativeButton_setTextDisplay(CGUI_UINativeButton* self, CGUI_GDITextContext* gdiTextContext) {
    CGUI_Window* wnd = self->window;

    if (self->getButtonStyle(self) != CGUI_ButtonStyle_Default) {
        if (gdiTextContext->alignHorizontal == CGUI_TextAlignmentH_Center) {
            wnd->setWindowStyle(wnd, wnd->getWindowStyle(wnd) | BS_CENTER);
        } else if (gdiTextContext->alignHorizontal == CGUI_TextAlignmentH_Left) {
            wnd->setWindowStyle(wnd, wnd->getWindowStyle(wnd) | BS_LEFT);
        } else {
            wnd->setWindowStyle(wnd, wnd->getWindowStyle(wnd) | BS_RIGHT);
        }

        HFONT font = cgui_createFont(gdiTextContext);
        self->postMessage(self, false, WM_SETFONT, (WPARAM) font, 0);
    } else {
        dbg_printf("Default button state, use deferred font setting instead.\n");
    }

    self->gdiTextContext = gdiTextContext;
    self->_gdiRefreshFlag = true;

    return self->update(self);
}

CGUI_Result cgui_uiNativeButton_setButtonStyle(CGUI_UINativeButton* self, CGUI_UINativeButtonStyle style) {
    self->buttonStyle = style;
    CGUI_Window* wnd = self->window;

    if (style == CGUI_ButtonStyle_Default) {
        return wnd->setWindowStyle(wnd, wnd->getWindowStyle(wnd) | CGUI_DEFAULT_BUTTON_PRESET);
    } else if (style == CGUI_ButtonStyle_CheckBox) {
        return wnd->setWindowStyle(wnd, wnd->getWindowStyle(wnd) | CGUI_CHECKBOX_BUTTON_PRESET);
    } else if (style == CGUI_ButtonStyle_RadioButton) {
        return wnd->setWindowStyle(wnd, wnd->getWindowStyle(wnd) | CGUI_RADIOBUTTON_BUTTON_PRESET);
    }

    return self->update(self);
}

CGUI_UINativeButtonStyle cgui_uiNativeButton_getButtonStyle(CGUI_UINativeButton* self) {
    return self->buttonStyle;
}

#define is_radio_or_checkbox(style) (style == CGUI_ButtonStyle_CheckBox || style == CGUI_ButtonStyle_RadioButton)

CGUI_Result cgui_uiNativeButton_setButtonState(CGUI_UINativeButton* self, CGUI_UINativeButtonState state) {
    CGUI_Window* wnd = self->window;
    CGUI_Result result;

    if (state == CGUI_ButtonState_Normal) {
        if (self->buttonStyle == CGUI_ButtonStyle_Default) {
            result = wnd->postMessageAsync(wnd, BM_SETSTATE, BST_UNCHECKED, 0);
        } else {
            panic("cgui_uiNativeButton_setButtonState: Not a normal button!");
        }
    } else if (state == CGUI_ButtonState_Pushed) {
        if (self->buttonStyle == CGUI_ButtonStyle_Default) {
            result = wnd->postMessageAsync(wnd, BM_SETSTATE, BST_PUSHED, 0);
        } else {
            panic("cgui_uiNativeButton_setButtonState: Not a normal button!");
        }
    } else if (state == CGUI_ButtonState_Checked) {
        if (is_radio_or_checkbox(self->buttonStyle)) {
            result = wnd->postMessageAsync(wnd, BM_SETCHECK, BST_CHECKED, 0);
        } else {
            panic("cgui_uiNativeButton_setButtonState: Not a checkbox!");
        }
    } else {
        if (is_radio_or_checkbox(self->buttonStyle)) {
            result = wnd->postMessageAsync(wnd, BM_SETCHECK, BST_UNCHECKED, 0);
        } else {
            panic("cgui_uiNativeButton_setButtonState: Not a checkbox!");
        }
    }

    self->buttonState = state;
    return result;
}

CGUI_UINativeButtonState cgui_uiNativeButton_getButtonState(CGUI_UINativeButton* self) {
    return self->buttonState;
}

