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

/**
 * @note This macro can be applied to evaluating whether the given component implements all the required traits.
 * @param _implFlag The implementation flag ( @p implFlag ) of a certain component instance.
 * @param _Traits The required traits, connected by bitwise OR.
 * @return Whether the component implements all the required traits.
 */
#define impl(_implFlag, _Traits) (((_implFlag) & (_Traits)) == (_Traits))

#endif

/* Prototypes */
typedef struct UIComponent CGUI_UIComponent;

typedef struct UIDisposable CGUI_UIDisposable;

typedef struct UILayout CGUI_UILayout;

typedef struct UIDrawable CGUI_UIDrawable;

typedef struct UIState CGUI_UIState;

typedef struct UIStyle CGUI_UIStyle;

typedef enum FontStyle CGUI_FontStyle;

typedef enum TextAlign CGUI_TextAlign;

typedef enum FontStyle {
    CGUI_TextStyle_Normal,
    CGUI_TextStyle_Italic,
    CGUI_TextStyle_Bold,
    CGUI_TextStyle_BoldItalic
} CGUI_FontStyle;

typedef enum TextAlign {
    CGUI_TextAlign_Left,
    CGUI_TextAlign_Center,
    CGUI_TextAlign_Right
} CGUI_TextAlign;

typedef struct UIDisposable {
    /**
     * Destructor.
     *
     * @note This shall be implemented in derived classes, to do the actual cleanup.
     *
     *   @note What it @b NEED to handle: the @b derived @b component , its traits implementation @b inside @p CGUI_UIComponent,
     *   and custom extensions.
     *
     *   @note What it @b DON'T @b NEED to handle: the components from @p CGUI_UIComponent as well as @p CGUI_UIComponent itself.
     *   That includes: properties of it (e.g. EventHandlers), @b the @b children of the component, and @b the @b parent of the component.
     *   The destructor of @p CGUI_UIComponent will handle the destruction of these! */
    void (* destructor)(CGUI_UIComponent* component);
} CGUI_UIDisposable;

typedef struct UILayout {
    CGUI_Rectangle geometry;
    CGUI_Layout layout;

    CGUI_Result       (* setGeometry) (CGUI_UIComponent* component, CGUI_Rectangle geometry);
    CGUI_Result       (* setLayout)   (CGUI_UIComponent* component, CGUI_Layout layout);
    CGUI_Rectangle*   (* getGeometry) (CGUI_UIComponent* component);
    CGUI_Layout*      (* getLayout)   (CGUI_UIComponent* component);
} CGUI_UILayout;

typedef struct UIDrawable {
    void (* drawCallback)(CGUI_UIComponent* component);
    void (* refreshCallback)(CGUI_UIComponent* component);

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

    int             fontSize;
    int             fontWeight;
    CGUI_FontStyle  fontStyle;
    LPCSTR          fontFamily;

    CGUI_TextAlign textAlignment;

    int dropShadow;
    int shadowBlur;
    int shadowOffsetX;
    int shadowOffsetY;
} CGUI_UIStyle;

/**
 * @note The base 'class' for all high-level abstractions of UI components.
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
    CGUI_UIDisposable*  disposableImpl;
    CGUI_UIDrawable*    drawableImpl;
    CGUI_UIState*       stateImpl;
    CGUI_UILayout*      layoutImpl;
    CGUI_UIStyle*       styleImpl;

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
 * @note when the component is registered as a child of another component, there's no need to dealloc the component.
 * @note however when it is NOT registered as a child of another component, you need to dealloc it manually.
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
 * @note This method will destroy @b the @p CGUI_UIComponent @b itself. Do not double-free!
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
 * This method @b WILL @b NOT destroy the child.
 * It is the caller's responsibility to do so.
 * @param child The child to remove.
 * @return The removed child.*/
CGUI_UIComponent* cgui_uiComponent_removeChild(CGUI_UIComponent* component, CGUI_UIComponent* child);

/**
 * Remove a child from the children list by its ID.
 * @note This method @b WILL @b NOT destroy the child.
 * It is the caller's responsibility to do so.
 * @param id The ID of the child to remove.
 * @return The removed child.*/
CGUI_UIComponent* cgui_uiComponent_removeChildById(CGUI_UIComponent* component, LONG_PTR id);

/**
 * Set the event handler of the component.
 * @note This method @b WILL destroy the old handler.
 * @param handler The new event handler. */
void cgui_uiComponent_setEventHandler(CGUI_UIComponent* component, CGUI_EventHandler* handler);


/**
 * Create a new drawable component.
 * @param drawCallback The callback to be called when the component needs to be drawn.
 * @param refreshCallback The callback to be called when the component needs to be refreshed.
 * @return The created drawable component. */
CGUI_UIDrawable* cgui_createUIDrawable(void (* drawCallback)(CGUI_UIComponent* component), void (* refreshCallback)(CGUI_UIComponent* component));

/**
 * Destroy the drawable component.
 * @note This method will destroy the callback.
 * @param drawable The drawable component to destroy. */
void cgui_destroyUIDrawable(CGUI_UIDrawable* drawable);

/**
 * Draw the component.
 * @note This method will call the draw callback of the component's traits implementation (implemented by user), but if not provided, nothing will happen.
 * @note This method will call the draw callback of all its children.
 * @param component The component to draw. */
void cgui_uiDrawable_draw(CGUI_UIComponent* component);

/**
 * Refresh the component.
 * @note This method will call the refresh callback of the component's traits implementation (implemented by user), but if not provided, nothing will happen.
 * @note This method will call the refresh callback of all its children.
 * @param component The component to refresh. */
void cgui_uiDrawable_refresh(CGUI_UIComponent* component);

/**
 * Create a new state component.
 * @return The created state component. */
CGUI_UIState* cgui_createUIState();

/**
 * Destroy the state component.
 * @param state The state component to destroy. */
void cgui_destroyUIState(CGUI_UIState* state);

/**
 * Get the visibility of the component.
 * @param component The component to get the visibility of.
 * @return The visibility of the component. */
bool cgui_uiState_isVisible(CGUI_UIComponent* component);

/**
 * Get the enabled state of the component.
 * @param component The component to get the enabled state of.
 * @return The enabled state of the component. */
bool cgui_uiState_isEnabled(CGUI_UIComponent* component);

/**
 * Set the visibility of the component.
 * @param component The component to set the visibility of.
 * @param visible The new visibility of the component.
 * @return The result of the operation. */
CGUI_Result cgui_uiState_setVisible(CGUI_UIComponent* component, bool visible);

/**
 * Set the enabled state of the component.
 * @param component The component to set the enabled state of.
 * @param enabled The new enabled state of the component.
 * @return The result of the operation. */
CGUI_Result cgui_uiState_setEnabled(CGUI_UIComponent* component, bool enabled);

/**
 * Create a new layout component.
 * @return The created layout component. */
CGUI_UILayout* cgui_createUILayout();

/**
 * Destroy the layout component.
 * @param layout The layout component to destroy. */
void cgui_destroyUILayout(CGUI_UILayout* layout);

/**
 * Set the geometry of the component.
 * @param component The component to set the geometry of.
 * @param geometry The new geometry of the component.
 * @return The result of the operation. */
CGUI_Result cgui_uiLayout_setGeometry(CGUI_UIComponent* component, CGUI_Rectangle geometry);

/**
 * Set the layout of the component.
 * @param component The component to set the layout of.
 * @param layout The new layout of the component.
 * @return The result of the operation. */
CGUI_Result cgui_uiLayout_setLayout(CGUI_UIComponent* component, CGUI_Layout layout);

/**
 * Get the geometry of the component.
 * @param component The component to get the geometry of.
 * @return The geometry of the component. */
CGUI_Rectangle* cgui_uiLayout_getGeometry(CGUI_UIComponent* component);

/**
 * Get the layout of the component.
 * @param component The component to get the layout of.
 * @return The layout of the component. */
CGUI_Layout* cgui_uiLayout_getLayout(CGUI_UIComponent* component);

/**
 * Create a new style component.
 * @warning not implemented yet.
 * @return The created style component. */
CGUI_UIStyle* cgui_createUIStyle();

/**
 * Destroy the style component.
 * @param style The style component to destroy. */
void cgui_destroyUIStyle(CGUI_UIStyle* style);

#endif //CGUI_UI_H
