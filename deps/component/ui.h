//
// Created on: 2024/11/7, 21:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_H
#define CGUI_UI_H

#include <windows.h>

#include "../util/common.h"
#include "../geo/rect.h"
#include "../util/vector.h"
#include "../event/handler.h"
#include "../geo/layout.h"

#ifndef CGUI_UI_TRAITS
#define CGUI_UI_TRAITS

typedef unsigned int CGUI_Trait;

const CGUI_Trait CGUI_Trait_UIComponent     = 0x00000001;
const CGUI_Trait CGUI_Trait_UIDisposable    = 0x00000002;
const CGUI_Trait CGUI_Trait_UIState         = 0x00000004;
const CGUI_Trait CGUI_Trait_UIDrawable      = 0x00000008;
const CGUI_Trait CGUI_Trait_UILayout        = 0x00000010;
const CGUI_Trait CGUI_Trait_UIStyle         = 0x00000020;

#define impl(_implFlag, _Traits) (((_implFlag) & (_Traits)) == (_Traits))

#endif

/* Prototypes */
typedef struct UIComponent CGUI_UIComponent;

typedef struct UIDisposable CGUI_UIDisposable;

typedef struct UILayout CGUI_UILayout;

typedef struct UIDrawable CGUI_UIDrawable;

typedef struct UIState CGUI_UIState;

typedef struct UIStyle CGUI_UIStyle;

typedef struct UIDisposable {
    /**
     * Destructor.
     * This shall be implemented in derived classes, to do the actual cleanup.
     *
     * - What it NEED to handle: **the derived component**, its traits implementation **inside** `CGUI_UIComponent`,
     *   and custom extensions.
     *
     * - What it **DON'T NEED** to handle: the components from `CGUI_UIComponent` as well as `CGUI_UIComponent` itself.
     *   That includes: properties of it (e.g. EventHandlers), **the children of the component**, and **the parent of the component**.
     *   The destructor of CGUI_UIComponent will handle the destruction of these! */
    void (* destructor)(CGUI_UIComponent* component);
} CGUI_UIDisposable;

typedef struct UILayout {
    CGUI_Rectangle geometry;
    CGUI_Layout layout;

    CGUI_Result             (* setGeometry) (CGUI_UIComponent* component, CGUI_Rectangle geometry);
    CGUI_Result             (* setLayout)   (CGUI_UIComponent* component, CGUI_Layout layout);
    const CGUI_Rectangle*   (* getGeometry) (CGUI_UIComponent* component);
    const CGUI_Layout*      (* getLayout)   (CGUI_UIComponent* component);
} CGUI_UILayout;

typedef struct UIDrawable {
    void (* draw)(CGUI_UIComponent* component);
    void (* refresh)(CGUI_UIComponent* component);
} CGUI_UIDrawable;

typedef struct UIState {
    bool visible;
    bool enabled;

    CGUI_Result (* setVisible)  (CGUI_UIComponent* component, bool visible);
    CGUI_Result (* setEnabled)  (CGUI_UIComponent* component, bool enabled);
    bool        (* isVisible)   (CGUI_UIComponent* component);
    bool        (* isEnabled)   (CGUI_UIComponent* component);
} CGUI_UIState;

typedef struct UIStyle {
    COLORREF backgroundColor;
    COLORREF foregroundColor;
    COLORREF borderColor;
    COLORREF shadowColor;

    int borderWidth;
    int cornerRadius;

    int fontSize;
    int fontWeight;
    int fontStyle;
    LPCSTR fontFamily;

    int textAlignment;

    int dropShadow;
    int shadowBlur;
    int shadowOffsetX;
    int shadowOffsetY;
} CGUI_UIStyle;

/**
 * The base 'class' for all high-level abstractions of UI components.
 * It is used to provide a common 'interface' (if you name it).
 * By the way, all the 'classes' derived from this class shall
 * avoid using low-level API abstractions (e.g. GDI, Win32, etc.) directly. */
typedef struct UIComponent {
    CGUI_Trait implFlag;

    /* Properties */
    LPCSTR name;
    LONG_PTR id;

    CGUI_UIComponent* parent;
    Vector* children;

    CGUI_EventHandler* eventHandler;

    /* Traits Implementation */
    CGUI_UIDisposable*  disposable;
    CGUI_UIDrawable*    drawable;
    CGUI_UIState*       state;
    CGUI_UILayout*      layout;
    CGUI_UIStyle*       style;

    /* Methods */
    void                (* addChild)            (CGUI_UIComponent* component, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChild)         (CGUI_UIComponent* component, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChildById)     (CGUI_UIComponent* component, LONG_PTR id);

    CGUI_Result (* setComponentName)    (CGUI_UIComponent* component, LPCSTR name);
    LPCSTR      (* getComponentName)    (CGUI_UIComponent* component);

    void        (* setEventHandler)     (CGUI_UIComponent* component, CGUI_EventHandler* handler);

} CGUI_UIComponent;

/**
 * Create a new component with specified traits.
 * @param name The name of the component.
 * @param id The ID of the component.
 * @param parent The parent of the component.
 * @param implFlag The traits implementation flag.
 * @return The created component. */
CGUI_UIComponent* cgui_createUIComponent(LPCSTR name, LONG_PTR id, CGUI_UIComponent* parent, CGUI_Trait implFlag);

/**
 * Create a new component, which only implements CGUI_Trait_UIComponent itself.
 * @param name The name of the component.
 * @param id The ID of the component.
 * @param parent The parent of the component.
 * @return The created component. */
CGUI_UIComponent* cgui_createUIComponentDefault(LPCSTR name, LONG_PTR id, CGUI_UIComponent* parent);

/**
 * Destroy the component.
 * @note This method will destroy all the children of the component,
 * @note This method will call the destructor of the component's traits implementation (implemented by user), but if not provided, nothing will happen.
 * @note This method will destroy **the component itself**. Do not double-free!
 * @param component The component to destroy. */
void cgui_destroyUIComponent(CGUI_UIComponent* component);

/**
 * Set the name of the component.
 * @param name The new name of the component.
 * @return The result of the operation. */
CGUI_Result cgui_uiComponent_setComponentName(CGUI_UIComponent* component, LPCSTR name);

/**
 * Get the name of the component.
 * @return The name of the component. */
LPCSTR      cgui_uiComponent_getComponentName(CGUI_UIComponent* component);

void cgui_uiComponent_addChild(CGUI_UIComponent* component, CGUI_UIComponent* child);

/**
 * Remove a child from the children list.
 * This method **WILL NOT** destroy the child.
 * It is the caller's responsibility to do so.
 * @param child The child to remove.
 * @return The removed child.*/
CGUI_UIComponent* cgui_uiComponent_removeChild(CGUI_UIComponent* component, CGUI_UIComponent* child);

/**
 * Remove a child from the children list by its ID.
 * This method **WILL NOT** destroy the child.
 * It is the caller's responsibility to do so.
 * @param id The ID of the child to remove.
 * @return The removed child.*/
CGUI_UIComponent* cgui_uiComponent_removeChildById(CGUI_UIComponent* component, LONG_PTR id);

/**
 * Set the event handler of the component.
 * This method **WILL** destroy the old handler.
 * It is the caller's responsibility to do so.
 * @param handler The new event handler. */
void cgui_uiComponent_setEventHandler(CGUI_UIComponent* component, CGUI_EventHandler* handler);

/*
void cgui_uiComponent_draw(CGUI_UIComponent* component);
void cgui_uiComponent_refresh(CGUI_UIComponent* component);

CGUI_Result cgui_uiComponent_setVisible(CGUI_UIComponent* component, bool visible);
CGUI_Result cgui_uiComponent_setEnabled(CGUI_UIComponent* component, bool enabled);
bool cgui_uiComponent_isVisible(CGUI_UIComponent* component);
bool cgui_uiComponent_isEnabled(CGUI_UIComponent* component);

CGUI_Result cgui_uiComponent_setGeometry(CGUI_UIComponent* component, CGUI_Rectangle geometry);
CGUI_Result cgui_uiComponent_setLayout(CGUI_UIComponent* component, CGUI_Layout layout);
const CGUI_Rectangle* cgui_uiComponent_getGeometry(CGUI_UIComponent* component);
const CGUI_Layout* cgui_uiComponent_getLayout(CGUI_UIComponent* component);
*/

#endif //CGUI_UI_H
