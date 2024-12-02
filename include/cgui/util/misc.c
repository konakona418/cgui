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

int cgui_cStringLen(LPCSTR str) {
    return (int) strlen(str);
}

void cgui_destroyListViewItem(CGUI_ListViewItem* item) {
    free(item->text);
}

void cgui_destroyListViewItems(CGUI_ListViewItems* items) {
    for (int i = 0; i < items->count; i++) {
        cgui_destroyListViewItem(&items->items[i]);
    }
}
