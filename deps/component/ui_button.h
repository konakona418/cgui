//
// Created on: 2024/11/29, 11:06.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_UI_BUTTON_H
#define CGUI_UI_BUTTON_H

#include "ui.h"
#include "../win32/window.h"
#include "../win32/gdi.h"

#define CGUI_DEFAULT_BUTTON_PRESET (BS_PUSHBUTTON | BS_FLAT)
#define CGUI_CHECKBOX_BUTTON_PRESET (BS_CHECKBOX | BS_FLAT)
#define CGUI_RADIOBUTTON_BUTTON_PRESET (BS_RADIOBUTTON | BS_FLAT)

typedef struct UINativeButton CGUI_UINativeButton;
typedef CGUI_UINativeButton CGUI_UIButton;
typedef CGUI_UINativeButton CGUI_UIRadioButton;
typedef CGUI_UINativeButton CGUI_UICheckBox;

typedef enum UINativeButtonStyle {
    CGUI_ButtonStyle_Default,
    CGUI_ButtonStyle_CheckBox,
    CGUI_ButtonStyle_RadioButton,
} CGUI_UINativeButtonStyle;

typedef enum UINativeButtonState {
    CGUI_ButtonState_Normal,
    CGUI_ButtonState_Pushed,

    /* for checkbox */
    CGUI_ButtonState_Checked,
    CGUI_ButtonState_Unchecked,
} CGUI_UINativeButtonState;

typedef struct UINativeButton {
    CGUI_UIComponent* component;

    CGUI_Window* window;

    CGUI_UINativeButtonStyle buttonStyle;
    CGUI_UINativeButtonState buttonState;

    CGUI_GDITextContext* gdiTextContext;
    obsolete bool _gdiRefreshFlag;

    /* methods */
    CGUI_Result (* bindWindowInstance)(CGUI_UINativeButton* self, CGUI_Window* nativeWindow);

    CGUI_Result (* setState)(CGUI_UINativeButton* self, int swState);
    CGUI_Result (* postMessage)(CGUI_UINativeButton* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);
    CGUI_Result (* hide)(CGUI_UINativeButton* self);
    CGUI_Result (* show)(CGUI_UINativeButton* self);
    CGUI_Result (* update)(CGUI_UINativeButton* self);
    CGUI_Result (* close)(CGUI_UINativeButton* self);
    CGUI_Result (* destroy)(CGUI_UINativeButton* self);
    
    CGUI_Result (* setText)(CGUI_UINativeButton* self, LPCSTR text);
    LPCSTR      (* getText)(CGUI_UINativeButton* self);
    CGUI_Result (* setTextDisplay)(CGUI_UINativeButton* self, CGUI_GDITextContext* ctx);
    CGUI_Result (* setButtonStyle)(CGUI_UINativeButton* self, CGUI_UINativeButtonStyle style);
    CGUI_UINativeButtonStyle (* getButtonStyle)(CGUI_UINativeButton* self);
    CGUI_Result (* setButtonState)(CGUI_UINativeButton* self, CGUI_UINativeButtonState state);
    CGUI_UINativeButtonState (* getButtonState)(CGUI_UINativeButton* self);

    /* implementation for CGUI_UIComponent */
    void                (* addChild)            (CGUI_UINativeButton* self, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChild)         (CGUI_UINativeButton* self, CGUI_UIComponent* child);
    CGUI_UIComponent*   (* removeChildById)     (CGUI_UINativeButton* self, LONG_PTR id);

    CGUI_Result (* setComponentName)    (CGUI_UINativeButton* self, LPCSTR name);
    LPCSTR      (* getComponentName)    (CGUI_UINativeButton* self);

    void        (* setEventHandler)     (CGUI_UINativeButton* self, CGUI_EventHandler* handler);

    /* implementation for CGUI_UIDrawable */
    void (* draw)       (CGUI_UINativeButton* self);
    void (* refresh)    (CGUI_UINativeButton* self);

    /* implementation for CGUI_UILayout */
    CGUI_Result       (* setGeometry) (CGUI_UINativeButton* self, CGUI_Rectangle geometry);
    CGUI_Result       (* setLayout)   (CGUI_UINativeButton* self, CGUI_Layout layout);
    CGUI_Rectangle*   (* getGeometry) (CGUI_UINativeButton* self);
    CGUI_Layout*      (* getLayout)   (CGUI_UINativeButton* self);

    /* implementation for CGUI_UIState */
    CGUI_Result (* setVisible)  (CGUI_UINativeButton* self, bool visible);
    CGUI_Result (* setEnabled)  (CGUI_UINativeButton* self, bool enabled);
    bool        (* isVisible)   (CGUI_UINativeButton* self);
    bool        (* isEnabled)   (CGUI_UINativeButton* self);

} CGUI_UINativeButton;

CGUI_UINativeButton* cgui_createUINativeButton(CGUI_UIComponent* parent, CGUI_InternalID internalId);

CGUI_UINativeButton* cgui_createUINativeButtonFromWindow(CGUI_Window* nativeWindow, CGUI_UIComponent* parent, CGUI_InternalID internalId);

CGUI_Result cgui_uiNativeButton_bindWindowInstance(CGUI_UINativeButton* self, CGUI_Window* nativeWindow);

void cgui_destroyUINativeButton(CGUI_UINativeButton* window);

void cgui_uiNativeButton_destroyCallback(CGUI_UIComponent* component);

void cgui_uiNativeButton_readyCallback(CGUI_UIComponent* component);

void cgui_uiNativeButton_drawCallback(CGUI_UIComponent* component);

void cgui_uiNativeButton_refreshCallback(CGUI_UIComponent* component);

CGUI_Result cgui_uiNativeButton_setState(CGUI_UINativeButton* self, int swState);

CGUI_Result
cgui_uiNativeButton_postMessage(CGUI_UINativeButton* self, bool isAsync, UINT msg, WPARAM wParam, LPARAM lParam);

CGUI_Result cgui_uiNativeButton_hide(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_show(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_update(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_close(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_destroy(CGUI_UINativeButton* self);

void cgui_uiNativeButton_addChild(CGUI_UINativeButton* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeButton_removeChild(CGUI_UINativeButton* self, CGUI_UIComponent* child);

CGUI_UIComponent* cgui_uiNativeButton_removeChildById(CGUI_UINativeButton* self, LONG_PTR id);

CGUI_Result cgui_uiNativeButton_setComponentName(CGUI_UINativeButton* self, LPCSTR name);

LPCSTR cgui_uiNativeButton_getComponentName(CGUI_UINativeButton* self);

void cgui_uiNativeButton_setEventHandler(CGUI_UINativeButton* self, CGUI_EventHandler* handler);

void cgui_uiNativeButton_draw(CGUI_UINativeButton* self);

void cgui_uiNativeButton_refresh(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_setGeometry(CGUI_UINativeButton* self, CGUI_Rectangle geometry);

CGUI_Result cgui_uiNativeButton_setLayout(CGUI_UINativeButton* self, CGUI_Layout layout);

CGUI_Rectangle* cgui_uiNativeButton_getGeometry(CGUI_UINativeButton* self);

CGUI_Layout* cgui_uiNativeButton_getLayout(CGUI_UINativeButton* self);

bool cgui_uiNativeButton_isVisible(CGUI_UINativeButton* self);

bool cgui_uiNativeButton_isEnabled(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_setVisible(CGUI_UINativeButton* self, bool visible);

CGUI_Result cgui_uiNativeButton_setEnabled(CGUI_UINativeButton* self, bool enabled);

HWND cgui_uiNativeButton_getWindowHandle(CGUI_UIComponent* component);

LPCSTR cgui_uiNativeButton_getText(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_setText(CGUI_UINativeButton* self, LPCSTR text);

CGUI_Result cgui_uiNativeButton_setTextDisplay(CGUI_UINativeButton* self, CGUI_GDITextContext * gdiTextContext);

CGUI_Result cgui_uiNativeButton_setButtonStyle(CGUI_UINativeButton* self, CGUI_UINativeButtonStyle style);

CGUI_UINativeButtonStyle cgui_uiNativeButton_getButtonStyle(CGUI_UINativeButton* self);

CGUI_Result cgui_uiNativeButton_setButtonState(CGUI_UINativeButton* self, CGUI_UINativeButtonState state);

CGUI_UINativeButtonState cgui_uiNativeButton_getButtonState(CGUI_UINativeButton* self);

#endif //CGUI_UI_BUTTON_H
