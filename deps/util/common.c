//
// Created on: 2024/11/6, 13:42.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <malloc.h>
#include "common.h"

CGUI_Error* createError(int code, const char* msg) {
    CGUI_Error* ptr = malloc(sizeof(CGUI_Error));
    ptr->code = code;
    ptr->message = msg;
    return ptr;
}

CGUI_Error* createErrorFromError(CGUI_Error* error) {
    return createError(error->code, error->message);
}

CGUI_Error createErrorInstance(int code, const char* msg) {
    return (CGUI_Error) {
            .code = code,
            .message = msg
    };
}

CGUI_Error createErrorInstanceFromError(CGUI_Error* error) {
    return (CGUI_Error) {
            .code = error->code,
            .message = error->message
    };
}

void destroyError(CGUI_Error* error) {
    free(error);
}
