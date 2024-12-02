//
// Created on: 2024/11/5, 23:45.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_RECT_H
#define CGUI_RECT_H

typedef struct Rectangle {
    int x;
    int y;
    int width;
    int height;
} CGUI_Rectangle;

CGUI_Rectangle cgui_createRectangle(int x, int y, int width, int height);

#endif //CGUI_RECT_H
