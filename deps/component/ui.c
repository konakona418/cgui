//
// Created on: 2024/11/7, 21:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "ui.h"
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
    component->disposableImpl = NULL;
    component->drawableImpl = NULL;
    component->layoutImpl = NULL;
    component->stateImpl = NULL;
    component->styleImpl = NULL;
    component->win32Impl = NULL;

    // register the methods of the base class.
    component->addChild = cgui_uiComponent_addChild;
    component->removeChild = cgui_uiComponent_removeChild;
    component->removeChildById = cgui_uiComponent_removeChildById;

    component->setComponentName = cgui_uiComponent_setComponentName;
    component->getComponentName = cgui_uiComponent_getComponentName;

    component->setEventHandler = cgui_uiComponent_setEventHandler;

    return component;
}

void cgui_destroyUIComponent(CGUI_UIComponent* component) {
    // note that all the properties of the base class will be automatically freed.
    if (component->eventHandler != NULL) {
        cgui_destroyEventHandler(component->eventHandler);
    }

    component->children->iter(component->children, (void (*)(void*)) cgui_destroyUIComponent);

    // call the custom destructor to free the custom type specific properties.
    if (component->disposableImpl->destructor != NULL &&
        impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIDisposable)) {
        component->disposableImpl->destructor(component);

        // free the disposable impl.
        cgui_destroyUIDisposable(component->disposableImpl);
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

CGUI_UIDisposable* cgui_createUIDisposable(void* upperLevel, void (* destructor)(CGUI_UIComponent* component)) {
    CGUI_UIDisposable* disposable = (CGUI_UIDisposable*) malloc(sizeof(CGUI_UIDisposable));
    disposable->destructor = destructor;

    return disposable;
}

void cgui_destroyUIDisposable(CGUI_UIDisposable* disposable) {
    free(disposable);
}

CGUI_UIDrawable* cgui_createUIDrawable(void (* drawCallback)(CGUI_UIComponent* component), void (* refreshCallback)(CGUI_UIComponent* component)) {
    CGUI_UIDrawable* drawable = (CGUI_UIDrawable*) malloc(sizeof(CGUI_UIDrawable));
    drawable->drawCallback = drawCallback;
    drawable->refreshCallback = refreshCallback;

    drawable->draw = cgui_uiDrawable_draw;
    drawable->refresh = cgui_uiDrawable_refresh;
    return drawable;
}

void cgui_destroyUIDrawable(CGUI_UIDrawable* drawable) {
    free(drawable);
}

void cgui_uiDrawable_draw(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIDrawable)) {
        if (component->drawableImpl->drawCallback != NULL) {
            component->drawableImpl->drawCallback(component);
        }
    }
    component->children->iter(component->children, (void (*)(void*)) cgui_uiDrawable_draw);
}

void cgui_uiDrawable_refresh(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIDrawable)) {
        if (component->drawableImpl->refreshCallback != NULL) {
            component->drawableImpl->refreshCallback(component);
        }
    }
    component->children->iter(component->children, (void (*)(void*)) cgui_uiDrawable_refresh);
}

CGUI_UIState* cgui_createUIState() {
    CGUI_UIState* state = (CGUI_UIState*) malloc(sizeof(CGUI_UIState));
    state->enabled = true;
    state->visible = true;

    state->isEnabled = cgui_uiState_isEnabled;
    state->isVisible = cgui_uiState_isVisible;
    state->setEnabled = cgui_uiState_setEnabled;
    state->setVisible = cgui_uiState_setVisible;

    return state;
}

void cgui_destroyUIState(CGUI_UIState* state) {
    free(state);
}

bool cgui_uiState_isEnabled(CGUI_UIComponent* state) {
    if (impl(state->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIState)) {
        return state->stateImpl->enabled;
    }
    return false;
}

bool cgui_uiState_isVisible(CGUI_UIComponent* state) {
    if (impl(state->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIState)) {
        return state->stateImpl->visible;
    }
    return false;
}

CGUI_Result cgui_uiState_setEnabled(CGUI_UIComponent* state, bool enabled) {
    if (impl(state->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIState)) {
        state->stateImpl->enabled = enabled;
    }
    return create_ok(NULL);
}

CGUI_Result cgui_uiState_setVisible(CGUI_UIComponent* state, bool visible) {
    if (impl(state->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UIState)) {
        state->stateImpl->visible = visible;
    }
    return create_ok(NULL);
}

CGUI_UILayout* cgui_createUILayout() {
    CGUI_Layout layout;
    layout.padding = CGUI_PADDING_DEFAULT;
    layout.margin = CGUI_MARGIN_DEFAULT;
    layout.direction = CGUI_Direction_Horizontal;
    layout.alignment = CGUI_Alignment_Left;
    layout.anchor = CGUI_Anchor_TopLeft;

    CGUI_Rectangle rect = cgui_createRectangle(0, 0, 0, 0);

    CGUI_UILayout* impl = (CGUI_UILayout*) malloc(sizeof(CGUI_UILayout));
    impl->layout = layout;
    impl->geometry = rect;

    impl->getGeometry = cgui_uiLayout_getGeometry;
    impl->setGeometry = cgui_uiLayout_setGeometry;
    impl->getLayout = cgui_uiLayout_getLayout;
    impl->setLayout = cgui_uiLayout_setLayout;

    return impl;
}

void cgui_destroyUILayout(CGUI_UILayout* layout) {
    free(layout);
}

CGUI_Rectangle* cgui_uiLayout_getGeometry(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UILayout)) {
        return &component->layoutImpl->geometry;
    }
    return NULL;
}

CGUI_Result cgui_uiLayout_setGeometry(CGUI_UIComponent* component, CGUI_Rectangle rect) {
    if (impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UILayout)) {
        component->layoutImpl->geometry = rect;
    }
    return create_ok(NULL);
}

CGUI_Layout* cgui_uiLayout_getLayout(CGUI_UIComponent* component) {
    if (impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UILayout)) {
        return &component->layoutImpl->layout;
    }
    return NULL;
}

CGUI_Result cgui_uiLayout_setLayout(CGUI_UIComponent* component, CGUI_Layout layout) {
    if (impl(component->implFlag, CGUI_Trait_UIComponent | CGUI_Trait_UILayout)) {
        component->layoutImpl->layout = layout;
    }
    return create_ok(NULL);
}

CGUI_UIStyle* cgui_createUIStyle() {
    CGUI_UIStyle* style = (CGUI_UIStyle*) malloc(sizeof(CGUI_UIStyle));
    // todo: to be implemented.
    return style;
}

void cgui_destroyUIStyle(CGUI_UIStyle* style) {
    free(style);
}

CGUI_UIWin32* cgui_createUIWin32(HWND (* getWindowHandle)(CGUI_UIComponent* component)) {
    CGUI_UIWin32* win = (CGUI_UIWin32*) malloc(sizeof(CGUI_UIWin32));
    win->getWindowHandle = getWindowHandle;
    return win;
}

void cgui_destroyUIWin32(CGUI_UIWin32* win) {
    free(win);
}

const char* id_to_string(LONG_PTR id) {
    char buffer[24];
    sprintf(buffer, "%lld", id);
    return strdup(buffer);
}

CGUI_ComponentManager* cgui_createComponentManager() {
    CGUI_ComponentManager* manager = (CGUI_ComponentManager*) malloc(sizeof(CGUI_ComponentManager));
    manager->components = create_hash_table(DEFAULT_BUCKET_COUNT);

    manager->addComponent = cgui_componentManager_addComponent;

    manager->getComponentById = cgui_componentManager_getComponentById;
    manager->getComponentByName = cgui_componentManager_getComponentByName;
    manager->getComponentPredicate = cgui_componentManager_getComponentPredicate;

    manager->removeComponent = cgui_componentManager_removeComponent;
    manager->removeComponentById = cgui_componentManager_removeComponentById;
    manager->removeComponentByName = cgui_componentManager_removeComponentByName;
    manager->removeAllComponents = cgui_componentManager_removeAllComponents;

    manager->iter = cgui_componentManager_iter;

    return manager;
}

void cgui_destroyUIComponentIfParent(CGUI_UIComponent* component) {
    if (component->parent == NULL) {
        cgui_destroyUIComponent(component);
    }
}

void cgui_destroyComponentManager(CGUI_ComponentManager* manager) {
    manager->components->iter_values(manager->components, (void (*)(void*)) cgui_destroyUIComponentIfParent);
    // todo: this may lead to double free as the parent component will free its children.
    destroy_hash_table(manager->components);
    free(manager);
}

CGUI_Result cgui_componentManager_addComponent(CGUI_ComponentManager* manager, CGUI_UIComponent* component) {
    if (manager->components->contains(manager->components, id_to_string(component->id))) {
        return create_err(CGUI_Error_ComponentIdAlreadyExists());
    }
    manager->components->insert(manager->components, id_to_string(component->id), component);
    return create_ok(NULL);
}

CGUI_Result cgui_componentManager_removeComponent(CGUI_ComponentManager* manager, CGUI_UIComponent* component) {
    return cgui_componentManager_removeComponentById(manager, component->id);
}

CGUI_Result cgui_componentManager_removeComponentById(CGUI_ComponentManager* manager, LONG_PTR id) {
    if (manager->components->contains(manager->components, id_to_string(id))) {
        manager->components->remove(manager->components, id_to_string(id));
    }
    return create_ok(NULL);
}

IterPredicateResult cgui_componentManager_predicateName(const char* key, void* value, void* target) {
    CGUI_UIComponent* component = (CGUI_UIComponent*) value;
    return strcmp(component->name, (char*) target) == 0;
}

CGUI_Result cgui_componentManager_removeComponentByName(CGUI_ComponentManager* manager, LPCSTR name) {
    char* name_copy = strdup(name);
    CGUI_UIComponent* component = manager->components->find_if(manager->components, name_copy, cgui_componentManager_predicateName);
    cgui_componentManager_removeComponentById(manager, component->id);
    return create_ok(NULL);
}

CGUI_Result cgui_componentManager_removeAllComponents(CGUI_ComponentManager* manager) {
    manager->components->iter_values(manager->components, (void (*)(void*)) cgui_destroyUIComponent);
    destroy_hash_table(manager->components);
    return create_ok(NULL);
}

CGUI_Result cgui_componentManager_getComponentById(CGUI_ComponentManager* manager, LONG_PTR id) {
    CGUI_UIComponent* result = manager->components->find(manager->components, id_to_string(id));
    if (result == NULL) {
        return create_err(CGUI_Error_ComponentNotFound());
    } else {
        return create_ok(result);
    }
}

CGUI_Result cgui_componentManager_getComponentByName(CGUI_ComponentManager* manager, const char* name) {
    CGUI_UIComponent* result = manager->components->find_if(manager->components, (void*) name, cgui_componentManager_predicateName);
    if (result == NULL) {
        return create_err(CGUI_Error_ComponentNotFound());
    } else {
        return create_ok(result);
    }
}

CGUI_Result cgui_componentManager_getComponentPredicate(CGUI_ComponentManager* manager, void* target, ComponentPredicate predicate) {
    CGUI_UIComponent* result = manager->components->find_if(manager->components, target, predicate);
    if (result == NULL) {
        return create_err(CGUI_Error_ComponentNotFound());
    } else {
        return create_ok(result);
    }
}

