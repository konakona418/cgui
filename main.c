#include <stdio.h>
#include "include/cgui/util/common.h"
#include "include/cgui/util/hashmap.h"
#include "include/cgui/util/vector.h"
#include "include/cgui/win32/window.h"
#include "include/cgui/app/core.h"
#include "include/cgui/app/application.h"
#include "include/cgui/component/ui_factory.h"
#include "include/cgui/component/ui_window.h"
#include "include/cgui/component/ui_button.h"
#include "include/cgui/component/ui_label.h"
#include "include/cgui/component/ui_textbox.h"
#include "include/cgui/component/ui_list.h"

CGUI_UINativeWindow* wnd;
CGUI_UINativeLabel* label;
CGUI_UINativeLabel* label2;
CGUI_UINativeListView* listView;

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
        btn->setText(btn, "Checked!");
        label->setGeometry(label, cgui_createRectangle(random(0, 400), random(0, 400), 100, 100));
        label->setText(label, "Checked!");
    } else {
        btn->setButtonState(btn, CGUI_ButtonState_Unchecked);
        btn->setText(btn, "Unchecked!");
        label->setText(label, "Unchecked!");

        CGUI_ListViewItems items = listView->getSelectedItem(listView);
        for (int i = 0; i < items.count; i++) {
            CGUI_ListViewItem item = items.items[i];
            printf("Selected Item %d: %s\n", i, item.text);
        }
    }
    label2->setText(label2, cgui_digitToString(cnt++));
}

void onTextChanged(CGUI_TextBoxEventArgs args) {
    printf("Text length: %d\n", args.textLength);
    CGUI_TextBoxAcquisitionHandle h = args.acquisitionHandle;
    char* buf;
    h.acquire(&h, &buf);
    assert(strlen(buf) == args.textLength);
    free(buf);
}

void onItemSelected(CGUI_ListViewSelectedEventArgs args) {
    CGUI_ListViewItems items = args.acquisitionHandle.acquire(&args.acquisitionHandle);
    for (int i = 0; i < items.count; i++) {
        CGUI_ListViewItem item = items.items[i];
        printf("Called onItemSelected! Item %d: %s\n", i, item.text);
    }
}

int main(void) {
    CGUI_RuntimeContext* ctx = cgui_defaultRuntimeContext();
    CGUI_Application* app = app_ctor(ctx);

    CGUI_UIFactoryCluster* uiFactory = cgui_createUIFactoryCluster();

    CGUI_WindowClassOptions options = cgui_defaultWindowOptions("CGUI_WINDOW");
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
                    .realBackgroundColor = cgui_transparentColor(),//cgui_rgbaToColor(255, 0, 255),
            },
            .alignHorizontal = CGUI_TextAlignmentH_Center,
            .alignVertical = CGUI_TextAlignmentV_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
    };

    CGUI_GDITextContext gdiTextContext2 = {
            .fontStyle = {
                    .fontName = "Segoe UI",
                    .fontSize = 20,
                    .foregroundColor = cgui_rgbaToColor(0, 0, 255),
                    .backgroundColor = cgui_rgbaToColor(255, 255, 0),
                    .realBackgroundColor = cgui_transparentColor(),//cgui_rgbaToColor(255, 255, 255),
            },
            .alignHorizontal = CGUI_TextAlignmentH_Center,
            .alignVertical = CGUI_TextAlignmentV_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
    };

    CGUI_GDITextContext* ctxTextDisp = cgui_createGdiTextContextFromInstance(gdiTextContext);
    CGUI_GDITextContext* ctxTextDisp2 = cgui_createGdiTextContextFromInstance(gdiTextContext2);

    CGUI_ButtonOptions buttonOptions = {
            .geometry = {
                    .x = 10,
                    .y = 10,
                    .width = 100,
                    .height = 30
            },
            .parent = wnd->component,
            .text = "Click me!",
            .buttonType = CGUI_ButtonType_CheckBox,
            .defaultState = true,
            .hasBorder = true,
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
            .hasBorder = true,
            .parent = wnd->component,
            .text = "Label",
    };
    label = unwrap(uiFactory->createComponent(uiFactory, "Label", 1, into_box(&labelOptions)));
    //wnd->addChild(wnd, label->component);

    label->setVisible(label, true);
    label->setTextDisplay(label, ctxTextDisp);

    CGUI_LabelOptions labelOptions2 = {
            .geometry = {
                    .x = 50,
                    .y = 120,
                    .width = 300,
                    .height = 150
            },
            .hasBorder = true,
            .parent = wnd->component,
            .text = "Label2",
            .displayScrollBarH = false,
            .displayScrollBarV = true,
    };

    label2 = unwrap(uiFactory->createComponent(uiFactory, "Label", 1, into_box(&labelOptions2)));
    //wnd->addChild(wnd, label2->component);

    label2->setTextDisplay(label2, ctxTextDisp);
    label2->setVisible(label2, true);

    CGUI_TextBoxOptions textBoxOptions = {
            .geometry = {
                    .x = 200,
                    .y = 10,
                    .width = 300,
                    .height = 150
            },
            .parent = wnd->component,
            .text = "TextBox",
            .allowAutoScrollH = true,
            .allowAutoScrollV = true,
            .allowMultiline = true,
            .displayScrollBarH = false,
            .displayScrollBarV = true,
            .isPassword = false,
            .hasBorder = true,
            .isReadOnly = false
    };

    CGUI_UINativeTextBox* textBox = unwrap(uiFactory->createComponent(uiFactory, "TextBox", 1, into_box(&textBoxOptions)));

    textBox->setTextDisplay(textBox, ctxTextDisp);
    textBox->setVisible(textBox, true);
    // into(CGUI_TextBoxHandler, textBox->component->eventHandler->localHandler)->onTextChanged = onTextChanged;

    //wnd->ready(wnd);

    CGUI_ListViewOptions listViewOptions = {
            .geometry = {
                    .x = 10,
                    .y = 250,
                    .width = 300,
                    .height = 150
            },
            .parent = wnd->component,
            .selectionMode = CGUI_ListViewSelectionMode_Single,
            .displayScrollBarV = true,
            .displayScrollBarH = false,
            .hasBorder = true,
            .hasComboBox = false,
    };
    listView = unwrap(uiFactory->createComponent(uiFactory, "ListView", 1, into_box(&listViewOptions)));
    listView->setVisible(listView, true);
    listView->setTextDisplay(listView, ctxTextDisp2);

    into(CGUI_ListViewHandler, listView->component->eventHandler->localHandler)->onItemSelected = onItemSelected;

    listView->appendItem(listView, "Item 1");
    listView->appendItem(listView, "Item 2");
    listView->insertItem(listView, "Item 3", (CGUI_ListViewItemSelector) {
            .selectorType = CGUI_ListViewItemSelectorType_Index,
            .inner.idx = 1
    });
    listView->insertItem(listView, "Item 4", (CGUI_ListViewItemSelector) {
            .selectorType = CGUI_ListViewItemSelectorType_Name,
            .inner.name = "Item 1"
    });

    CGUI_ListViewItemSelector itemSelector = {
            .selectorType = CGUI_ListViewItemSelectorType_Name,
            .inner.name = "Item 1"
    };
    listView->removeItem(listView, itemSelector);

    cgui_destroyUIFactoryCluster(uiFactory);

    app->run(false);

    return 0;
}
