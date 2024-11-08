//
// Created on: 2024/11/8, 11:23.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_LAYOUT_H
#define CGUI_LAYOUT_H

typedef struct Layout CGUI_Layout;

typedef struct Margin CGUI_Margin;

typedef struct Padding CGUI_Padding;

typedef enum Alignment CGUI_Alignment;

typedef enum Anchor CGUI_Anchor;

typedef enum Direction CGUI_Direction;

typedef struct Margin {
    int left;
    int top;
    int right;
    int bottom;
} CGUI_Margin;

typedef struct Padding {
    int left;
    int top;
    int right;
    int bottom;
} CGUI_Padding;

typedef enum Alignment {
    CGUI_Alignment_Left,
    CGUI_Alignment_Center,
    CGUI_Alignment_Right,
} CGUI_Alignment;

typedef enum Anchor {
    CGUI_Anchor_Left,
    CGUI_Anchor_Top,
    CGUI_Anchor_Right,
    CGUI_Anchor_Bottom,
    CGUI_Anchor_Center,
    CGUI_Anchor_BottomLeft,
    CGUI_Anchor_BottomRight,
    CGUI_Anchor_TopLeft,
    CGUI_Anchor_TopRight,
} CGUI_Anchor;

typedef enum Direction {
    CGUI_Direction_Horizontal,
    CGUI_Direction_Vertical
} CGUI_Direction;

typedef struct Layout {
    CGUI_Margin margin;
    CGUI_Padding padding;
    CGUI_Alignment alignment;
    CGUI_Direction direction;
    CGUI_Anchor anchor;
} CGUI_Layout;


#endif //CGUI_LAYOUT_H
