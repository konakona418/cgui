//
// Created on: 2024/11/17, 11:17.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "gdi.h"

CGUI_Color cgui_rgbaToColor(int r, int g, int b) {
    CGUI_Color color = {
        .rgb = RGB(r, g, b),
        ._inner = {
                .r = r,
                .g = g,
                .b = b,
        },
        .transparent = false
    };
    return color;
}

HBRUSH cgui_intoSolidBrush(CGUI_Color color) {
    return CreateSolidBrush(color.rgb);
}

CGUI_Color cgui_transparentColor() {
    CGUI_Color color = {
        .rgb = 0,
        ._inner = {
                .r = 0,
                .g = 0,
                .b = 0,
        },
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

CGUI_Win32TAParam cgui_textAlignIntoGdi(CGUI_TextAlignmentHorizontal alignment) {
    switch (alignment) {
        case CGUI_TextAlignmentH_Center:
            return TA_CENTER;
        case CGUI_TextAlignmentH_Left:
            return TA_LEFT;
        case CGUI_TextAlignmentH_Right:
            return TA_RIGHT;
        default:
            return 0;
    }
}

CGUI_Win32DTParam cgui_textAlignIntoDrawText(CGUI_GDITextContext* fontCtx) {
    CGUI_Win32DTParam aggregated = 0;

    switch (fontCtx->alignVertical) {
        case CGUI_TextAlignmentV_Bottom:
            aggregated |= DT_BOTTOM;
            break;
        case CGUI_TextAlignmentV_Center:
            aggregated |= DT_CENTER;
            break;
        case CGUI_TextAlignmentV_Top:
            aggregated |= DT_TOP;
            break;
        default:
            panic("Invalid text v-alignment");
            break;
    }

    switch (fontCtx->alignHorizontal) {
        case CGUI_TextAlignmentH_Center:
            aggregated |= DT_VCENTER;
            break;
        case CGUI_TextAlignmentH_Left:
            aggregated |= DT_LEFT;
            break;
        case CGUI_TextAlignmentH_Right:
            aggregated |= DT_RIGHT;
            break;
        default:
            panic("Invalid text h-alignment");
            break;
    }

    if (fontCtx->multiLine) {
        aggregated |= DT_WORDBREAK;
        dbg_printf("Multi-line text\n");
    } else {
        aggregated |= DT_SINGLELINE;
    }

    return aggregated;
}

CGUI_Win32SSParam cgui_textAlignIntoStaticStyle(CGUI_GDITextContext* fontCtx) {
    CGUI_Win32SSParam aggregated = 0;

    switch (fontCtx->alignHorizontal) {
        case CGUI_TextAlignmentH_Center:
            aggregated |= SS_CENTER;
            break;
        case CGUI_TextAlignmentH_Left:
            aggregated |= SS_LEFT;
            break;
        case CGUI_TextAlignmentH_Right:
            aggregated |= SS_RIGHT;
            break;
        default:
            panic("Invalid text h-alignment");
            break;
    }

    return aggregated;
}

CGUI_Win32ESParam cgui_textAlignIntoEditStyle(CGUI_GDITextContext* fontCtx) {
    CGUI_Win32ESParam aggregated = 0;

    switch (fontCtx->alignHorizontal) {
        case CGUI_TextAlignmentH_Center:
            aggregated |= ES_CENTER;
            break;
        case CGUI_TextAlignmentH_Left:
            aggregated |= ES_LEFT;
            break;
        case CGUI_TextAlignmentH_Right:
            aggregated |= ES_RIGHT;
            break;
        default:
            panic("Invalid text h-alignment");
            break;
    }

    return aggregated;
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
    context->alignHorizontal = CGUI_TextAlignmentH_Left;
    context->alignVertical = CGUI_TextAlignmentV_Top;
    context->fontStyle.backgroundColor = cgui_transparentColor();
    context->fontStyle.fontName = (char*) malloc(64);
    strcpy(context->fontStyle.fontName, "Arial");
    context->fontStyle.fontSize = 16;
    context->fontStyle.foregroundColor = cgui_rgbaToColor(0, 0, 0);
    context->fontStyle.realBackgroundColor = cgui_transparentColor();
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

    context->alignHorizontal = instance.alignHorizontal;
    context->alignVertical = instance.alignVertical;
    context->fontStyle.backgroundColor = instance.fontStyle.backgroundColor;
    context->fontStyle.fontName = (char*) malloc(64);
    strcpy(context->fontStyle.fontName, instance.fontStyle.fontName);
    context->fontStyle.fontSize = instance.fontStyle.fontSize;
    context->fontStyle.foregroundColor = instance.fontStyle.foregroundColor;
    context->fontStyle.realBackgroundColor = instance.fontStyle.realBackgroundColor;
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

void cgui_drawRect(HDC hdc, RECT rect, CGUI_GDIDrawRectContext context) {
    if (context.fill) {
        HBRUSH brush = cgui_intoSolidBrush(context.backgroundColor);
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    } else {
        HPEN pen = CreatePen(PS_SOLID, context.borderWidth, context.borderColor.rgb);
        SelectObject(hdc, pen);
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        DeleteObject(pen);
    }
}
