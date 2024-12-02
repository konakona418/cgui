//
// Created on: 2024/11/17, 11:17.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_GDI_H
#define CGUI_GDI_H

#include <windows.h>

#include "../util/common.h"
#include "../util/misc.h"
#include "../geo/layout.h"
#include "../geo/rect.h"

typedef enum TextAlignmentHorizontal {
    CGUI_TextAlignmentH_Left,
    CGUI_TextAlignmentH_Center,
    CGUI_TextAlignmentH_Right
} CGUI_TextAlignmentHorizontal;

typedef enum TextAlignmentVertical {
    CGUI_TextAlignmentV_Top,
    CGUI_TextAlignmentV_Center,
    CGUI_TextAlignmentV_Bottom
} CGUI_TextAlignmentVertical;

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

typedef struct ColorInner {
    int r;
    int g;
    int b;
} CGUI_ColorInner;

typedef struct Color {
    COLORREF rgb;

    CGUI_ColorInner _inner;
    bool transparent;
} CGUI_Color;

typedef struct GDIFontStyle {
    int fontSize;
    LPSTR fontName;
    CGUI_Color foregroundColor;
    CGUI_Color backgroundColor;

    CGUI_Color realBackgroundColor;
} CGUI_GDIFontStyle;

typedef struct GDITextContext {
    CGUI_GDIFontStyle fontStyle;
    CGUI_TextStyle textStyle;

    CGUI_TextAlignmentHorizontal alignHorizontal;
    CGUI_TextAlignmentVertical alignVertical;

    CGUI_TextOrientation orientation;

    bool multiLine;
} CGUI_GDITextContext;

CGUI_Color cgui_rgbaToColor(int r, int g, int b);

HBRUSH cgui_intoSolidBrush(CGUI_Color color);

CGUI_Color cgui_transparentColor();

HFONT cgui_createFont(CGUI_GDITextContext* fontCtx);

CGUI_Win32TAParam cgui_textAlignIntoGdi(CGUI_TextAlignmentHorizontal alignment);

CGUI_Win32DTParam cgui_textAlignIntoDrawText(CGUI_GDITextContext* fontCtx);

CGUI_Win32SSParam cgui_textAlignIntoStaticStyle(CGUI_GDITextContext* fontCtx);

CGUI_Win32ESParam cgui_textAlignIntoEditStyle(CGUI_GDITextContext* fontCtx);

CGUI_GDITextContext* cgui_createGdiTextContext();

CGUI_GDITextContext* cgui_createGdiTextContextFromInstance(CGUI_GDITextContext instance);

void cgui_destroyGdiTextContext(CGUI_GDITextContext* context);

void cgui_drawText(LPCSTR text, HWND hwnd, CGUI_GDITextContext* context, CGUI_Rectangle geometry);

#endif //CGUI_GDI_H
