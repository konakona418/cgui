//
// Created on: 2024/11/5, 23:45.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "rect.h"

CGUI_Rectangle cgui_createRectangle(int x, int y, int width, int height) {
    CGUI_Rectangle rect;
    rect.x = x;
    rect.y = y;
    rect.width = width;
    rect.height = height;
    return rect;
}