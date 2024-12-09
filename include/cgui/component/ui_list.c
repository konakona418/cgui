//
// Created on: 2024/11/29, 11:06.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_list.h"

#include "../util/error.h"

CGUI_UINativeListView*
cgui_createUINativeListViewFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    CGUI_UINativeListView* listView = (CGUI_UINativeListView*) malloc(sizeof(CGUI_UINativeListView));

    listView->selectionMode = CGUI_ListViewSelectionMode_Single;

    LPCSTR wndIdentifier = nativeWindow ? nativeWindow->wndIdentifier : "(anonymous)";
    LONG_PTR wndId = (LONG_PTR) (nativeWindow ? nativeWindow->getWindowHandle(nativeWindow) : 0);
    listView->component = cgui_createUIComponent(cgui_concatString("listView", wndIdentifier),
                                                 (LONG_PTR) internalId,
                                                 parent,
                                                 CGUI_Trait_UIComponent | CGUI_Trait_UILayout | CGUI_Trait_UIDrawable
                                                 | CGUI_Trait_UIState | CGUI_Trait_UIDisposable | CGUI_Trait_UIWin32);

    listView->component->layoutImpl = cgui_createUILayout();
    listView->component->drawableImpl = cgui_createUIDrawable(
            cgui_uiNativeListView_readyCallback,
            cgui_uiNativeListView_drawCallback,
            cgui_uiNativeListView_refreshCallback);
    listView->component->stateImpl = cgui_createUIState();
    listView->component->disposableImpl = cgui_createUIDisposable(listView, cgui_uiNativeListView_destroyCallback);
    listView->component->win32Impl = cgui_createUIWin32(cgui_uiNativeListView_getWindowHandle, internalId);

    listView->window = nativeWindow;

    listView->gdiTextContext = NULL;
    listView->_gdiRefreshFlag = false;

    listView->show = cgui_uiNativeListView_show;
    listView->hide = cgui_uiNativeListView_hide;
    listView->close = cgui_uiNativeListView_close;
    listView->update = cgui_uiNativeListView_update;
    listView->postMessage = cgui_uiNativeListView_postMessage;

    listView->setTextDisplay = cgui_uiNativeListView_setTextDisplay;
    listView->getTextDisplay = cgui_uiNativeListView_getTextDisplay;

    listView->appendItem = cgui_uiNativeListView_appendItem;
    listView->insertItem = cgui_uiNativeListView_insertItem;
    listView->removeItem = cgui_uiNativeListView_removeItem;
    listView->getSelectedItem = cgui_uiNativeListView_getSelectedItem;

    listView->addChild = cgui_uiNativeListView_addChild;
    listView->removeChild = cgui_uiNativeListView_removeChild;
    listView->removeChildById = cgui_uiNativeListView_removeChildById;
    listView->setComponentName = cgui_uiNativeListView_setComponentName;
    listView->getComponentName = cgui_uiNativeListView_getComponentName;
    listView->setEventHandler = cgui_uiNativeListView_setEventHandler;

    listView->draw = cgui_uiNativeListView_draw;
    listView->refresh = cgui_uiNativeListView_refresh;

    listView->getGeometry = cgui_uiNativeListView_getGeometry;
    listView->getLayout = cgui_uiNativeListView_getLayout;
    listView->getGeometry = cgui_uiNativeListView_getGeometry;
    listView->setLayout = cgui_uiNativeListView_setLayout;
    listView->setGeometry = cgui_uiNativeListView_setGeometry;

    listView->isEnabled = cgui_uiNativeListView_isEnabled;
    listView->isVisible = cgui_uiNativeListView_isVisible;
    listView->setEnabled = cgui_uiNativeListView_setEnabled;
    listView->setVisible = cgui_uiNativeListView_setVisible;

    return listView;
}

CGUI_UINativeListView* cgui_createUINativeListView(CGUI_UIComponent* parent, CGUI_InternalID internalId) {
    return cgui_createUINativeListViewFromWindow(NULL, parent, internalId);
}

void cgui_destroyUINativeListView(CGUI_UINativeListView* listView) {
    cgui_destroyWindowInstance(listView->window);
    cgui_destroyGdiTextContext(listView->gdiTextContext);
    free(listView);
}

CGUI_Result cgui_uiNativeListView_bindWindowInstance(CGUI_UINativeListView* self, CGUI_Window* nativeWindow) {
    if (self == NULL || nativeWindow == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    self->window = nativeWindow;

    LPCSTR wndIdentifier = nativeWindow->wndIdentifier;
    self->component->name = cgui_concatString("listView", wndIdentifier);
    self->component->id = (LONG_PTR) nativeWindow->getWindowHandle(nativeWindow);

    return create_ok(NULL);
}

void cgui_uiNativeListView_show(CGUI_UINativeListView* self) {
    self->update(self);
    self->window->show(self->window);
}

void cgui_uiNativeListView_hide(CGUI_UINativeListView* self) {
    self->window->hide(self->window);
}

void cgui_uiNativeListView_update(CGUI_UINativeListView* self) {
    self->window->update(self->window);
}

CGUI_Result cgui_uiNativeListView_close(CGUI_UINativeListView* self) {
    return self->window->close(self->window);
}

CGUI_Result
cgui_uiNativeListView_postMessage(CGUI_UINativeListView* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (isAsync) {
        return self->window->postMessageAsync(self->window, msg, wParam, lParam);
    } else {
        return self->window->postMessageSync(self->window, msg, wParam, lParam);
    }
}

void cgui_uiNativeListView_setTextDisplay(CGUI_UINativeListView* self, CGUI_GDITextContext* gdiTextContext) {
    self->gdiTextContext = gdiTextContext;
    self->_gdiRefreshFlag = true;

    CGUI_Win32SSParam ssParam = cgui_textAlignIntoStaticStyle(gdiTextContext);
    self->window->setWindowStyle(self->window, self->window->getWindowStyle(self->window) | ssParam);

    HFONT hFont = cgui_createFont(gdiTextContext);
    self->postMessage(self, false, WM_SETFONT, (WPARAM) hFont, 0);
    self->update(self); // this will signal repaint.
}

CGUI_GDITextContext cgui_uiNativeListView_getTextDisplay(CGUI_UINativeListView* self) {
    unwrap(create_err(CGUI_Error_NotImplemented()));
    return (CGUI_GDITextContext) {0};
}

long long int cgui_uiNativeListView_findItem(CGUI_UINativeListView* self, CGUI_ListViewItemSelector itemSelector) {
    if (itemSelector.selectorType == CGUI_ListViewItemSelectorType_Index) {
        return itemSelector.inner.idx;
    } else {
        return (LPARAM) unwrap(
                self->postMessage(self, false, LB_FINDSTRINGEXACT, -1, (LPARAM) itemSelector.inner.name));
    }
}

CGUI_Result cgui_uiNativeListView_appendItem(CGUI_UINativeListView* self, LPCSTR text) {
    return self->postMessage(self, false, LB_ADDSTRING, 0, (LPARAM) text);
}

CGUI_Result
cgui_uiNativeListView_insertItem(CGUI_UINativeListView* self, LPCSTR text, CGUI_ListViewItemSelector itemSelector) {
    if (itemSelector.selectorType == CGUI_ListViewItemSelectorType_Index) {
        return self->postMessage(self, false, LB_INSERTSTRING, itemSelector.inner.idx, (LPARAM) text);
    } else {
        return self->postMessage(self, false, LB_INSERTSTRING, cgui_uiNativeListView_findItem(self, itemSelector),
                                 (LPARAM) text);
    }
}

CGUI_Result cgui_uiNativeListView_removeItem(CGUI_UINativeListView* self, CGUI_ListViewItemSelector itemSelector) {
    if (itemSelector.selectorType == CGUI_ListViewItemSelectorType_Index) {
        return self->postMessage(self, false, LB_DELETESTRING, itemSelector.inner.idx, 0);
    } else {
        return self->postMessage(self, false, LB_DELETESTRING, cgui_uiNativeListView_findItem(self, itemSelector), 0);
    }
}

abstraction_leak CGUI_ListViewItems cgui_uiNativeListView_getSelectedItemLeaked(CGUI_UINativeListView* self) {
    int count = self->selectionMode != CGUI_ListViewSelectionMode_Single ?
                (int) (LPARAM) SendMessage(self->window->hwnd, LB_GETSELCOUNT, 0, 0) :
                1;
    CGUI_ListViewItems items = {
            .count = count,
            .items = (CGUI_ListViewItem*) malloc(sizeof(CGUI_ListViewItem) * count)
    };

    if (count > 1) {
        int* indices = (int*) malloc(sizeof(int) * count);
        SendMessage(self->window->hwnd, LB_GETSELITEMS, count, (LPARAM) indices);
        for (int i = 0; i < count; i++) {
            items.items[i].idx = indices[i];

            size_t bufferSize = (size_t) SendMessage(self->window->hwnd, LB_GETTEXTLEN, indices[i], 0);
            items.items[i].text = (char*) malloc(bufferSize + 1);
            SendMessage(self->window->hwnd, LB_GETTEXT, indices[i], (LPARAM) items.items[i].text);
        }
        free(indices);
    } else if (count == 1) {
        items.items[0].idx = (int) (LPARAM) unwrap(self->postMessage(self, false, LB_GETCURSEL, 0, 0));
        if (items.items[0].idx == -1) {
            items.count = 0;
            free(items.items);
        }

        size_t bufferSize = (size_t) SendMessage(self->window->hwnd, LB_GETTEXTLEN, items.items[0].idx, 0);
        items.items[0].text = (char*) malloc(bufferSize + 1);
        SendMessage(self->window->hwnd, LB_GETTEXT, items.items[0].idx, (LPARAM) items.items[0].text);
    }
    return items;
}

CGUI_ListViewItems cgui_uiNativeListView_getSelectedItem(CGUI_UINativeListView* self) {
    return cgui_uiNativeListView_getSelectedItemLeaked(self);
}

void cgui_uiNativeListView_addChild(CGUI_UINativeListView* self, CGUI_UIComponent* child) {
    self->component->addChild(self->component, child);
    self->update(self);
}

CGUI_UIComponent* cgui_uiNativeListView_removeChild(CGUI_UINativeListView* self, CGUI_UIComponent* child) {
    return self->component->removeChild(self->component, child);
}

CGUI_UIComponent* cgui_uiNativeListView_removeChildById(CGUI_UINativeListView* self, LONG_PTR id) {
    return self->component->removeChildById(self->component, id);
}

CGUI_Result cgui_uiNativeListView_setComponentName(CGUI_UINativeListView* self, LPCSTR name) {
    return self->component->setComponentName(self->component, name);
}

LPCSTR cgui_uiNativeListView_getComponentName(CGUI_UINativeListView* self) {
    return self->component->getComponentName(self->component);
}

void cgui_uiNativeListView_setEventHandler(CGUI_UINativeListView* self, CGUI_EventHandler* handler) {
    self->component->setEventHandler(self->component, handler);
}

void cgui_uiNativeListView_draw(CGUI_UINativeListView* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->draw(self->component);
    }
}

void cgui_uiNativeListView_refresh(CGUI_UINativeListView* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIDrawable)) {
        self->component->drawableImpl->refresh(self->component);
    }
}

CGUI_Result cgui_uiNativeListView_setGeometry(CGUI_UINativeListView* self, CGUI_Rectangle geometry) {
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

CGUI_Result cgui_uiNativeListView_setLayout(CGUI_UINativeListView* self, CGUI_Layout layout) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Rectangle* cgui_uiNativeListView_getGeometry(CGUI_UINativeListView* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UILayout)) {
        return self->component->layoutImpl->getGeometry(self->component);
    }
    return NULL;
}

CGUI_Layout* cgui_uiNativeListView_getLayout(CGUI_UINativeListView* self) {
    return NULL;
}

bool cgui_uiNativeListView_isVisible(CGUI_UINativeListView* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isVisible(self->component);
    } else {
        return false;
    }
}

bool cgui_uiNativeListView_isEnabled(CGUI_UINativeListView* self) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        return self->component->stateImpl->isEnabled(self->component);
    } else {
        return false;
    }
}

CGUI_Result cgui_uiNativeListView_setVisible(CGUI_UINativeListView* self, bool visible) {
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

CGUI_Result cgui_uiNativeListView_setEnabled(CGUI_UINativeListView* self, bool enabled) {
    if (impl(self->component->implFlag, CGUI_Trait_UIState)) {
        self->component->stateImpl->setEnabled(self->component, enabled);
        return self->window->setEnabled(self->window, enabled);
    } else {
        return create_err(CGUI_Error_NotImplemented());
    }
}

HWND cgui_uiNativeListView_getWindowHandle(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeListView* nativeWindow = (CGUI_UINativeListView*) component->disposableImpl->upperLevel;
        return nativeWindow->window->getWindowHandle(nativeWindow->window);
    } else {
        return NULL;
    }
}

void cgui_uiNativeListView_readyCallback(CGUI_UIComponent* component) {
    dbg_printf("uiNativeLabel ready: %s\n", component->name);
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        CGUI_UINativeListView* self = (CGUI_UINativeListView*) component->disposableImpl->upperLevel;
        self->update(self);
    }
}

void cgui_uiNativeListView_drawCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeListView_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeListView_refreshCallback(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        if (component->disposableImpl != NULL) {
            //cgui_uiNativeListView_show(component->disposableImpl->upperLevel);
        }
    }
}

void cgui_uiNativeListView_destroyCallback(CGUI_UIComponent* component) {
    // note: the UIComponent and UIDisposable are freed by the parent.
    cgui_destroyUIDrawable(component->drawableImpl);
    cgui_destroyUILayout(component->layoutImpl);
    cgui_destroyUIState(component->stateImpl);
    cgui_destroyUIWin32(component->win32Impl);

    cgui_destroyUINativeListView((CGUI_UINativeListView*) component->disposableImpl->upperLevel);
}