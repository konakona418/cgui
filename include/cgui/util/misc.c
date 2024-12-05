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

LPCSTR cgui_utf16ToAnsi(LPCWCH str) {
    if (str == NULL) return NULL;

    size_t size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL) + 1;
    char* ansiString = (char*) malloc(size);
    size_t realSize = WideCharToMultiByte(CP_UTF8, 0, str, -1, ansiString, size, NULL, NULL);
    if (realSize == 0) {
        free(ansiString);
        return NULL;
    }
    return ansiString;
}
