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

HFONT cgui_createFont(CGUI_GDITextContext* fontCtx) {
    LOGFONT lf = {0};
    lf.lfHeight = fontCtx->fontStyle.fontSize;
    lf.lfWidth = 0;

    if (fontCtx->orientation == CGUI_TextOrientation_Horizontal) {
        lf.lfEscapement = 0;
        lf.lfOrientation = 0;
    } else {
        lf.lfEscapement = 900;
        lf.lfOrientation = 900;
    }

    if (fontCtx->textStyle.bold) {
        lf.lfWeight = FW_BOLD;
    } else {
        lf.lfWeight = FW_NORMAL;
    }

    if (fontCtx->textStyle.italic) {
        lf.lfItalic = TRUE;
    } else {
        lf.lfItalic = FALSE;
    }

    if (fontCtx->textStyle.underline) {
        lf.lfUnderline = TRUE;
    } else {
        lf.lfUnderline = FALSE;
    }

    if (fontCtx->textStyle.strikeout) {
        lf.lfStrikeOut = TRUE;
    } else {
        lf.lfStrikeOut = FALSE;
    }

    lstrcpy(lf.lfFaceName, fontCtx->fontStyle.fontName);

    lf.lfQuality = PROOF_QUALITY;
    lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfPitchAndFamily = DEFAULT_PITCH;
    lf.lfCharSet = DEFAULT_CHARSET;

    return CreateFontIndirect(&lf);
}

UINT cgui_textAlignIntoGdi(CGUI_TextAlignment alignment) {
    switch (alignment) {
        case CGUI_TextAlignment_Center:
            return TA_CENTER;
        case CGUI_TextAlignment_Left:
            return TA_LEFT;
        case CGUI_TextAlignment_Right:
            return TA_RIGHT;
        default:
            return 0;
    }
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

CGUI_GDITextContext* cgui_createGdiTextContext() {
    CGUI_GDITextContext* context = (CGUI_GDITextContext*) malloc(sizeof(CGUI_GDITextContext));
    context->alignment = CGUI_TextAlignment_Left;
    context->fontStyle.backgroundColor = cgui_transparentColor();
    context->fontStyle.fontName = (char*) malloc(64);
    strcpy(context->fontStyle.fontName, "Arial");
    context->fontStyle.fontSize = 12;
    context->fontStyle.foregroundColor = cgui_rgbaToColor(0, 0, 0);
    context->multiLine = false;
    context->orientation = CGUI_TextOrientation_Horizontal;
    context->textStyle.bold = false;
    context->textStyle.italic = false;
    context->textStyle.strikeout = false;
    context->textStyle.underline = false;
    return context;
}

CGUI_GDITextContext* cgui_createGdiTextContextFromInstance(CGUI_GDITextContext instance) {
    CGUI_GDITextContext* context = (CGUI_GDITextContext*) malloc(sizeof(CGUI_GDITextContext));

    context->alignment = instance.alignment;
    context->fontStyle.backgroundColor = instance.fontStyle.backgroundColor;
    context->fontStyle.fontName = (char*) malloc(64);
    strcpy(context->fontStyle.fontName, instance.fontStyle.fontName);
    context->fontStyle.fontSize = instance.fontStyle.fontSize;
    context->fontStyle.foregroundColor = instance.fontStyle.foregroundColor;
    context->multiLine = instance.multiLine;
    context->orientation = instance.orientation;
    context->textStyle.bold = instance.textStyle.bold;
    context->textStyle.italic = instance.textStyle.italic;
    context->textStyle.strikeout = instance.textStyle.strikeout;
    context->textStyle.underline = instance.textStyle.underline;
    return context;
}

void cgui_destroyGdiTextContext(CGUI_GDITextContext* context) {
    // free(context->fontStyle.fontName);
    free(context);
}
