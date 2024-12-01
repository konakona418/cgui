#include <stdio.h>
#include "deps/util/common.h"
#include "deps/util/hashmap.h"
#include "deps/util/vector.h"
#include "deps/win32/window.h"
#include "deps/app/core.h"
#include "deps/app/application.h"
#include "deps/component/ui_factory.h"
#include "deps/component/ui_window.h"
#include "deps/component/ui_button.h"
#include "deps/component/ui_label.h"

CGUI_UINativeWindow* wnd;
CGUI_UINativeLabel* label;
CGUI_UINativeLabel* label2;

int cnt = 0;


void onClose(CGUI_EventArgs args) {
    wnd->close(wnd);
    wnd->destroy(wnd);
    cgui_applicationInstance()->stop();
}

void onClick(CGUI_MouseEventArgs args) {
    printf("Clicked @ (%ld,%ld)!\n", args.point.x, args.point.y);
}

void btnOnClick(CGUI_MouseEventArgs args) {
    CGUI_UINativeButton* btn = into(CGUI_UINativeButton, ((CGUI_UIComponent*) args.base.component)->disposableImpl->upperLevel);
    CGUI_UINativeButtonState state = btn->getButtonState(btn);
    //btn->update(btn);
    if (state == CGUI_ButtonState_Unchecked) {
        btn->setButtonState(btn, CGUI_ButtonState_Checked);
        btn->setText(btn, "Unchecked!");
        label->setText(label, "Unchecked!");
    } else {
        btn->setButtonState(btn, CGUI_ButtonState_Unchecked);
        btn->setText(btn, "Checked!");
        label->setText(label, "Checked!");
    }
    label2->setText(label2, cgui_digitToString(cnt++));
}

int main(void) {
    CGUI_RuntimeContext* ctx = cgui_defaultRuntimeContext();
    CGUI_Application* app = cgui_createApplication(ctx);

    CGUI_UIFactoryCluster* uiFactory = cgui_createUIFactoryCluster();

    CGUI_WindowClassOptions options = {
            .className = "CGUI_Window",
            .cursor = cgui_defaultCursor(),
            .geometry = {
                    .x = 100,
                    .y = 100,
                    .width = 640,
                    .height = 480
            },
            .icon = cgui_defaultIcon(),
            .menuName = "test",
            .title = "CGUI Window",
            .allowDoubleClick = false
    };
    wnd = unwrap(uiFactory->createComponent(uiFactory, "Window", 1, into_box(&options)));
    wnd->show(wnd);

    CGUI_WindowHandler* wndHandler = into(CGUI_WindowHandler, wnd->component->eventHandler->localHandler);
    wndHandler->onClose = onClose;
    wndHandler->onMouseUp = onClick;

    CGUI_GDITextContext gdiTextContext = {
            .fontStyle = {
                    .fontName = "Segoe UI",
                    .fontSize = 20,
                    .foregroundColor = cgui_rgbaToColor(255, 0, 0),
                    .backgroundColor = cgui_rgbaToColor(0, 255, 255),
            },
            .alignment = CGUI_TextAlignment_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
    };

    CGUI_GDITextContext* ctxTextDisp = cgui_createGdiTextContextFromInstance(gdiTextContext);

    CGUI_ButtonOptions buttonOptions = {
            .geometry = {
                    .x = 10,
                    .y = 10,
                    .width = 100,
                    .height = 100
            },
            .parent = wnd->component,
            .text = "Click me!",
            .buttonType = CGUI_ButtonType_RadioButton,
            .defaultState = true,
    };
    CGUI_UINativeButton* button = unwrap(uiFactory->createComponent(uiFactory, "Button", 1, into_box(&buttonOptions)));
    //wnd->addChild(wnd, button->component);

    button->setVisible(button, true);

    CGUI_ButtonHandler* btnHandler = into(CGUI_ButtonHandler, button->component->eventHandler->localHandler);
    btnHandler->onClick = btnOnClick;
    btnHandler->onDoubleClick = btnOnClick;

    button->setTextDisplay(button, ctxTextDisp);

    CGUI_LabelOptions labelOptions = {
            .geometry = {
                    .x = 10,
                    .y = 120,
                    .width = 100,
                    .height = 100
            },
            .parent = wnd->component,
            .text = "Label",
    };
    label = unwrap(uiFactory->createComponent(uiFactory, "Label", 1, into_box(&labelOptions)));
    //wnd->addChild(wnd, label->component);

    label->setVisible(label, true);
    label->setTextDisplay(label, ctxTextDisp);

    CGUI_LabelOptions labelOptions2 = {
            .geometry = {
                    .x = 200,
                    .y = 160,
                    .width = 300,
                    .height = 150
            },
            .parent = wnd->component,
            .text = "Label2",
    };

    label2 = unwrap(uiFactory->createComponent(uiFactory, "Label", 1, into_box(&labelOptions2)));
    //wnd->addChild(wnd, label2->component);

    label2->setTextDisplay(label2, ctxTextDisp);
    label2->setVisible(label2, true);

    //wnd->ready(wnd);

    cgui_destroyUIFactoryCluster(uiFactory);

    app->run(false);

    return 0;
}
