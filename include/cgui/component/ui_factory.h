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

#include "ui_window.h"
#include "ui_button.h"
#include "ui_label.h"
#include "ui_textbox.h"
#include "ui_list.h"

#include "../app/application.h"

typedef struct UIFactoryCluster CGUI_UIFactoryCluster;
typedef void* CGUI_AnyComponent;

typedef CGUI_Result (* CGUI_UIFactory)(int argc, CGUI_Box* argv);

typedef struct UIFactoryClusterConfig {
    bool forceUpdateParent;
} CGUI_UIFactoryClusterConfig;

CGUI_UIFactoryClusterConfig cgui_createUIFactoryClusterConfig();

typedef struct UIFactoryCluster {
    CGUI_UIFactoryClusterConfig config;

    HashTable* factories;

    CGUI_Result (* registerFactory)(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory);

    CGUI_Result (* unregisterFactory)(CGUI_UIFactoryCluster* cluster, const char* name);

    CGUI_Result (* createComponent)(CGUI_UIFactoryCluster* cluster, const char* name, int argc, CGUI_Box* argv);
} CGUI_UIFactoryCluster;

CGUI_UIFactoryCluster* cgui_createUIFactoryCluster();

void cgui_destroyUIFactoryCluster(CGUI_UIFactoryCluster* cluster);

CGUI_UIFactoryCluster* cgui_getUIFactoryClusterInstance();

#define cgui_uiFactoryCluster() cgui_getUIFactoryClusterInstance()

CGUI_Result
cgui_factoryCluster_registerFactory(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory);

CGUI_Result cgui_factoryCluster_unregisterFactory(CGUI_UIFactoryCluster* cluster, const char* name);

CGUI_Result
cgui_factoryCluster_createComponent(CGUI_UIFactoryCluster* cluster, const char* name, int argc, CGUI_Box* argv);

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

    bool displayScrollBarH;
    bool displayScrollBarV;

    bool hasCaption;
    bool hasSystemMenu;
    bool isThickFrame;
    bool hasMinimizeButton;
    bool hasMaximizeButton;
} CGUI_WindowClassOptions;

CGUI_WindowClassOptions cgui_defaultWindowOptions(const char* className);

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

    bool displayScrollBarH;
    bool displayScrollBarV;
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
    unavailable bool isPassword;
} CGUI_TextBoxOptions;

CGUI_Result cgui_uiFactory_createTextBox(int argc, CGUI_Box* argv);

typedef struct ListBoxOptions {
    CGUI_UIComponent* parent;
    CGUI_Rectangle geometry;

    unavailable LPCSTR text;
    CGUI_ListViewSelectionMode selectionMode;
    unavailable bool allowDragDrop;

    bool hasBorder;
    bool hasComboBox;
    bool displayScrollBarH;
    bool displayScrollBarV;
} CGUI_ListViewOptions;

CGUI_Result cgui_uiFactory_createListView(int argc, CGUI_Box* argv);

typedef struct ComponentWrapper_ {
    CGUI_UINativeImageLabel* (* wrapImageLabel)(CGUI_UINativeLabel* standardLabel);
} CGUI_ComponentWrapper_;

CGUI_UINativeImageLabel* cgui_wrapImageLabel(CGUI_UINativeLabel* standardLabel);

static CGUI_ComponentWrapper_ CGUI_ComponentWrapper = {
        .wrapImageLabel = cgui_wrapImageLabel,
};

#endif //CGUI_UI_FACTORY_H
