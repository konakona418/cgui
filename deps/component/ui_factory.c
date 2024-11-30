//
// Created on: 2024/11/21, 10:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_factory.h"
#include "ui_window.h"
#include "../event/handler.h"
#include "ui_button.h"

static CGUI_Singleton* cgui_uiFactoryClusterSingleton = NULL;

CGUI_UIFactoryCluster* cgui_createUIFactoryCluster() {
    if (unlikely(cgui_uiFactoryClusterSingleton == NULL)) {
        cgui_uiFactoryClusterSingleton = cgui_createSingleton();

        CGUI_UIFactoryCluster* cluster = (CGUI_UIFactoryCluster*) malloc(sizeof(CGUI_UIFactoryCluster));
        cluster->factories = create_hash_table(DEFAULT_BUCKET_COUNT);

        cluster->registerFactory = cgui_factoryCluster_registerFactory;
        cluster->unregisterFactory = cgui_factoryCluster_unregisterFactory;
        cluster->createComponent = cgui_factoryCluster_createComponent;

        cgui_initFactoryCluster(cluster);

        cgui_initSingleton(cgui_uiFactoryClusterSingleton, cluster);

        return cluster;
    } else if (cgui_isSingletonInitialized(cgui_uiFactoryClusterSingleton)) {
        return cgui_getSingletonValue(cgui_uiFactoryClusterSingleton);
    } else {
        panic("UIFactoryCluster is not initialized.");
    }
}

CGUI_UIFactoryCluster* cgui_getUIFactoryClusterInstance() {
    return cgui_createUIFactoryCluster();
}

void cgui_destroyUIFactoryCluster(CGUI_UIFactoryCluster* cluster) {
    destroy_hash_table(cluster->factories);
    free(cluster);
}

CGUI_Result cgui_factoryCluster_registerFactory(CGUI_UIFactoryCluster* cluster, const char* name, CGUI_UIFactory factory) {
    if (cluster->factories->insert(cluster->factories, cgui_toLowercase(name), factory))
        return create_ok(NULL);
    return create_err(createError(1, "Failed to register factory."));
}

CGUI_Result cgui_factoryCluster_unregisterFactory(CGUI_UIFactoryCluster* cluster, const char* name) {
    if (cluster->factories->remove(cluster->factories, cgui_toLowercase(name)))
        return create_ok(NULL);
    return create_err(createError(1, "Failed to unregister factory."));
}

CGUI_Result cgui_factoryCluster_createComponent(CGUI_UIFactoryCluster* cluster, const char* name, int argc, CGUI_Box* argv) {
    CGUI_UIFactory factory = cluster->factories->find(cluster->factories, cgui_toLowercase(name));
    if (unlikely(factory == NULL)) {
        return create_err(CGUI_Error_FactoryNotFound());
    }
    CGUI_Result result = factory(argc, argv);
    return result;
}

void cgui_initFactoryCluster(CGUI_UIFactoryCluster* cluster) {
    cluster->registerFactory(cluster, "window", cgui_uiFactory_createWindow);
    cluster->registerFactory(cluster, "button", cgui_uiFactory_createButton);
    cluster->registerFactory(cluster, "label", cgui_uiFactory_createLabel);
    cluster->registerFactory(cluster, "listbox", cgui_uiFactory_createListBox);
    cluster->registerFactory(cluster, "textbox", cgui_uiFactory_createTextBox);
}

CGUI_Result cgui_uiFactory_createWindow(int argc, CGUI_Box* argv) {
    CGUI_WindowClassOptions* options;
    if (argc < 1) {
        return create_err(CGUI_Error_InvalidArgument());
    } else {
        options = unbox_as(CGUI_WindowClassOptions, &argv[0]);
    }

    CGUI_Application* app = unwrap_option(cgui_tryGetApplicationInstance());
    CGUI_WindowClassFactory* wndClassFactory = app->core->wndClassFactory;
    CGUI_WindowClassManager* wndClassManager = app->core->wndClassManager;

    CGUI_Option option = wndClassManager->getWindowClass(wndClassManager, options->className);
    CGUI_WindowClass* wndClass;
    if (unlikely(is_none(&option))) {
        if (options->allowDoubleClick) {
            wndClassFactory->setWindowClassStyle(wndClassFactory, CS_DBLCLKS);
        }
        wndClassFactory->setWindowClassName(wndClassFactory, options->className);
        wndClassFactory->setWindowInstance(wndClassFactory, app->ctx->hInstance);
        wndClassFactory->setWindowProc(wndClassFactory, cgui_application_getWindowProc());
        wndClassFactory->setWindowBackgroundBrush(wndClassFactory, (HBRUSH)(COLOR_WINDOW + 1));

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
        unwrap(wndClassManager->addWindowClassAndRegister(wndClassManager, wndClass));
    } else {
        wndClass = unwrap_option(option);
    }

    CGUI_WindowFactory* wndFactory = app->core->wndFactory;
    CGUI_WindowManager* wndManager = app->core->wndManager;

    CGUI_Window* wnd;
    wndFactory->setWindowClass(wndFactory, wndClass);
    wndFactory->setWindowName(wndFactory, options->title);
    // todo: further updates should allow the user to specify the style.
    wndFactory->setWindowStyle(wndFactory, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);
    wndFactory->setWindowMenu(wndFactory, NULL); // no menu for now.
    // todo: fill the rest of the options.

    wnd = unwrap(wndFactory->createWindow(wndFactory));
    wndManager->addWindow(wndManager, wnd);

    CGUI_ComponentManager* compManager = app->core->compManager;

    CGUI_InternalID nextId = compManager->getNextInternalId(compManager);
    CGUI_UIWindow* wndComp = cgui_createUINativeWindowFromWindow(wnd, NULL, nextId);
    compManager->addComponent(compManager, wndComp->component);

    // initialize the event handler.
    CGUI_WindowHandler* localHandler = cgui_createWindowHandler();

    CGUI_LocalHandlerContext localHandlerCtx = {
            .destructor = cgui_destroyWindowHandler,
            .handlerFlag = CGUI_LocalHandler_WindowRoot,
            .localHandler = localHandler
    };
    CGUI_EventHandler* handler = cgui_createEventHandler(localHandlerCtx, wndComp->component);

    wndComp->setEventHandler(wndComp, handler);

    return create_ok(wndComp);
}

void cgui_uiFactory_wrapNormalButton(CGUI_Window* window);

void cgui_uiFactory_wrapCheckBox(CGUI_Window* window);

void cgui_uiFactory_wrapRadioButton(CGUI_Window* window);

CGUI_Result cgui_uiFactory_createButton(int argc, CGUI_Box* argv) {
    if (argc < 1) {
        return create_err(CGUI_Error_InvalidArgument());
    }

    CGUI_ButtonOptions* options = unbox_as(CGUI_ButtonOptions, &argv[0]);
    CGUI_ButtonType buttonType = options->buttonType;

    CGUI_Application* app = cgui_applicationInstance();

    CGUI_ComponentManager* compManager = app->core->compManager;
    CGUI_InternalID nextId = compManager->getNextInternalId(compManager);

    CGUI_WindowFactory* wndFactory = app->core->wndFactory;
    wndFactory->setWindowClassName(wndFactory, "BUTTON");
    wndFactory->setWindowName(wndFactory, options->text);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);
    wndFactory->setWindowStyle(wndFactory, WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPSIBLINGS);

    if (options->parent) {
        wndFactory->setWindowParent(wndFactory, options->parent->win32Impl->getWindowHandle(options->parent));
    }
    wndFactory->setWindowMenu(wndFactory, (HMENU) nextId);
    wndFactory->setWindowInstance(wndFactory, app->ctx->hInstance);

    CGUI_Window* wnd = unwrap(wndFactory->createWindow(wndFactory));

    CGUI_UINativeButton* buttonComp = cgui_createUINativeButtonFromWindow(wnd, options->parent, nextId);

    switch (buttonType) {
        case CGUI_ButtonType_Default:
            cgui_uiFactory_wrapNormalButton(wnd);
            buttonComp->setButtonStyle(buttonComp, CGUI_ButtonStyle_Default);
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Normal);
        case CGUI_ButtonType_CheckBox:
            cgui_uiFactory_wrapCheckBox(wnd);
            buttonComp->setButtonStyle(buttonComp, CGUI_ButtonStyle_CheckBox);
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Unchecked);
        case CGUI_ButtonType_RadioButton:
            cgui_uiFactory_wrapRadioButton(wnd);
            buttonComp->setButtonStyle(buttonComp, CGUI_ButtonStyle_RadioButton);
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Unchecked);
    }

    compManager->addComponent(compManager, buttonComp->component);

    CGUI_ButtonHandler* localHandler = cgui_createButtonHandler();
    CGUI_LocalHandlerContext localHandlerCtx = {
            .destructor = cgui_destroyButtonHandler,
            .handlerFlag = CGUI_LocalHandler_Button,
            .localHandler = localHandler
    };
    CGUI_EventHandler* eventHandler = cgui_createEventHandler(localHandlerCtx, buttonComp->component);

    buttonComp->setEventHandler(buttonComp, eventHandler);

    return create_ok(buttonComp);
}

void cgui_uiFactory_wrapNormalButton(CGUI_Window* window) {
    window->setWindowStyle(window, window->getWindowStyle(window) | BS_DEFPUSHBUTTON);
}

void cgui_uiFactory_wrapCheckBox(CGUI_Window* window) {
    window->setWindowStyle(window, window->getWindowStyle(window) | BS_CHECKBOX);
}

void cgui_uiFactory_wrapRadioButton(CGUI_Window* window) {
    window->setWindowStyle(window, window->getWindowStyle(window) | BS_RADIOBUTTON);
}

CGUI_Result cgui_uiFactory_createLabel(int argc, CGUI_Box* argv) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Result cgui_uiFactory_createTextBox(int argc, CGUI_Box* argv) {
    return create_err(CGUI_Error_NotImplemented());
}

CGUI_Result cgui_uiFactory_createListBox(int argc, CGUI_Box* argv) {
    return create_err(CGUI_Error_NotImplemented());
}
