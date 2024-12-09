//
// Created on: 2024/11/8, 11:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <stdlib.h>
#include "handler.h"
#include "../component/trait.h"

#include "../component/ui.h"
#include "../component/ui_label.h"
#include "../component/ui_button.h"
#include "../component/ui_textbox.h"
#include "../component/ui_list.h"

void cgui_eventHandler_defaultOnPaint(CGUI_EventArgs args) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) args.component;
    if (impl(component->implFlag, CGUI_Trait_UIDrawable)) {
        if (component->drawableImpl->refresh) {
            component->drawableImpl->refresh(component);
        }
    }
}

CGUI_EventHandler* cgui_createEventHandler(CGUI_LocalHandlerContext localHandlerCtx, void* componentBase) {
    CGUI_EventHandler* handler = (CGUI_EventHandler*) malloc(sizeof(CGUI_EventHandler));
    handler->component = componentBase;

    handler->localHandler = localHandlerCtx.localHandler;
    handler->handlerFlag = localHandlerCtx.handlerFlag;
    handler->localHandlerDestructor = localHandlerCtx.destructor;

    handler->handleEvent = cgui_eventHandler_handleEvent;
    handler->setComponent = cgui_eventHandler_setComponent;

    return handler;
}

void cgui_eventHandler_setComponent(CGUI_EventHandler* handler, void* component) {
    handler->component = component;
}

void cgui_destroyEventHandler(CGUI_EventHandler* handler) {
    if (handler->localHandlerDestructor && handler->localHandler) {
        handler->localHandlerDestructor(handler->localHandler);
    }
    free(handler);
}

int cgui_routeToLocalHandler(CGUI_EventHandler* handler, void* localHandler, HWND hwnd, UINT msg, WPARAM wParam,
                             LPARAM lParam) {
    if (handle(handler->handlerFlag, CGUI_LocalHandler_WindowRoot)) {
        return into(CGUI_WindowHandler, handler->localHandler)->handleEventLocal(localHandler, handler, hwnd, msg,
                                                                                 wParam, lParam);
    } else if (handle(handler->handlerFlag, CGUI_LocalHandler_Button)) {
        return into(CGUI_ButtonHandler, handler->localHandler)->handleEventLocal(localHandler, handler, hwnd, msg,
                                                                                 wParam, lParam);
    } else if (handle(handler->handlerFlag, CGUI_LocalHandler_TextBox)) {
        return into(CGUI_TextBoxHandler, handler->localHandler)->handleEventLocal(localHandler, handler, hwnd, msg,
                                                                                  wParam, lParam);
    } else if (handle(handler->handlerFlag, CGUI_LocalHandler_Label)) {
        return into(CGUI_LabelHandler, handler->localHandler)->handleEventLocal(localHandler, handler, hwnd, msg,
                                                                                wParam, lParam);
    } else if (handle(handler->handlerFlag, CGUI_LocalHandler_ListView)) {
        return into(CGUI_ListViewHandler, handler->localHandler)->handleEventLocal(localHandler, handler, hwnd, msg,
                                                                                   wParam, lParam);
    } else {
        panic("Invalid handler type.");
    }
}

#define eq_wm(_msg) eq_any((int) _msg, 7, WM_CTLCOLORBTN, WM_CTLCOLOREDIT, WM_CTLCOLORMSGBOX, \
WM_CTLCOLORLISTBOX, WM_CTLCOLORSCROLLBAR, WM_CTLCOLORSTATIC, WM_CTLCOLORDLG)

long long int
cgui_eventHandler_handleEvent(CGUI_EventHandler* self, CGUI_ComponentQuery query, UINT msg, WPARAM wParam,
                              LPARAM lParam, CGUI_ApplicationMessageCallback callback) {
    if (msg == WM_COMMAND) {
        HWND hComponent = (HWND) lParam;
        if (likely(query.hwnd != hComponent)) {
            if (unlikely(hComponent == NULL)) {
                // todo: [IMPORTANT] when this value is NULL, it usually means that this message originates from a menu option.
                // add something to handle this.
            }

            if (likely(IsWindow(hComponent))) {
                int id = LOWORD(wParam);
                CGUI_ComponentQuery queryLocal = cgui_createComponentQuery(hComponent, id);

                return callback(queryLocal, msg, wParam, lParam);
            } else {
                printf("WM_COMMAND: Child component not a window. Ignoring.\n");
            }
        } else {
            // dbg_printf("WM_COMMAND: child component hwnd eq hwnd, routing.\n");
            return cgui_routeToLocalHandler(self, self->localHandler, query.hwnd, msg, wParam, lParam);
        }
    } else if (eq_wm(msg)) {
        HWND hComponent = (HWND) lParam;
        if (likely(query.hwnd != hComponent)) {
            if (unlikely(hComponent == NULL)) {

            }

            if (likely(IsWindow(hComponent))) {
                int id = GetDlgCtrlID(hComponent);
                CGUI_ComponentQuery queryLocal = cgui_createComponentQuery(hComponent, id);

                return callback(queryLocal, msg, wParam, lParam);
            }
        } else {
            // if the query hwnd is the same as the hwnd of the message, then send the message to the local handler.
            return cgui_routeToLocalHandler(self, self->localHandler, query.hwnd, msg, wParam, lParam);
        }
    }
    return cgui_routeToLocalHandler(self, self->localHandler, query.hwnd, msg, wParam, lParam);
}

CGUI_WindowHandler* cgui_createWindowHandler() {
    CGUI_WindowHandler* handler = malloc(sizeof(CGUI_WindowHandler));

    handler->onCreate = NULL;
    handler->onDestroy = NULL;
    handler->onClose = NULL;

    handler->onPaint = cgui_eventHandler_defaultOnPaint;

    handler->onSize = NULL;
    handler->onFocus = NULL;

    handler->onMouseDown = NULL;
    handler->onMouseUp = NULL;
    handler->onMouseMove = NULL;
    handler->onMouseHover = NULL;
    handler->onMouseLeave = NULL;
    handler->onMouseWheel = NULL;

    handler->onClick = NULL;
    handler->onDoubleClick = NULL;
    handler->onKeyDown = NULL;
    handler->onKeyUp = NULL;

    handler->handleEventLocal = cgui_windowHandler_handleEventLocal;

    return handler;
}

void cgui_destroyWindowHandler(void* handler) {
    free((CGUI_WindowHandler*) handler);
}

long long int cgui_windowHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                                  WPARAM wParam, LPARAM lParam) {
    CGUI_WindowHandler* self = (CGUI_WindowHandler*) pSelf;
    CGUI_EventArgs args = cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam);
    switch (msg) {
        case WM_CREATE:
            if (self->onCreate) {
                self->onCreate(args);
            }
            break;
        case WM_DESTROY:
            if (self->onDestroy) {
                self->onDestroy(args);
            }
            break;
        case WM_CLOSE:
            if (self->onClose) {
                self->onClose(args);
            }
            break;
        case WM_PAINT:
            if (self->onPaint) {
                self->onPaint(args);
            }
            return DefWindowProc(hwnd, msg, wParam, lParam);
            break;
        case WM_SIZE:
            if (self->onSize) {
                self->onSize(args);
            }
            //InvalidateRect(hwnd, NULL, TRUE);
            //UpdateWindow(hwnd);
            return DefWindowProc(hwnd, msg, wParam, lParam);
        case WM_SETFOCUS:
            if (self->onFocus) {
                self->onFocus(args);
            }
            break;
        case WM_MOUSEMOVE:
            if (self->onMouseMove) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onMouseMove(mouseArgs);
            }
            break;
        case WM_LBUTTONDOWN:
            if (self->onMouseDown) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button.state != LeftButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button.state = LeftButton;
                self->onMouseDown(mouseArgs);
            }
            break;
        case WM_LBUTTONUP:
            if (self->onMouseUp) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button.state != LeftButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button.state = LeftButton;
                self->onMouseUp(mouseArgs);
            }
            break;
        case WM_RBUTTONDOWN:
            if (self->onMouseDown) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button.state != RightButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button.state = RightButton;
                self->onMouseDown(mouseArgs);
            }
            break;
        case WM_RBUTTONUP:
            if (self->onMouseUp) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                if (mouseArgs.button.state != RightButton) printf("WM_MOUSEMOVE: button code mismatch.\n");
                mouseArgs.button.state = RightButton;
                self->onMouseUp(mouseArgs);
            }
            break;
        case WM_MOUSEWHEEL:
            if (self->onMouseWheel) {
                CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onMouseWheel(mouseArgs);
            }
            break;
        case WM_KEYDOWN:
            if (self->onKeyDown) {
                CGUI_KeyEventArgs keyArgs = cgui_createKeyEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onKeyDown(keyArgs);
            }
            break;
        case WM_KEYUP:
            if (self->onKeyUp) {
                CGUI_KeyEventArgs keyArgs = cgui_createKeyEventArgs(parent->component, hwnd, msg, wParam, lParam);
                self->onKeyUp(keyArgs);
            }
            break;
        case WM_COMMAND:
            panic("WM_COMMAND: Unhandled command message!");
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

long long int cgui_labelHandler_defaultOnGdiReady(CGUI_GdiReadyEventArgs args) {
    CGUI_UIComponent* component = args.base.component;
    if (!impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        panic("Fatal! Not a valid NativeLabel component!");
    }
    CGUI_UINativeLabel* label = (CGUI_UINativeLabel*) component->disposableImpl->upperLevel;

    if (label->gdiTextContext == NULL) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }

    HDC hdc = args.hdc;
    HWND hwnd = args.hwnd;

    CGUI_GDITextContext* drawCtx = label->gdiTextContext;

    SetTextColor(hdc, drawCtx->fontStyle.foregroundColor.rgb);
    // SetTextAlign(hdc, cgui_textAlignIntoGdi(drawCtx->alignment));

    if (drawCtx->fontStyle.backgroundColor.transparent) {
        SetBkMode(hdc, TRANSPARENT);
    } else {
        SetBkColor(hdc, drawCtx->fontStyle.backgroundColor.rgb);
    }
    label->_gdiRefreshFlag = false;

    if (drawCtx->fontStyle.realBackgroundColor.transparent) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    } else {
        return (LPARAM) CreateSolidBrush(drawCtx->fontStyle.realBackgroundColor.rgb);
    }
}

CGUI_LabelHandler* cgui_createLabelHandler() {
    CGUI_LabelHandler* handler = (CGUI_LabelHandler*) malloc(sizeof(CGUI_LabelHandler));
    handler->onGdiReady = cgui_labelHandler_defaultOnGdiReady;
    handler->handleEventLocal = cgui_labelHandler_handleEventLocal;
    return handler;
}


void cgui_destroyLabelHandler(void* handler) {
    free((CGUI_LabelHandler*) handler);
}

long long int cgui_labelHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                                 WPARAM wParam, LPARAM lParam) {
    CGUI_LabelHandler* self = (CGUI_LabelHandler*) pSelf;
    if (eq_wm(msg)) {
        CGUI_GdiReadyEventArgs args = cgui_createGdiReadyEventArgs(parent->component, hwnd, msg, wParam, lParam);
        if (self->onGdiReady) {
            return self->onGdiReady(args);
        }
        return (LPARAM) GetStockObject(NULL_BRUSH);
    } else {
        //InvalidateRect(hwnd, NULL, TRUE);
        //UpdateWindow(hwnd);
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

long long int cgui_buttonHandler_defaultOnGdiReady(CGUI_GdiReadyEventArgs args) {
    CGUI_UIComponent* component = args.base.component;
    if (!impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        panic("Fatal! Not a valid NativeLabel component!");
    }
    CGUI_UINativeButton* button = (CGUI_UINativeButton*) component->disposableImpl->upperLevel;

    if (button->gdiTextContext == NULL) {
        //dbg_printf("onGdiReady, not painted.\n");
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }
    //dbg_printf("onGdiReady, painted.\n");

    HDC hdc = args.hdc;
    HWND hwnd = args.hwnd;

    CGUI_GDITextContext* drawCtx = button->gdiTextContext;

    /**
     * Default button specific draw implementation.
     * As the Win32 native button does not support custom styles,
     * we need to overwrite its original draw implementation. */
    if (button->getButtonStyle(button) == CGUI_ButtonStyle_Default) {
        RECT rect;
        LPCSTR text;

        text = button->getText(button);
        GetClientRect(hwnd, &rect);
        SetTextColor(hdc, drawCtx->fontStyle.foregroundColor.rgb);
        if (drawCtx->fontStyle.backgroundColor.transparent) {
            SetBkMode(hdc, TRANSPARENT);
        } else {
            SetBkColor(hdc, drawCtx->fontStyle.backgroundColor.rgb);
        }
        HFONT hFont = cgui_createFont(drawCtx);
        //SetTextAlign(hdc, cgui_textAlignIntoGdi(drawCtx->alignment));
        SelectObject(hdc, hFont);

        CGUI_Win32DTParam dtParam = cgui_textAlignIntoDrawText(drawCtx);
        DrawText(hdc, text, cgui_cStringLen(text), &rect, dtParam);

        if (drawCtx->fontStyle.realBackgroundColor.transparent) {
            return (LPARAM) GetStockObject(NULL_BRUSH);
        } else {
            return (LPARAM) cgui_intoSolidBrush(drawCtx->fontStyle.realBackgroundColor);
        }
    }

    SetTextColor(hdc, drawCtx->fontStyle.foregroundColor.rgb);
    //SetTextAlign(hdc, cgui_textAlignIntoGdi(drawCtx->alignment));

    if (drawCtx->fontStyle.backgroundColor.transparent) {
        SetBkMode(hdc, TRANSPARENT);
    } else {
        SetBkColor(hdc, drawCtx->fontStyle.backgroundColor.rgb);
    }

    // button->update(button);

    button->_gdiRefreshFlag = false;

    if (drawCtx->fontStyle.realBackgroundColor.transparent) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    } else {
        return (LPARAM) cgui_intoSolidBrush(drawCtx->fontStyle.realBackgroundColor);
    }
}

CGUI_ButtonHandler* cgui_createButtonHandler() {
    CGUI_ButtonHandler* handler = (CGUI_ButtonHandler*) malloc(sizeof(CGUI_ButtonHandler));

    handler->onClick = NULL;
    handler->onDoubleClick = NULL;
    handler->onMouseDown = NULL;
    handler->onMouseUp = NULL;
    handler->onFocus = NULL;
    handler->onDefocus = NULL;

    handler->onGdiReady = cgui_buttonHandler_defaultOnGdiReady;

    handler->handleEventLocal = cgui_buttonHandler_handleEventLocal;
    return handler;
}

void cgui_destroyButtonHandler(void* handler) {
    free((CGUI_ButtonHandler*) handler);
}

long long cgui_buttonHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                              WPARAM wParam, LPARAM lParam) {
    CGUI_ButtonHandler* self = (CGUI_ButtonHandler*) pSelf;

    if (eq_wm(msg)) {
        CGUI_GdiReadyEventArgs args = cgui_createGdiReadyEventArgs(parent->component, hwnd, msg, wParam, lParam);
        if (self->onGdiReady) {
            return self->onGdiReady(args);
        }
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }

    int bnMessage = HIWORD(wParam);
    CGUI_MouseEventArgs mouseArgs = cgui_createMouseEventArgs(parent->component, hwnd, msg, wParam, lParam);

    switch (bnMessage) {
        case BN_CLICKED:
            if (self->onClick) {
                self->onClick(mouseArgs);
            }
            break;
        case BN_DBLCLK:
            if (self->onDoubleClick) {
                self->onDoubleClick(mouseArgs);
            }
            break;
        case BN_SETFOCUS:
            if (self->onFocus) {
                self->onFocus(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        case BN_KILLFOCUS:
            if (self->onDefocus) {
                self->onDefocus(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        default:
            break;
    }

    //InvalidateRect(hwnd, NULL, TRUE);
    //UpdateWindow(hwnd);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

long long int cgui_textBoxHandler_defaultOnGdiReady(CGUI_GdiReadyEventArgs args) {
    CGUI_UIComponent* component = args.base.component;
    if (!impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        panic("Fatal! Not a valid NativeLabel component!");
    }
    CGUI_UINativeTextBox* textBox = (CGUI_UINativeTextBox*) component->disposableImpl->upperLevel;

    if (textBox->gdiTextContext == NULL) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }

    HDC hdc = args.hdc;

    CGUI_GDITextContext* drawCtx = textBox->gdiTextContext;

    SetTextColor(hdc, drawCtx->fontStyle.foregroundColor.rgb);

    if (drawCtx->fontStyle.backgroundColor.transparent) {
        SetBkMode(hdc, TRANSPARENT);
    } else {
        SetBkColor(hdc, drawCtx->fontStyle.backgroundColor.rgb);
    }
    textBox->_gdiRefreshFlag = false;

    if (drawCtx->fontStyle.realBackgroundColor.transparent) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    } else {
        return (LPARAM) CreateSolidBrush(drawCtx->fontStyle.realBackgroundColor.rgb);
    }
}

CGUI_TextBoxHandler* cgui_createTextBoxHandler() {
    CGUI_TextBoxHandler* handler = (CGUI_TextBoxHandler*) malloc(sizeof(CGUI_TextBoxHandler));

    handler->onTextChanged = NULL;
    handler->onKeyDown = NULL;
    handler->onKeyUp = NULL;
    handler->onMouseDown = NULL;
    handler->onMouseUp = NULL;
    handler->onDefocus = NULL;
    handler->onFocus = NULL;

    handler->onGdiReady = cgui_textBoxHandler_defaultOnGdiReady;

    handler->handleEventLocal = cgui_textBoxHandler_handleEventLocal;
    return handler;
}

void cgui_destroyTextBoxHandler(void* handler) {
    free((CGUI_TextBoxHandler*) handler);
}

long long int cgui_textBoxHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                                   WPARAM wParam, LPARAM lParam) {
    CGUI_TextBoxHandler* self = (CGUI_TextBoxHandler*) pSelf;

    if (eq_wm(msg)) {
        CGUI_GdiReadyEventArgs args = cgui_createGdiReadyEventArgs(parent->component, hwnd, msg, wParam, lParam);
        if (self->onGdiReady) {
            return self->onGdiReady(args);
        }
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }

    int notificationCode = HIWORD(wParam);
    switch (notificationCode) {
        case EN_CHANGE:
            if (self->onTextChanged) {
                self->onTextChanged(cgui_createTextBoxEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        case EN_SETFOCUS:
            if (self->onFocus) {
                dbg_printf("onFocus\n");
                self->onFocus(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        case EN_KILLFOCUS:
            if (self->onDefocus) {
                self->onDefocus(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        default:
            break;
    }

    //InvalidateRect(hwnd, NULL, TRUE);
    //UpdateWindow(hwnd);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

long long int cgui_listViewHandler_defaultOnGdiReady(CGUI_GdiReadyEventArgs args) {
    CGUI_UIComponent* component = args.base.component;
    if (!impl(component->implFlag, CGUI_Trait_UIDisposable)) {
        panic("Fatal! Not a valid NativeLabel component!");
    }
    CGUI_UINativeListView* listView = (CGUI_UINativeListView*) component->disposableImpl->upperLevel;
    if (listView->gdiTextContext == NULL) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }

    HDC hdc = args.hdc;

    CGUI_GDITextContext* drawCtx = listView->gdiTextContext;

    SetTextColor(hdc, drawCtx->fontStyle.foregroundColor.rgb);

    if (drawCtx->fontStyle.backgroundColor.transparent) {
        SetBkMode(hdc, TRANSPARENT);
    } else {
        SetBkColor(hdc, drawCtx->fontStyle.backgroundColor.rgb);
    }
    listView->_gdiRefreshFlag = false;

    if (drawCtx->fontStyle.realBackgroundColor.transparent) {
        return (LPARAM) GetStockObject(NULL_BRUSH);
    } else {
        return (LPARAM) CreateSolidBrush(drawCtx->fontStyle.realBackgroundColor.rgb);
    }
}

CGUI_ListViewHandler* cgui_createListViewHandler() {
    CGUI_ListViewHandler* handler = (CGUI_ListViewHandler*) malloc(sizeof(CGUI_ListViewHandler));

    handler->onItemSelected = NULL;
    handler->onItemDoubleClicked = NULL;
    handler->onMouseDown = NULL;
    handler->onMouseUp = NULL;
    handler->onMouseWheel = NULL;
    handler->onDefocus = NULL;
    handler->onFocus = NULL;

    handler->onGdiReady = cgui_listViewHandler_defaultOnGdiReady;

    handler->handleEventLocal = cgui_listViewHandler_handleEventLocal;
    return handler;
}

void cgui_destroyListViewHandler(void* handler) {
    free((CGUI_ListViewHandler*) handler);
}

long long int cgui_listViewHandler_handleEventLocal(void* pSelf, CGUI_EventHandler* parent, HWND hwnd, UINT msg,
                                                    WPARAM wParam, LPARAM lParam) {
    CGUI_ListViewHandler* self = (CGUI_ListViewHandler*) pSelf;

    if (eq_wm(msg)) {
        CGUI_GdiReadyEventArgs args = cgui_createGdiReadyEventArgs(parent->component, hwnd, msg, wParam, lParam);
        if (self->onGdiReady) {
            return self->onGdiReady(args);
        }
        return (LPARAM) GetStockObject(NULL_BRUSH);
    }

    int code = HIWORD(wParam);
    switch (code) {
        case LBN_SELCHANGE:
            if (self->onItemSelected) {
                CGUI_EventArgs base = cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam);
                CGUI_UINativeListView* listView = (CGUI_UINativeListView*) ((CGUI_UIComponent*) (base.component))->disposableImpl->upperLevel;

                CGUI_ListViewItemAcquisitionHandle handle = cgui_createListViewItemAcquisitionHandle(
                        (CGUI_ListViewItems (*)(void*)) listView->getSelectedItem, listView);
                self->onItemSelected(cgui_createListViewSelectedEventArgs(base, handle));
            }
            break;
        case LBN_DBLCLK:
            if (self->onItemDoubleClicked) {
                self->onItemDoubleClicked(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        case LBN_SETFOCUS:
            if (self->onFocus) {
                self->onFocus(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        case LBN_KILLFOCUS:
            if (self->onDefocus) {
                self->onDefocus(cgui_createEventArgs(parent->component, hwnd, msg, wParam, lParam));
            }
            break;
        default:
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

CGUI_LocalHandlerContext
cgui_createLocalHandlerContext(void* localHandler, LocalHandlerFlag handlerFlag, LocalHandlerDestructor destructor) {
    return (CGUI_LocalHandlerContext) {
            .localHandler = localHandler,
            .handlerFlag = handlerFlag,
            .destructor = destructor
    };
}
