//
// Created on: 2024/11/12, 10:35.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "args.h"

CGUI_EventArgs cgui_createEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    CGUI_EventArgs args = {
            .argc = 0,
            .argv = NULL,
            .component = component,
            .hwnd = hwnd,
            .lParam = lParam,
            .wParam = wParam,
            .msg = msg,
    };
    return args;
}

CGUI_MouseButton cgui_asMouseButton(WPARAM wParam) {
    CGUI_MouseButtonState mouseButtonState;
    if (wParam & MK_LBUTTON) {
        mouseButtonState = LeftButton;
    } else if (wParam & MK_MBUTTON) {
        mouseButtonState = MiddleButton;
    } else if (wParam & MK_RBUTTON) {
        mouseButtonState = RightButton;
    } else if (wParam & MK_XBUTTON1) {
        mouseButtonState = XButton1;
    } else if (wParam & MK_XBUTTON2) {
        mouseButtonState = XButton2;
    } else {
        //printf("Invalid mouse button state.\n");
        mouseButtonState = LeftButton;
    }

    CGUI_MouseButtonKeyState keyState;
    keyState.isCtrlDown = (wParam & MK_CONTROL) != 0;
    keyState.isShiftDown = (wParam & MK_SHIFT) != 0;

    return (CGUI_MouseButton) {
            .keyState = keyState,
            .state = mouseButtonState
    };
}

CGUI_MouseEventArgs cgui_createMouseEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // todo: remains to be checked
    return (CGUI_MouseEventArgs) {
            .base = cgui_createEventArgs(component, hwnd, msg, wParam, lParam),
            .button = cgui_asMouseButton(wParam),
            .point = {
                    .x = (short) (LOWORD(lParam)),
                    .y = (short) (HIWORD(lParam)),
            },
    };
}


CGUI_KeyEventArgs cgui_createKeyEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    // todo: remains to be checked
    return (CGUI_KeyEventArgs) {
            .base = cgui_createEventArgs(component, hwnd, msg, wParam, lParam),
            .keyCode = (UINT) wParam,
            .keyState = (CGUI_KeyState) (lParam & 0x000F),
    };
}

CGUI_TextBoxAcquisitionHandle cgui_createTextBoxAcquisitionHandle(HWND hwnd, UINT len) {
    return (CGUI_TextBoxAcquisitionHandle) {
            .hwnd = hwnd,
            .textLength = len,
            .acquire = cgui_textBoxAcquisitionHandle_acquire,
    };
}

abstraction_leak void cgui_textBoxAcquisitionHandle_acquire(CGUI_TextBoxAcquisitionHandle* self, char** buffer) {
    *buffer = (char*) malloc(sizeof(char) * (self->textLength + 1));
    GetWindowText(self->hwnd, *buffer, (int) self->textLength + 1);
}

CGUI_TextBoxEventArgs cgui_createTextBoxEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    int len = GetWindowTextLength(hwnd);
    CGUI_TextBoxEventArgs eventArgs = {
            .base = cgui_createEventArgs(component, hwnd, msg, wParam, lParam),
            .acquisitionHandle = cgui_createTextBoxAcquisitionHandle(hwnd, len),
            .textLength = len,
    };
    return eventArgs;
}

CGUI_GdiReadyEventArgs
cgui_createGdiReadyEventArgs(void* component, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return (CGUI_GdiReadyEventArgs) {
            .base = cgui_createEventArgs(component, hwnd, msg, wParam, lParam),
            .hdc = (HDC) wParam,
            .hwnd = (HWND) lParam,
    };
}

CGUI_ListViewItemAcquisitionHandle
cgui_createListViewItemAcquisitionHandle(CGUI_ListViewItems (* _inner)(void* pSelf), void* pSelf) {
    return (CGUI_ListViewItemAcquisitionHandle) {
            .pSelf = pSelf,
            ._inner = _inner,
            .acquire = cgui_listViewItemAcquisitionHandle_acquire,
    };
}

CGUI_ListViewItems cgui_listViewItemAcquisitionHandle_acquire(CGUI_ListViewItemAcquisitionHandle* self) {
    return self->_inner(self->pSelf);
}

CGUI_ListViewSelectedEventArgs
cgui_createListViewSelectedEventArgs(CGUI_EventArgs base, CGUI_ListViewItemAcquisitionHandle handle) {
    return (CGUI_ListViewSelectedEventArgs) {
            .base = base,
            .acquisitionHandle = handle,
    };
}