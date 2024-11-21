//
// Created on: 2024/11/21, 10:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_factory.h"
#include "ui_window.h"

CGUI_UIFactoryCluster* cgui_createUIFactoryCluster() {
    CGUI_UIFactoryCluster* cluster = (CGUI_UIFactoryCluster*) malloc(sizeof(CGUI_UIFactoryCluster));
    cluster->factories = create_hash_table(DEFAULT_BUCKET_COUNT);

    cluster->registerFactory = cgui_factoryCluster_registerFactory;
    cluster->unregisterFactory = cgui_factoryCluster_unregisterFactory;
    cluster->createComponent = cgui_factoryCluster_createComponent;
    return cluster;
}

void cgui_destroyUIFactoryCluster(CGUI_UIFactoryCluster* cluster) {
    cluster->factories->iter_values(cluster->factories, free);
    destroy_hash_table(cluster->factories);
    free(cluster);
}

CGUI_Result cgui_factoryCluster_registerFactory(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory) {
    if (cluster->factories->insert(cluster->factories, name, factory))
        return create_ok(NULL);
    return create_err(createError(1, "Failed to register factory."));
}

CGUI_Result cgui_factoryCluster_unregisterFactory(CGUI_UIFactoryCluster* cluster, const char* name) {
    if (cluster->factories->remove(cluster->factories, name))
        return create_ok(NULL);
    return create_err(createError(1, "Failed to unregister factory."));
}

CGUI_Result cgui_factoryCluster_createComponent(CGUI_UIFactoryCluster* cluster, const char* name, int argc, void* argv) {
    CGUI_UIFactory factory = cluster->factories->find(cluster->factories, name);
    if (unlikely(factory == NULL)) {
        return create_err(CGUI_Error_FactoryNotFound());
    }
    CGUI_Result result = factory(argc, argv);
    return result;
}

CGUI_Result cgui_initFactoryCluster(CGUI_UIFactoryCluster* cluster) {
    cluster->registerFactory(cluster, "window", cgui_uiFactory_createWindow);
}

CGUI_Result cgui_uiFactory_createWindow(int argc, void* argv) {
    CGUI_WindowClassOptions* options;
    if (argc < 1) {
        return create_err(CGUI_Error_InvalidArgument());
    } else {
        options = (CGUI_WindowClassOptions*) argv;
    }

    CGUI_Application* app = unwrap_option(cgui_getApplicationInstance());
    CGUI_WindowClassFactory* wndClassFactory = app->core->wndClassFactory;
    CGUI_WindowClassManager* wndClassManager = app->core->wndClassManager;

    CGUI_Option option = wndClassManager->getWindowClass(wndClassManager, options->className);
    CGUI_WindowClass* wndClass;
    if (unlikely(is_none(&option))) {
        wndClassFactory->setWindowClassStyle(wndClassFactory, CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_DROPSHADOW);
        wndClassFactory->setWindowInstance(wndClassFactory, GetModuleHandle(NULL));
        wndClassFactory->setWindowProc(wndClassFactory, app->handler->getWindowProc(app->handler));
        wndClassFactory->setWindowBackgroundBrush(wndClassFactory, (HBRUSH)(COLOR_WINDOW + 1));

        wndClassFactory->setWindowClassName(wndClassFactory, options->className);
        if (options->menuName) {
            wndClassFactory->setWindowMenuName(wndClassFactory, options->menuName);
        } else {
            wndClassFactory->setWindowMenuName(wndClassFactory, "Window");
        }
        if (options->icon) {
            wndClassFactory->setWindowIcon(wndClassFactory, options->icon);
        }
        if (options->cursor) {
            wndClassFactory->setWindowCursor(wndClassFactory, options->cursor);
        }

        wndClass = unwrap(wndClassFactory->createWindowClass(wndClassFactory));
        wndClassManager->addWindowClassAndRegister(wndClassManager, wndClass);
    } else {
        wndClass = unwrap_option(option);
    }

    CGUI_WindowFactory* wndFactory = app->core->wndFactory;
    CGUI_WindowManager* wndManager = app->core->wndManager;

    CGUI_Window* wnd;
    wndFactory->setWindowClass(wndFactory, wndClass);
    wndFactory->setWindowName(wndFactory, options->title);
    wndFactory->setWindowStyle(wndFactory, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CHILD);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);
    // todo: fill the rest of the options.

    wnd = unwrap(wndFactory->createWindow(wndFactory));
    wndManager->addWindow(wndManager, wnd);

    CGUI_ComponentManager* compManager = app->core->compManager;
    CGUI_UIWindow* wndComp = cgui_createUINativeWindowFromWindow(wnd, NULL);
    compManager->addComponent(compManager, wndComp->component);

    return create_ok(wndComp);
}

