//
// Created on: 2024/11/25, 20:14.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_MISC_H
#define CGUI_MISC_H

#include <windows.h>

typedef struct ComponentQuery CGUI_ComponentQuery;
typedef int CGUI_InternalID;

typedef int (* CGUI_ApplicationMessageCallback)(CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct ComponentQuery {
    HWND hwnd;
    CGUI_InternalID internalId;

    void* component;
} CGUI_ComponentQuery;

#define CGUI_COMPONENT_QUERY_NO_ID (-1)
#define cgui_componentQueryHasId(_query) (_query.internalId != CGUI_COMPONENT_QUERY_NO_ID)
#define cgui_componentQueryIsIdLegal(_internalId) (_internalId >= 0)

CGUI_ComponentQuery cgui_createComponentQuery(HWND hwnd, int internalId);

#endif //CGUI_MISC_H
