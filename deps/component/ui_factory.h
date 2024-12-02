//
// Created on: 2024/11/21, 10:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_FACTORY_H
#define CGUI_UI_FACTORY_H

#include "../util/hashmap.h"
#include "../geo/rect.h"
#include "../win32/gdi.h"

#include "../app/application.h"

typedef struct UIFactoryCluster CGUI_UIFactoryCluster;
typedef void* CGUI_AnyComponent;
typedef CGUI_Result (* CGUI_UIFactory)(int argc, CGUI_Box* argv);

typedef struct UIFactoryCluster {
    HashTable* factories;

    CGUI_Result (* registerFactory)(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory);
    CGUI_Result (* unregisterFactory)(CGUI_UIFactoryCluster* cluster, const char* name);

    CGUI_Result (* createComponent)(CGUI_UIFactoryCluster* cluster, const char* name, int argc, CGUI_Box* argv);
} CGUI_UIFactoryCluster;

CGUI_UIFactoryCluster* cgui_createUIFactoryCluster();
void cgui_destroyUIFactoryCluster(CGUI_UIFactoryCluster* cluster);
CGUI_UIFactoryCluster* cgui_getUIFactoryClusterInstance();

#define cgui_uiFactoryCluster() cgui_getUIFactoryClusterInstance()

CGUI_Result cgui_factoryCluster_registerFactory(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory);
CGUI_Result cgui_factoryCluster_unregisterFactory(CGUI_UIFactoryCluster* cluster, const char* name);
CGUI_Result cgui_factoryCluster_createComponent(CGUI_UIFactoryCluster* cluster, const char* name, int argc, CGUI_Box* argv);

void cgui_initFactoryCluster(CGUI_UIFactoryCluster* cluster);

typedef struct WindowClassOptions {
    CGUI_UIComponent* parent;

    LPCSTR className;
    LPCSTR menuName;

    HCURSOR cursor;
    HICON icon;

    LPCSTR title;
    CGUI_Rectangle geometry;

    bool allowDoubleClick;
} CGUI_WindowClassOptions;

CGUI_Result cgui_uiFactory_createWindow(int argc, CGUI_Box* argv);

typedef enum ButtonType {
    CGUI_ButtonType_Default,
    CGUI_ButtonType_CheckBox,
    CGUI_ButtonType_RadioButton,
} CGUI_ButtonType;

/**
 * Default state of the button.
 * When set to false, the button will remain unchecked;
 * When set to true, the button will be checked.
 * @note Only available when buttonType is CGUI_ButtonType_CheckBox or CGUI_ButtonType_RadioButton.
 */
typedef bool CGUI_ButtonDefaultState;

typedef struct ButtonOptions {
    CGUI_UIComponent* parent;

    CGUI_ButtonType buttonType;

    LPCSTR text;
    CGUI_Rectangle geometry;

    CGUI_ButtonDefaultState defaultState;

    bool hasBorder;
} CGUI_ButtonOptions;

CGUI_Result cgui_uiFactory_createButton(int argc, CGUI_Box* argv);

typedef struct LabelOptions {
    CGUI_UIComponent* parent;

    LPCSTR text;
    CGUI_Rectangle geometry;

    bool hasBorder;
} CGUI_LabelOptions;

CGUI_Result cgui_uiFactory_createLabel(int argc, CGUI_Box* argv);

typedef struct TextBoxOptions {
    CGUI_UIComponent* parent;

    LPCSTR text;
    CGUI_Rectangle geometry;

    bool hasBorder;

    bool allowMultiline;

    bool allowAutoScrollH;
    bool allowAutoScrollV;

    bool displayScrollBarH;
    bool displayScrollBarV;

    bool isReadOnly;
    // this property does not function well.
    obsolete bool isPassword;
} CGUI_TextBoxOptions;

CGUI_Result cgui_uiFactory_createTextBox(int argc, CGUI_Box* argv);

CGUI_Result cgui_uiFactory_createListBox(int argc, CGUI_Box* argv);

#endif //CGUI_UI_FACTORY_H
