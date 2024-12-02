//
// Created on: 2024/11/21, 10:24.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui_factory.h"
#include "ui_window.h"
#include "ui_button.h"
#include "ui_label.h"
#include "ui_textbox.h"
#include "ui_list.h"

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
    cluster->registerFactory(cluster, "listview", cgui_uiFactory_createListView);
    cluster->registerFactory(cluster, "textbox", cgui_uiFactory_createTextBox);
}

CGUI_WindowClassOptions cgui_defaultWindowOptions(const char* className) {
    CGUI_WindowClassOptions options = {
            .className = className,
            .cursor = cgui_defaultCursor(),
            .geometry = {
                    .x = 100,
                    .y = 100,
                    .width = 640,
                    .height = 480
            },
            .icon = cgui_defaultIcon(),
            .menuName = "Window",
            .title = "Window",
            .allowDoubleClick = false,
            .displayScrollBarH = false,
            .displayScrollBarV = false,
            .hasCaption = true,
            .hasSystemMenu = true,
            .isThickFrame = true,
            .hasMinimizeButton = true,
            .hasMaximizeButton = true,
            .parent = NULL,
    };
    return options;
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
    wndFactory->setWindowExStyle(wndFactory, WS_EX_COMPOSITED); // use double buffering.

    CGUI_Win32WSParam aggregatedStyle = 0;
    aggregatedStyle |= options->displayScrollBarH ? WS_HSCROLL : 0;
    aggregatedStyle |= options->displayScrollBarV ? WS_VSCROLL : 0;

    aggregatedStyle |= options->hasCaption ? WS_CAPTION : 0;
    aggregatedStyle |= options->hasSystemMenu ? WS_SYSMENU : 0;
    aggregatedStyle |= options->isThickFrame ? WS_THICKFRAME : 0;
    aggregatedStyle |= options->hasMinimizeButton ? WS_MINIMIZEBOX : 0;
    aggregatedStyle |= options->hasMaximizeButton ? WS_MAXIMIZEBOX : 0;

    aggregatedStyle |= WS_CLIPCHILDREN;
    wndFactory->setWindowStyle(wndFactory, aggregatedStyle);

    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);
    wndFactory->setWindowMenu(wndFactory, NULL); // no menu for now.
    if (options->parent) {
        wndFactory->setWindowParent(wndFactory, cgui_getComponentWindowHandle(options->parent));
    }
    // todo: fill the rest of the options.

    wnd = unwrap(wndFactory->createWindow(wndFactory));
    wndManager->addWindow(wndManager, wnd);

    CGUI_ComponentManager* compManager = app->core->compManager;

    CGUI_InternalID nextId = compManager->getNextInternalId(compManager);
    CGUI_UIWindow* wndComp = cgui_createUINativeWindowFromWindow(wnd, NULL, nextId);
    compManager->addComponent(compManager, wndComp->component);

    if (options->parent) {
        CGUI_UIComponent* parent = options->parent;
        parent->addChild(parent, wndComp->component);
    }

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
    wndFactory->setWindowExStyle(wndFactory, WS_EX_TRANSPARENT);
    wndFactory->setWindowClassName(wndFactory, "BUTTON");
    wndFactory->setWindowName(wndFactory, options->text);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);

    CGUI_Win32WSParam aggregatedStyle = 0;
    aggregatedStyle |= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
    aggregatedStyle |= options->hasBorder ? WS_BORDER : 0;
    wndFactory->setWindowStyle(wndFactory, aggregatedStyle);

    HWND hParent;
    if (options->parent) {
        hParent = cgui_getComponentWindowHandle(options->parent);
        wndFactory->setWindowParent(wndFactory, hParent);
    } else {
        hParent = NULL;
    }
    wndFactory->setWindowMenu(wndFactory, (HMENU) nextId);
    wndFactory->setWindowInstance(wndFactory, app->ctx->hInstance);

    CGUI_Window* wnd = unwrap(wndFactory->createWindow(wndFactory));

    CGUI_UINativeButton* buttonComp = cgui_createUINativeButtonFromWindow(wnd, options->parent, nextId);

    compManager->addComponent(compManager, buttonComp->component);

    if (options->parent) {
        CGUI_UIComponent* parent = options->parent;
        parent->addChild(parent, buttonComp->component);
    }

    CGUI_ButtonHandler* localHandler = cgui_createButtonHandler();
    CGUI_LocalHandlerContext localHandlerCtx = {
            .destructor = cgui_destroyButtonHandler,
            .handlerFlag = CGUI_LocalHandler_Button,
            .localHandler = localHandler
    };
    CGUI_EventHandler* eventHandler = cgui_createEventHandler(localHandlerCtx, buttonComp->component);

    buttonComp->setEventHandler(buttonComp, eventHandler);

    switch (buttonType) {
        case CGUI_ButtonType_Default:
            buttonComp->setButtonStyle(buttonComp, CGUI_ButtonStyle_Default);
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Normal);
            break;
        case CGUI_ButtonType_CheckBox:
            buttonComp->setButtonStyle(buttonComp, CGUI_ButtonStyle_CheckBox);
            options->defaultState ?
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Checked) :
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Unchecked);
            break;
        case CGUI_ButtonType_RadioButton:
            buttonComp->setButtonStyle(buttonComp, CGUI_ButtonStyle_RadioButton);
            options->defaultState ?
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Checked) :
            buttonComp->setButtonState(buttonComp, CGUI_ButtonState_Unchecked);
            break;
    }

    // todo: low-end function calls.
    if (IsWindow(hParent)) {
        UpdateWindow(hParent);
        InvalidateRect(hParent, NULL, TRUE);
    }

    return create_ok(buttonComp);
}

CGUI_Result cgui_uiFactory_createLabel(int argc, CGUI_Box* argv) {
    if (argc < 1) {
        return create_err(CGUI_Error_InvalidArgument());
    }
    CGUI_LabelOptions* options = unbox_as(CGUI_LabelOptions, &argv[0]);

    CGUI_Application* app = cgui_applicationInstance();

    CGUI_ComponentManager* compManager = app->core->compManager;
    CGUI_InternalID nextId = compManager->getNextInternalId(compManager);

    CGUI_WindowFactory* wndFactory = app->core->wndFactory;
    wndFactory->setWindowExStyle(wndFactory, WS_EX_TRANSPARENT);
    wndFactory->setWindowClassName(wndFactory, "STATIC");
    wndFactory->setWindowName(wndFactory, options->text);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);

    CGUI_Win32WSParam aggregatedStyle = 0;
    aggregatedStyle |= options->hasBorder ? WS_BORDER : 0;
    aggregatedStyle |= options->displayScrollBarH ? WS_HSCROLL : 0;
    aggregatedStyle |= options->displayScrollBarV ? WS_VSCROLL : 0;
    aggregatedStyle |= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;

    wndFactory->setWindowStyle(wndFactory, aggregatedStyle);

    HWND hParent;
    if (options->parent) {
        hParent = cgui_getComponentWindowHandle(options->parent);
        wndFactory->setWindowParent(wndFactory, hParent);
    } else {
        hParent = NULL;
    }

    wndFactory->setWindowMenu(wndFactory, (HMENU) nextId);
    wndFactory->setWindowInstance(wndFactory, app->ctx->hInstance);
    CGUI_Window* wnd = unwrap(wndFactory->createWindow(wndFactory));

    CGUI_UINativeLabel* labelComp = cgui_createUINativeLabelFromWindow(wnd, options->parent, nextId);
    compManager->addComponent(compManager, labelComp->component);

    if (options->parent) {
        CGUI_UIComponent* parent = options->parent;
        parent->addChild(parent, labelComp->component);
    }

    CGUI_LabelHandler* localHandler = cgui_createLabelHandler();
    CGUI_LocalHandlerContext localHandlerCtx = {
            .destructor = cgui_destroyLabelHandler,
            .handlerFlag = CGUI_LocalHandler_Label,
            .localHandler = localHandler
    };

    CGUI_EventHandler* eventHandler = cgui_createEventHandler(localHandlerCtx, labelComp->component);
    labelComp->setEventHandler(labelComp, eventHandler);

    if (IsWindow(hParent)) {
        UpdateWindow(hParent);
        InvalidateRect(hParent, NULL, TRUE);
    }

    return create_ok(labelComp);
}

CGUI_Result cgui_uiFactory_createTextBox(int argc, CGUI_Box* argv) {
    if (argc < 1) {
        return create_err(CGUI_Error_InvalidArgument());
    }
    CGUI_TextBoxOptions* options = unbox_as(CGUI_TextBoxOptions, &argv[0]);

    CGUI_Application* app = cgui_applicationInstance();

    CGUI_ComponentManager* compManager = app->core->compManager;
    CGUI_InternalID nextId = compManager->getNextInternalId(compManager);

    CGUI_WindowFactory* wndFactory = app->core->wndFactory;

    wndFactory->setWindowExStyle(wndFactory, WS_EX_TRANSPARENT);
    wndFactory->setWindowClassName(wndFactory, "EDIT");
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);
    wndFactory->setWindowName(wndFactory, options->text);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);

    CGUI_Win32WSParam aggregatedStyle = 0;
    aggregatedStyle |= options->allowMultiline ? ES_MULTILINE : 0;
    aggregatedStyle |= options->allowAutoScrollH ? ES_AUTOHSCROLL : 0;
    aggregatedStyle |= options->allowAutoScrollV ? ES_AUTOVSCROLL : 0;
    aggregatedStyle |= options->displayScrollBarH ? WS_HSCROLL : 0;
    aggregatedStyle |= options->displayScrollBarV ? WS_VSCROLL : 0;
    aggregatedStyle |= options->isReadOnly ? ES_READONLY : 0;
    // aggregatedStyle |= options->isPassword ? ES_PASSWORD : 0;
    aggregatedStyle |= options->hasBorder ? WS_BORDER : 0;
    aggregatedStyle |= (WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS);

    wndFactory->setWindowStyle(wndFactory, aggregatedStyle);

    HWND hParent;
    if (options->parent) {
        hParent = cgui_getComponentWindowHandle(options->parent);
        wndFactory->setWindowParent(wndFactory, hParent);
    } else {
        hParent = NULL;
    }

    wndFactory->setWindowMenu(wndFactory, (HMENU) nextId);
    wndFactory->setWindowInstance(wndFactory, app->ctx->hInstance);

    CGUI_Window* wnd = unwrap(wndFactory->createWindow(wndFactory));

    CGUI_UINativeTextBox* textBoxComp = cgui_createUINativeTextboxFromWindow(wnd, options->parent, nextId);
    compManager->addComponent(compManager, textBoxComp->component);

    if (options->parent) {
        CGUI_UIComponent* parent = options->parent;
        parent->addChild(parent, textBoxComp->component);
    }

    CGUI_TextBoxHandler* localHandler = cgui_createTextBoxHandler();
    CGUI_LocalHandlerContext localHandlerCtx = {
            .destructor = cgui_destroyTextBoxHandler,
            .handlerFlag = CGUI_LocalHandler_TextBox,
            .localHandler = localHandler
    };
    CGUI_EventHandler* eventHandler = cgui_createEventHandler(localHandlerCtx, textBoxComp->component);
    textBoxComp->setEventHandler(textBoxComp, eventHandler);

    if (IsWindow(hParent)) {
        UpdateWindow(hParent);
        InvalidateRect(hParent, NULL, TRUE);
    }

    return create_ok(textBoxComp);
}

CGUI_Result cgui_uiFactory_createListView(int argc, CGUI_Box* argv) {
    if (argc < 1) {
        return create_err(CGUI_Error_InvalidArgument());
    }
    CGUI_ListViewOptions* options = unbox_as(CGUI_ListViewOptions, &argv[0]);

    CGUI_Application* app = cgui_applicationInstance();

    CGUI_ComponentManager* compManager = app->core->compManager;
    CGUI_InternalID nextId = compManager->getNextInternalId(compManager);

    CGUI_WindowFactory* wndFactory = app->core->wndFactory;

    wndFactory->setWindowExStyle(wndFactory, WS_EX_TRANSPARENT);
    wndFactory->setWindowClassName(wndFactory, "LISTBOX");
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);
    // wndFactory->setWindowName(wndFactory, options->text);
    wndFactory->setWindowGeometryRect(wndFactory, &options->geometry);

    CGUI_Win32WSParam aggregatedStyle = 0;
    aggregatedStyle |= options->allowMultipleSelection ? LBS_MULTIPLESEL : 0;
    aggregatedStyle |= options->displayScrollBarV ? WS_VSCROLL : 0;
    aggregatedStyle |= options->displayScrollBarH ? WS_HSCROLL : 0;

    // todo: this is just a temporal solution. this will cause the list to be a multi-selectable list
    // but it will be found when the user used SHIFT/CTRL+click to select multiple items
    aggregatedStyle |= options->extendItemToFit ? LBS_EXTENDEDSEL : 0;
    aggregatedStyle |= options->hasBorder ? WS_BORDER : 0;
    aggregatedStyle |= options->hasComboBox ? LBS_COMBOBOX : 0;
    aggregatedStyle |= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | LBS_NOTIFY;

    wndFactory->setWindowStyle(wndFactory, aggregatedStyle);
    wndFactory->setWindowInstance(wndFactory, app->ctx->hInstance);
    wndFactory->setWindowMenu(wndFactory, (HMENU) nextId);

    HWND hParent;
    if (options->parent) {
        CGUI_UIComponent* parent = options->parent;
        hParent = cgui_getComponentWindowHandle(parent);
    } else {
        hParent = NULL;
    }

    wndFactory->setWindowParent(wndFactory, hParent);

    CGUI_Window* wnd = unwrap(wndFactory->createWindow(wndFactory));

    CGUI_UINativeListView* listViewComp = cgui_createUINativeListViewFromWindow(wnd, options->parent, nextId);

    if (options->allowMultipleSelection) {
        listViewComp->allowMultipleSelection = true;
    }

    compManager->addComponent(compManager, listViewComp->component);

    CGUI_ListViewHandler* localHandler = cgui_createListViewHandler();
    CGUI_LocalHandlerContext localHandlerCtx = {
            .destructor = cgui_destroyListViewHandler,
            .handlerFlag = CGUI_LocalHandler_ListView,
            .localHandler = localHandler
    };
    CGUI_EventHandler* eventHandler = cgui_createEventHandler(localHandlerCtx, listViewComp->component);
    listViewComp->setEventHandler(listViewComp, eventHandler);

    if (IsWindow(cgui_getComponentWindowHandle(options->parent))) {
        UpdateWindow(cgui_getComponentWindowHandle(options->parent));
        InvalidateRect(cgui_getComponentWindowHandle(options->parent), NULL, TRUE);
    }

    return create_ok(listViewComp);
}
