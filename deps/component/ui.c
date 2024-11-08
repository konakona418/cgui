//
// Created on: 2024/11/7, 21:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui.h"
#include "../util/common.h"
#include "../util/error.h"

CGUI_UIComponent* cgui_createUIComponentDefault(LPCSTR name, LONG_PTR id, CGUI_UIComponent* parent) {
    CGUI_UIComponent* component = cgui_createUIComponent(
            name, id, parent,
            CGUI_Trait_UIComponent);

    return component;
}

CGUI_UIComponent* cgui_createUIComponent(LPCSTR name, LONG_PTR id, CGUI_UIComponent* parent, CGUI_Trait implFlag) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) malloc(sizeof(CGUI_UIComponent));

    component->implFlag = implFlag;

    // set the properties of the base class.
    component->name = name;
    component->id = id;
    component->parent = parent;
    component->children = create_vector(sizeof(CGUI_UIComponent*), 0);
    component->eventHandler = NULL;

    // set the traits to null.
    component->disposable = NULL;
    component->drawable = NULL;
    component->layout = NULL;
    component->state = NULL;
    component->style = NULL;

    // register the methods of the base class.
    component->addChild = cgui_uiComponent_addChild;
    component->removeChild = cgui_uiComponent_removeChild;
    component->removeChildById = cgui_uiComponent_removeChildById;

    component->setComponentName = cgui_uiComponent_setComponentName;
    component->getComponentName = cgui_uiComponent_getComponentName;

    component->setEventHandler = cgui_uiComponent_setEventHandler;

    /*
    component->setVisible = cgui_uiComponent_setVisible;
    component->setEnabled = cgui_uiComponent_setEnabled;
    component->isVisible = cgui_uiComponent_isVisible;
    component->isEnabled = cgui_uiComponent_isEnabled;

    component->setGeometry = cgui_uiComponent_setGeometry;
    component->getGeometry = cgui_uiComponent_getGeometry;
    component->setLayout = cgui_uiComponent_setLayout;
    component->getLayout = cgui_uiComponent_getLayout;
    */

    return component;
}

void cgui_destroyUIComponent(CGUI_UIComponent* component) {
    // note that all the properties of the base class will be automatically freed.
    if (component->eventHandler != NULL) {
        cgui_destroyEventHandler(component->eventHandler);
    }

    component->children->iter(component->children, (void (*)(void*)) cgui_destroyUIComponent);

    // call the custom destructor to free the custom type specific properties.
    if (component->disposable->destructor != NULL &&
        impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIDisposable)) {
        component->disposable->destructor(component);
    } else {
        printf("No destructor found for the component %s.\n", component->name);
    }

    // note that the component itself is freed here!!
    // don't free it in the caller nor the component destructor.
    free(component);
}

void cgui_uiComponent_addChild(CGUI_UIComponent* component, CGUI_UIComponent* child) {
    // add the child to the children list.
    component->children->push_back(component->children, child);
}

CGUI_UIComponent* cgui_uiComponent_removeChild(CGUI_UIComponent* component, CGUI_UIComponent* child) {
    size_t i = 0;
    while (i < component->children->size) {
        if (component->children->get(component->children, i) == child) {
            CGUI_UIComponent* removed = component->children->remove(component->children, i);
            return removed;
        }
        i++;
    }
    return NULL;
}

CGUI_UIComponent* cgui_uiComponent_removeChildById(CGUI_UIComponent* component, LONG_PTR id) {
    size_t i = 0;
    while (i < component->children->size) {
        CGUI_UIComponent* child = (CGUI_UIComponent*) component->children->get(component->children, i);
        if (child->id == id) {
            CGUI_UIComponent* removed = component->children->remove(component->children, i);
            return removed;
        }
    }
    return NULL;
}

CGUI_Result cgui_uiComponent_setComponentName(CGUI_UIComponent* component, LPCSTR name) {
    if (component == NULL) {
        return create_err(CGUI_Error_IllegalNullPtr());
    }
    component->name = name;
    return create_ok(NULL);
}

LPCSTR cgui_uiComponent_getComponentName(CGUI_UIComponent* component) {
    return component->name;
}

void cgui_uiComponent_setEventHandler(CGUI_UIComponent* component, CGUI_EventHandler* handler) {
    if (component->eventHandler != NULL) {
        cgui_destroyEventHandler(component->eventHandler);
    }
    component->eventHandler = handler;
}
