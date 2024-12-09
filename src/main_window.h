//
// Created on: 2024/12/9, 18:21.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef DEMO_MAIN_WINDOW_H
#define DEMO_MAIN_WINDOW_H

#include <stdio.h>
#include "cgui/prelude.h"
#include "resources.h"

CGUI_UIFactoryCluster* uiFactory;

CGUI_UINativeWindow* wnd;
CGUI_UINativeLabel* labelTitle;
CGUI_UINativeLabel* labelCount;
CGUI_UINativeButton* button;

int cnt = 0;
int maxRecord = 0;
const int INITIAL_GIBBERISH = 42;
const int MAX_GIBBERISH = 100;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 500;

void saveData(int score) {
    FILE* fd = fopen("data.txt", "w");
    fprintf(fd, "%d", score);
    fclose(fd);
}

int readData() {
    FILE* fd = fopen("data.txt", "r");
    int score;
    if (fd == NULL) {
        return 0;
    }

    fscanf(fd, "%d", &score);
    fclose(fd);
    return score;
}

void window_onClose(CGUI_EventArgs args) {
    wnd->close(wnd);
    wnd->destroy(wnd);
    saveData(maxRecord);
    cgui_applicationInstance()->stop();
}

void window_onClick(CGUI_MouseEventArgs args) {
    printf("Clicked @ (%ld,%ld)!\n", args.point.x, args.point.y);
}

void button_onClick(CGUI_MouseEventArgs args) {
    CGUI_GDITextContext gdiTextContext = generateRandomColorScheme();
    CGUI_GDITextContext* gdiTextContextPtr = cgui_createGdiTextContextFromInstance(gdiTextContext);

    if (cnt + INITIAL_GIBBERISH <= MAX_GIBBERISH) {
        CGUI_GDITextContext randomColorScheme = generateRandomColorScheme();
        CGUI_GDITextContext* randomColorSchemePtr = cgui_createGdiTextContextFromInstance(randomColorScheme);
        CGUI_UINativeLabel* label = generateGibberish(wnd->component, cgui_createRectangle(500, 450, 350, 100));
        label->setTextDisplay(label, randomColorSchemePtr);
    }

    button->setGeometry(button,
                        cgui_createRectangle(
                                random(0, WINDOW_WIDTH - 180),
                                random(0, WINDOW_HEIGHT - 80),
                                150, 50));
    button->setTextDisplay(button, gdiTextContextPtr);

    labelCount->setText(labelCount, cgui_digitToString(++cnt));
    labelCount->setGeometry(labelCount,
                            cgui_createRectangle(
                                    random(0, WINDOW_WIDTH - 120),
                                    random(0, WINDOW_HEIGHT - 120),
                                    100, 50));

    if (cnt > maxRecord) {
        maxRecord = cnt;
    }
    char* titleBase = malloc(256);
    strcpy(titleBase, "Cookie Clicker - max record: ");
    const char* title = cgui_digitToString(maxRecord);
    strcat(titleBase, title);
    strcat(titleBase, "; current score: ");
    strcat(titleBase, cgui_digitToString(cnt));
    wnd->setWindowName(wnd, titleBase);
    free(titleBase);

}

void registerWindow() {
    CGUI_WindowClassOptions options = cgui_defaultWindowOptions("CGUI_WINDOW");
    options.geometry.width = WINDOW_WIDTH;
    options.geometry.height = WINDOW_HEIGHT;
    options.hasMaximizeButton = false;
    wnd = unwrap(uiFactory->createComponent(uiFactory, "Window", 1, into_box(&options)));

    CGUI_WindowHandler* wndHandler = cgui_getComponentHandler(CGUI_WindowHandler, wnd);
    wndHandler->onClose = window_onClose;
    wndHandler->onMouseUp = window_onClick;

    int score = readData();
    maxRecord = score;
    char* titleBase = malloc(256);
    strcpy(titleBase, "Cookie Clicker - max record: ");
    const char* title = cgui_digitToString(score);
    strcat(titleBase, title);
    wnd->setWindowName(wnd, titleBase);
    free(titleBase);

    wnd->show(wnd);
}

void generateTitleText() {
    CGUI_GDITextContext gdiTextContext = {
            .fontStyle = {
                    .fontName = "Consolas",
                    .fontSize = 42,
                    .foregroundColor = cgui_rgbaToColor(255, 0, 0),
                    .backgroundColor = cgui_rgbaToColor(0, 255, 255),
                    .realBackgroundColor = cgui_rgbaToColor(255, 0, 255),
            },
            .alignHorizontal = CGUI_TextAlignmentH_Center,
            .alignVertical = CGUI_TextAlignmentV_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
    };

    CGUI_GDITextContext* gdiTextContextPtr = cgui_createGdiTextContextFromInstance(gdiTextContext);

    CGUI_LabelOptions labelOptions = {
            .geometry = {
                    .x = 100,
                    .y = 50,
                    .width = 500,
                    .height = 100
            },
            .parent = wnd->component,
            .text = "COOKIE CLICKER FINAL REDUX PRO MAX!",
            .hasBorder = true,
    };

    labelTitle = unwrap(uiFactory->createComponent(uiFactory, "Label", 1, into_box(&labelOptions)));
    labelTitle->setTextDisplay(labelTitle, gdiTextContextPtr);
    labelTitle->setVisible(labelTitle, true);

    cgui_setComponentAsTopmost(labelTitle);
}

void generateLabelCount() {
    CGUI_GDITextContext gdiTextContext = {
            .fontStyle = {
                    .fontName = "Consolas",
                    .fontSize = 25,
                    .foregroundColor = cgui_rgbaToColor(0, 255, 128),
                    .backgroundColor = cgui_rgbaToColor(255, 0, 255),
                    .realBackgroundColor = cgui_rgbaToColor(255, 0, 255),
            },
            .alignHorizontal = CGUI_TextAlignmentH_Center,
            .alignVertical = CGUI_TextAlignmentV_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
            .multiLine = true,
    };

    CGUI_GDITextContext* gdiTextContextPtr = cgui_createGdiTextContextFromInstance(gdiTextContext);

    CGUI_LabelOptions labelOptions = {
            .geometry = {
                    .x = 100,
                    .y = 250,
                    .width = 100,
                    .height = 100
            },
            .parent = wnd->component,
            .text = "0",
            .hasBorder = true,
    };

    labelCount = unwrap(uiFactory->createComponent(uiFactory, "Label", 1, into_box(&labelOptions)));
    labelCount->setTextDisplay(labelCount, gdiTextContextPtr);
    labelCount->setVisible(labelCount, true);
    cgui_setComponentAsTopmost(labelCount);
}

void generateButton() {
    CGUI_GDITextContext gdiTextContext = {
            .fontStyle = {
                    .fontName = "Consolas",
                    .fontSize = 30,
                    .foregroundColor = cgui_rgbaToColor(0, 0, 255),
                    .backgroundColor = cgui_rgbaToColor(255, 255, 0),
                    .realBackgroundColor = cgui_rgbaToColor(255, 255, 0),
            },
            .alignHorizontal = CGUI_TextAlignmentH_Center,
            .alignVertical = CGUI_TextAlignmentV_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
            .multiLine = true,
    };

    CGUI_GDITextContext* gdiTextContextPtr = cgui_createGdiTextContextFromInstance(gdiTextContext);

    CGUI_ButtonOptions buttonOptions = {
            .geometry = {
                    .x = 325,
                    .y = 200,
                    .width = 150,
                    .height = 50
            },
            .parent = wnd->component,
            .text = "Click me!",
            .hasBorder = true,
            .buttonType = CGUI_ButtonType_Default,
    };

    button = unwrap(uiFactory->createComponent(uiFactory, "Button", 1, into_box(&buttonOptions)));
    button->setTextDisplay(button, gdiTextContextPtr);
    button->setVisible(button, true);
    cgui_getComponentHandler(CGUI_ButtonHandler, button)->onClick = button_onClick;
    cgui_getComponentHandler(CGUI_ButtonHandler, button)->onDoubleClick = button_onClick;

    cgui_setComponentAsTopmost(button);
}

int bootProgram(void) {
    CGUI_RuntimeContext* ctx = cgui_defaultRuntimeContext();
    CGUI_Application* app = app_ctor(ctx);

    uiFactory = cgui_createUIFactoryCluster();
    uiFactory->config.forceUpdateParent = false;

    registerWindow();

    CGUI_GDITextContext randomColorScheme[INITIAL_GIBBERISH];
    for (int i = 0; i < INITIAL_GIBBERISH; i++) {
        randomColorScheme[i] = generateRandomColorScheme();
    }

    for (int i = 0; i < INITIAL_GIBBERISH; i++) {
        CGUI_UINativeLabel* label = generateGibberish(wnd->component, cgui_createRectangle(500, 450, 350, 30));
        label->setTextDisplay(label, cgui_createGdiTextContextFromInstance(randomColorScheme[i]));
        // wnd->update(wnd);
    }

    generateTitleText();
    generateLabelCount();
    generateButton();

    app->run(false);

    return 0;
}

#endif //DEMO_MAIN_WINDOW_H
