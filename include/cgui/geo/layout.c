//
// Created on: 2024/11/8, 11:23.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "layout.h"

CGUI_Margin cgui_margin(int left, int top, int right, int bottom) {
    return (CGUI_Margin) {
            .left = left,
            .top = top,
            .right = right,
            .bottom = bottom
    };
}

CGUI_Padding cgui_padding(int left, int top, int right, int bottom) {
    return (CGUI_Padding) {
            .left = left,
            .top = top,
            .right = right,
            .bottom = bottom
    };
}
