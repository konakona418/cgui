//
// Created on: 2024/11/25, 20:14.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_MISC_H
#define CGUI_MISC_H

#include <windows.h>

typedef struct ComponentQuery CGUI_ComponentQuery;
typedef long long int CGUI_InternalID;

typedef LONG CGUI_Win32DTParam;
typedef LONG CGUI_Win32TAParam;
typedef LONG CGUI_Win32SSParam;
typedef LONG CGUI_Win32ESParam;
typedef LONG CGUI_Win32WSParam;

int cgui_cStringLen(LPCSTR str);

typedef long long int (* CGUI_ApplicationMessageCallback)(CGUI_ComponentQuery query, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct ComponentQuery {
    HWND hwnd;
    CGUI_InternalID internalId;

    void* component;
} CGUI_ComponentQuery;

typedef union ListViewItemSelector_ {
    int idx;
    LPCSTR name;
} CGUI_ListViewItemSelector_;

typedef enum ListViewItemSelectorType {
    CGUI_ListViewItemSelectorType_Index,
    CGUI_ListViewItemSelectorType_Name
} CGUI_ListViewItemSelectorType;

typedef struct ListViewItemSelector {
    CGUI_ListViewItemSelectorType selectorType;
    CGUI_ListViewItemSelector_ inner;
} CGUI_ListViewItemSelector;

typedef struct ListViewItem {
    LPSTR text;
    int idx;
} CGUI_ListViewItem;

typedef enum ListViewSelectionMode {
    CGUI_ListViewSelectionMode_Single,
    CGUI_ListViewSelectionMode_Multiple,
    CGUI_ListViewSelectionMode_ExtendedMultiple,
} CGUI_ListViewSelectionMode;

void cgui_destroyListViewItem(CGUI_ListViewItem* item);

typedef struct ListViewItems {
    CGUI_ListViewItem* items;
    int count;
} CGUI_ListViewItems;

void cgui_destroyListViewItems(CGUI_ListViewItems* items);

#define cgui_getComponentWindowHandle(_Component) ((_Component)->win32Impl->getWindowHandle((_Component)))
#define cgui_getUpperLevel(_TComponentImpl, _Component) (into((_TComponentImpl), ((CGUI_UIComponent*) (_Component))->disposableImpl->upperLevel))
#define cgui_getComponentHandler(_THandler, _Component) (into(_THandler, (_Component)->eventHandler->localHandler))

#define cgui_defaultRuntimeContext() (unwrap(cgui_createRuntimeContext(GetModuleHandle(NULL), GetCommandLine(), SW_SHOW)))
#define cgui_defaultIcon() (LoadIcon(NULL, IDI_APPLICATION))
#define cgui_defaultCursor() (LoadCursor(NULL, IDC_ARROW))
#define cgui_applicationInstance() (into(CGUI_Application, unwrap_option(cgui_tryGetApplicationInstance())))

#define CGUI_COMPONENT_QUERY_NO_ID (-1)
#define cgui_componentQueryHasId(_query) (_query.internalId != CGUI_COMPONENT_QUERY_NO_ID)
#define cgui_componentQueryIsIdLegal(_internalId) (_internalId >= 0)

CGUI_ComponentQuery cgui_createComponentQuery(HWND hwnd, int internalId);

#endif //CGUI_MISC_H
