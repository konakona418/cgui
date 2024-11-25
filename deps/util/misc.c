//
// Created on: 2024/11/25, 20:16.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "misc.h"

CGUI_ComponentQuery cgui_createComponentQuery(HWND hwnd, int internalId) {
    CGUI_ComponentQuery query = {hwnd, internalId};
    return query;
}
