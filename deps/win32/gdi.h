//
// Created on: 2024/11/17, 11:17.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_GDI_H
#define CGUI_GDI_H

#include <windows.h>

#include "../util/common.h"
#include "../geo/layout.h"
#include "../geo/rect.h"

typedef enum TextAlignment {
    CGUI_TextAlignment_Left,
    CGUI_TextAlignment_Center,
    CGUI_TextAlignment_Right
} CGUI_TextAlignment;

typedef enum TextOrientation {
    CGUI_TextOrientation_Horizontal,
    CGUI_TextOrientation_Vertical
} CGUI_TextOrientation;

typedef struct TextStyle {
    bool bold;
    bool italic;
    bool underline;
    bool strikeout;
} CGUI_TextStyle;

typedef struct Color {
    COLORREF rgb;
    bool transparent;
} CGUI_Color;

typedef struct GDIFontStyle {
    int fontSize;
    LPSTR fontName;
    CGUI_Color foregroundColor;
    CGUI_Color backgroundColor;
} CGUI_GDIFontStyle;

typedef struct GDITextContext {
    CGUI_GDIFontStyle fontStyle;
    CGUI_TextStyle textStyle;

    CGUI_TextAlignment alignment;
    CGUI_TextOrientation orientation;

    bool multiLine;
} CGUI_GDITextContext;

CGUI_Color cgui_rgbaToColor(int r, int g, int b);

CGUI_Color cgui_transparentColor();

HFONT cgui_createFont(CGUI_GDITextContext* fontCtx);

UINT cgui_textAlignIntoGdi(CGUI_TextAlignment alignment);

CGUI_GDITextContext* cgui_createGdiTextContext();

CGUI_GDITextContext* cgui_createGdiTextContextFromInstance(CGUI_GDITextContext instance);

void cgui_destroyGdiTextContext(CGUI_GDITextContext* context);

void cgui_drawText(LPCSTR text, HWND hwnd, CGUI_GDITextContext* context, CGUI_Rectangle geometry);

#endif //CGUI_GDI_H
