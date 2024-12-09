//
// Created on: 2024/12/3, 18:19.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef DEMO_PROG_RESOURCES_H
#define DEMO_PROG_RESOURCES_H

#include "cgui/prelude.h"
#include <math.h>

#define NEON_PURPLE cgui_rgbaToColor(148, 0, 211)
#define ELECTRIC_BLUE cgui_rgbaToColor(30, 144, 255)
#define LIME_GREEN cgui_rgbaToColor(50, 205, 50)
#define HOT_PINK cgui_rgbaToColor(255, 20, 147)

#define BRIGHT_ORANGE cgui_rgbaToColor(255, 165, 0)
#define DEEP_PURPLE cgui_rgbaToColor(75, 0, 130)
#define SKY_BLUE cgui_rgbaToColor(0, 191, 255)
#define BRIGHT_YELLOW cgui_rgbaToColor(255, 255, 0)

#define AQUA_GREEN cgui_rgbaToColor(0, 250, 154)
#define PEACH_PINK cgui_rgbaToColor(255, 105, 180)
#define BLACK cgui_rgbaToColor(0, 0, 0)
#define WHITE cgui_rgbaToColor(255, 255, 255)

#define NEON_YELLOW cgui_rgbaToColor(204, 255, 0)
#define MAGENTA cgui_rgbaToColor(255, 0, 255)
#define INDIGO cgui_rgbaToColor(75, 0, 130)
#define CYAN cgui_rgbaToColor(0, 255, 255)

#define MINT_GREEN cgui_rgbaToColor(152, 251, 152)
#define PEACH_ORANGE cgui_rgbaToColor(255, 218, 185)
#define MINT_BLUE cgui_rgbaToColor(175, 238, 238)
#define LIGHT_PURPLE cgui_rgbaToColor(221, 160, 221)

const char* modifiers[] = {
        "Spirit Sign \"Dream Seal\"",
        "Ultra Deluxe Edition",
        "Supreme Plus Turbo",
        "Master Spark",
        "Ultimate Gold Pro",
        "Hyper Mega Power",
        "Advanced Xtreme",
        "Infinite Edition",
        "Magic Sign \"Stardust Reverie\"",
        "Max Velocity",
        "Redux Infinite Turbo",
        "Ice Sign \"Icicle Fall\"",
        "Quantum Boosted Edition",
        "Legendary Chaos Mode",
        "Apocalypse Flavor",
        "Mastermind Prime",
};

CGUI_GDITextContext generateRandomColorScheme() {
    CGUI_Color colors[] = {
            NEON_PURPLE,
            ELECTRIC_BLUE,
            LIME_GREEN,
            HOT_PINK,
            BRIGHT_ORANGE,
            DEEP_PURPLE,
            SKY_BLUE,
            BRIGHT_YELLOW,
            AQUA_GREEN,
            PEACH_PINK,
            BLACK,
            WHITE,
            NEON_YELLOW,
            MAGENTA,
            INDIGO,
            CYAN,
            MINT_GREEN,
            PEACH_ORANGE,
            MINT_BLUE,
            LIGHT_PURPLE
    };

    CGUI_GDITextContext result = {
            .fontStyle = {
                    .fontName = "Consolas",
                    .fontSize = 20,
                    .foregroundColor = colors[random(0, 19)],
                    .backgroundColor = colors[random(0, 19)],
                    .realBackgroundColor = colors[random(0, 19)],
            },
            .alignHorizontal = CGUI_TextAlignmentH_Center,
            .alignVertical = CGUI_TextAlignmentV_Center,
            .orientation = CGUI_TextOrientation_Horizontal,
    };
    return result;
}

CGUI_UINativeLabel* generateGibberish(CGUI_UIComponent* parent, CGUI_Rectangle range) {
    CGUI_UIFactoryCluster* cluster = cgui_createUIFactoryCluster();
    int x = random(0, range.x);
    int y = random(0, range.y);
    int width = random(160, range.width);
    int height = random(30, range.height);

    int gibberishSelector = random(0, 15);
    gibberishSelector = gibberishSelector > 15 ? 0 : gibberishSelector;

    CGUI_LabelOptions options = {
            .displayScrollBarH = false,
            .displayScrollBarV = false,
            .geometry = {
                    .x = x,
                    .y = y,
                    .width = width,
                    .height = height,
            },
            .hasBorder = true,
            .parent = parent,
            .text = modifiers[gibberishSelector],
    };
    CGUI_UINativeLabel* label = unwrap(cluster->createComponent(cluster, "label", 1, into_box(&options)));

    return label;
}

#endif //DEMO_PROG_RESOURCES_H
