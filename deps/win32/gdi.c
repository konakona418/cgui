//
// Created on: 2024/11/17, 11:17.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "gdi.h"

CGUI_Color cgui_rgbaToColor(int r, int g, int b) {
    CGUI_Color color = {
        .rgb = RGB(r, g, b),
        .transparent = false
    };
    return color;
}

CGUI_Color cgui_transparentColor() {
    CGUI_Color color = {
        .rgb = 0,
        .transparent = true
    };
    return color;
}

void cgui_drawText(LPCSTR text, HWND hwnd, CGUI_GDITextContext* context, CGUI_Rectangle geometry) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    HDC hdc = ps.hdc;

    SetTextColor(hdc, context->fontStyle.foregroundColor.rgb);
    if (context->fontStyle.backgroundColor.transparent) {
        SetBkMode(hdc, TRANSPARENT);
    } else {
        SetBkColor(hdc, context->fontStyle.backgroundColor.rgb);
    }


    EndPaint(hwnd, &ps);
}
