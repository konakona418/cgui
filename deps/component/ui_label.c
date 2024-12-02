//
// Created on: 2024/11/27, 18:03.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_label.h"
#include "../util/error.h"

CGUI_UINativeLabel* cgui_createUINativeLabelFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    CGUI_UINativeLabel* label = (CGUI_UINativeLabel*) malloc(sizeof(CGUI_UINativeLabel));

    LPCSTR wndIdentifier = nativeWindow ? nativeWindow->wndIdentifier : "(anonymous)";
    LONG_PTR wndId = (LONG_PTR) (nativeWindow ? nativeWindow->getWindowHandle(nativeWindow) : 0);
    label->component = cgui_createUIComponent(cgui_concatString("label", wndIdentifier),
                                              (LONG_PTR) internalId,
                                              parent, 
                                              CGUI_Trait_UIComponent | CGUI_Trait_UILayout | CGUI_Trait_UIDrawable
                                              | CGUI_Trait_UIState | CGUI_Trait_UIDisposable | CGUI_Trait_UIWin32);
    
    label->component->layoutImpl = cgui_createUILayout();
    label->component->drawableImpl = cgui_createUIDrawable(
            cgui_uiNativeLabel_readyCallback,
            cgui_uiNativeLabel_drawCallback,
            cgui_uiNativeLabel_refreshCallback);
    label->component->stateImpl = cgui_createUIState();
    label->component->disposableImpl = cgui_createUIDisposable(label, cgui_uiNativeLabel_destroyCallback);
    label->component->win32Impl = cgui_createUIWin32(cgui_uiNativeLabel_getWindowHandle, internalId);
    
    label->window = nativeWindow;

    label->gdiTextContext = NULL;
    label->_gdiRefreshFlag = false;

    label->show = cgui_uiNativeLabel_show;
    label->hide = cgui_uiNativeLabel_hide;
    label->close = cgui_uiNativeLabel_close;
    label->update = cgui_uiNativeLabel_update;
    label->postMessage = cgui_uiNativeLabel_postMessage;

    label->setText = cgui_uiNativeLabel_setText;
    label->setTextDisplay = cgui_uiNativeLabel_setTextDisplay;
    label->getText = cgui_uiNativeLabel_getText;
    label->getTextDisplay = cgui_uiNativeLabel_getTextDisplay;
    
    label->addChild = cgui_uiNativeLabel_addChild;
    label->removeChild = cgui_uiNativeLabel_removeChild;
    label->removeChildById = cgui_uiNativeLabel_removeChildById;
    label->setComponentName = cgui_uiNativeLabel_setComponentName;
    label->getComponentName = cgui_uiNativeLabel_getComponentName;
    label->setEventHandler = cgui_uiNativeLabel_setEventHandler;
    
    label->draw = cgui_uiNativeLabel_draw;
    label->refresh = cgui_uiNativeLabel_refresh;
    
    label->getGeometry = cgui_uiNativeLabel_getGeometry;
    label->getLayout = cgui_uiNativeLabel_getLayout;
    label->getGeometry = cgui_uiNativeLabel_getGeometry;
    label->setLayout = cgui_uiNativeLabel_setLayout;
    label->setGeometry = cgui_uiNativeLabel_setGeometry;
    
    label->isEnabled = cgui_uiNativeLabel_isEnabled;
    label->isVisible = cgui_uiNativeLabel_isVisible;
    label->setEnabled = cgui_uiNativeLabel_setEnabled;
    label->setVisible = cgui_uiNativeLabel_setVisible;
    
    return label;
}

CGUI_UINativeLabel* cgui_createUINativeLabel(CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    return cgui_createUINativeLabelFromWindow(NULL, parent, internalId);
}

void cgui_destroyUINativeLabel(CGUI_UINativeLabel* label) {
    cgui_destroyWindowInstance(label->window);
    free(label);
}

CGUI_Result cgui_uiNativeLabel_bindWindowInstance(CGUI_UINativeLabel* self, CGUI_Window* nativeWindow) {
    if (self == NULL || nativeWindow == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->window = nativeWindow;

    LPCSTR wndIdentifier = nativeWindow->wndIdentifier;
    self->component->name = cgui_concatString("label", wndIdentifier);
    self->component->id = (LONG_PTR) nativeWindow->getWindowHandle(nativeWindow);

    return create_ok(NULL);
}

void cgui_uiNativeLabel_show(CGUI_UINativeLabel* self) {
    self->update(self);
    self->window->show(self->window);
}

void cgui_uiNativeLabel_hide(CGUI_UINativeLabel* self) {
    self->window->hide(self->window);
}

void cgui_uiNativeLabel_update(CGUI_UINativeLabel* self) {
    self->window->update(self->window);
}

CGUI_Result cgui_uiNativeLabel_close(CGUI_UINativeLabel* self) {
    return self->window->close(self->window);
}

CGUI_Result cgui_uiNativeLabel_postMessage(CGUI_UINativeLabel* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (isAsync) {
        return self->window->postMessageAsync(self->window, msg, wParam, lParam);
    } else {
        return self->window->postMessageSync(self->window, msg, wParam, lParam);
    }
}

void cgui_uiNativeLabel_setText(CGUI_UINativeLabel* self, LPCSTR text) {
    self->window->setWindowName(self->window, text);
    self->update(self);
}

void cgui_uiNativeLabel_setTextDisplay(CGUI_UINativeLabel* self, CGUI_GDITextContext* gdiTextContext) {
    self->gdiTextContext = gdiTextContext;
    self->_gdiRefreshFlag = true;

    CGUI_Win32SSParam ssParam = cgui_textAlignIntoStaticStyle(gdiTextContext);
    self->window->setWindowStyle(self->window, self->window->getWindowStyle(self->window) | ssParam);

    // todo: note that the default implementation of label will ignore the V-align property.

    HFONT hFont = cgui_createFont(gdiTextContext);
    self->postMessage(self, false, WM_SETFONT, (WPARAM) hFont, 0);
    self->update(self); // this will signal repaint.
}

LPCSTR cgui_uiNativeLabel_getText(CGUI_UINativeLabel* self) {
    return self->window->getWindowName(self->window);
}

CGUI_GDITextContext cgui_uiNativeLabel_getTextDisplay(CGUI_UINativeLabel* self) {
    // todo: get text display
    unwrap(create_err(CGUI_Error_NotImplemented()));
    return (CGUI_GDITextContext) {0};
}

void cgui_uiNativeLabel_addChild(CGUI_UINativeLabel* self, CGUI_UIComponent* child) {
    self->component->addChild(self->component, child);
    self->update(self);
}

CGUI_UIComponent* cgui_uiNativeLabel_removeChild(CGUI_UINativeLabel* self, CGUI_UIComponent* child) {
    return self->component->removeChild(self->component, child);
}

CGUI_UIComponent* cgui_uiNativeLabel_removeChildById(CGUI_UINativeLabel* self, LONG_PTR id) {
    return self->component->removeChildById(self->component, id);
}

CGUI_Result cgui_uiNativeLabel_setComponentName(CGUI_UINativeLabel* self, LPCSTR name) {
    return self->component->setComponentName(self->component, name);
}

LPCSTR cgui_uiNativeLabel_getComponentName(CGUI_UINativeLabel* self) {
    return self->component->getComponentName(self->component);
}

void cgui_uiNativeLabel_setEventHandler(CGUI_UINativeLabel* self, CGUI_EventHandler* handler) {
    self->component->setEventHandler(self->component, handler);
}

void cgui_uiNativeLabel_draw(CGUI_UINativeLabel* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->draw(self->component);
    }
}

void cgui_uiNativeLabel_refresh(CGUI_UINativeLabel* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->refresh(self->component);
    }
}

CGUI_Result cgui_uiNativeLabel_setGeometry(CGUI_UINativeLabel* self, CGUI_Rectangle geometry) {
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

CGUI_Result cgui_uiNativeLabel_setLayout(CGUI_UINativeLabel* self, CGUI_Layout layout) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Rectangle* cgui_uiNativeLabel_getGeometry(CGUI_UINativeLabel* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UILayout)) {
        return self->component->layoutImpl->getGeometry(self->component);
    }
    return NULL;
}

CGUI_Layout* cgui_uiNativeLabel_getLayout(CGUI_UINativeLabel* self) {
    return NULL;
}

bool cgui_uiNativeLabel_isVisible(CGUI_UINativeLabel* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isVisible(self->component);
    } else {
        return false;
    }
}

bool cgui_uiNativeLabel_isEnabled(CGUI_UINativeLabel* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isEnabled(self->component);
    } else {
        return false;
    }
}

CGUI_Result cgui_uiNativeLabel_setVisible(CGUI_UINativeLabel* self, bool visible) {
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

CGUI_Result cgui_uiNativeLabel_setEnabled(CGUI_UINativeLabel* self, bool enabled) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setEnabled(self->component, enabled);
        return self->window->setEnabled(self->window, enabled);
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

HWND cgui_uiNativeLabel_getWindowHandle(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeLabel* nativeWindow = (CGUI_UINativeLabel*)component->disposableImpl->upperLevel;
        return nativeWindow->window->getWindowHandle(nativeWindow->window);
    } else {
        return NULL;
    }
}

void cgui_uiNativeLabel_readyCallback(CGUI_UIComponent* component) {
    dbg_printf("uiNativeLabel ready: %s\n", component->name);
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeLabel* self = (CGUI_UINativeLabel*) component->disposableImpl->upperLevel;
        self->update(self);
    }
}

void cgui_uiNativeLabel_drawCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeLabel_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeLabel_refreshCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeLabel_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeLabel_destroyCallback(CGUI_UIComponent* component) {
    // note: the UIComponent and UIDisposable are freed by the parent.
    cgui_destroyUIDrawable(component->drawableImpl);
    cgui_destroyUILayout(component->layoutImpl);
    cgui_destroyUIState(component->stateImpl);
    cgui_destroyUIWin32(component->win32Impl);

    cgui_destroyUINativeLabel((CGUI_UINativeLabel*)component->disposableImpl->upperLevel);
}
