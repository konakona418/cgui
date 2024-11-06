//
// Created on: 2024/11/6, 21:51.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_OBSOLETE_H
#define CGUI_OBSOLETE_H

#include "common.h"

typedef struct Result {
    union {
        void* value;
        CGUI_Error* error;
    };
    CGUI_ResultType type;
} CGUI_Result;

void* unwrap(CGUI_Result result);
void* unwrap_or(CGUI_Result result, void* defaultValue);

CGUI_Result create_ok(void* value);
CGUI_Result create_err(CGUI_Error* error);

bool is_ok(CGUI_Result result);
bool is_err(CGUI_Result result);

typedef struct Option {
    void* value;
    CGUI_OptionType type;
} CGUI_Option;

void* unwrap_option(CGUI_Option option);
void* unwrap_option_or(CGUI_Option option, void* defaultValue);

CGUI_Option create_some(void* value);
CGUI_Option create_none(void);

bool is_some(CGUI_Option option);
bool is_none(CGUI_Option option);

#endif //CGUI_OBSOLETE_H
