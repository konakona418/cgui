//
// Created on: 2024/11/21, 10:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_FACTORY_H
#define CGUI_UI_FACTORY_H

#include "../util/hashmap.h"
#include "../geo/rect.h"

#include "../app/application.h"

typedef struct UIFactoryCluster CGUI_UIFactoryCluster;
typedef void* CGUI_AnyComponent;
typedef CGUI_Result (* CGUI_UIFactory)(int argc, void* argv);

typedef struct UIFactoryCluster {
    HashTable* factories;

    CGUI_Result (* registerFactory)(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory);
    CGUI_Result (* unregisterFactory)(CGUI_UIFactoryCluster* cluster, const char* name);

    CGUI_Result (* createComponent)(CGUI_UIFactoryCluster* cluster, const char* name, int argc, void* argv);
} CGUI_UIFactoryCluster;

CGUI_UIFactoryCluster* CGUI_createUIFactoryCluster();
void CGUI_destroyUIFactoryCluster(CGUI_UIFactoryCluster* cluster);

CGUI_Result cgui_factoryCluster_registerFactory(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory);
CGUI_Result cgui_factoryCluster_unregisterFactory(CGUI_UIFactoryCluster* cluster, const char* name);
CGUI_Result cgui_factoryCluster_createComponent(CGUI_UIFactoryCluster* cluster, const char* name, int argc, void* argv);

CGUI_Result cgui_initFactoryCluster(CGUI_UIFactoryCluster* cluster);

typedef struct WindowClassOptions {
    LPCSTR className;
    LPCSTR menuName;

    HCURSOR cursor;
    HICON icon;

    LPCSTR title;
    CGUI_Rectangle geometry;
} CGUI_WindowClassOptions;

CGUI_Result cgui_uiFactory_createWindow(int argc, void* argv);

#endif //CGUI_UI_FACTORY_H
