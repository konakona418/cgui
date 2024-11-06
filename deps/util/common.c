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


CGUI_Result create_ok(void* value)
{
    CGUI_Result result;
    result.type = CGUI_RESULT_TYPE_OK;
    result.value = value;
    return result;
}

CGUI_Result create_err(CGUI_Error* error)
{
    CGUI_Result result;
    result.type = CGUI_RESULT_TYPE_ERR;
    result.value = error;
    return result;
}

bool is_ok(CGUI_Result result)
{
    return result.type == CGUI_RESULT_TYPE_OK;
}

bool is_err(CGUI_Result result)
{
    return result.type == CGUI_RESULT_TYPE_ERR;
}

void* unwrap_or(CGUI_Result result, void* defaultValue)
{
    if (is_ok(result))
    {
        return result.value;
    }
    destroyError(result.error);
    return defaultValue;
}

void* unwrap(CGUI_Result result)
{
    if (is_ok(result))
    {
        return result.value;
    }
    destroyError(result.error);
    panic("Called `unwrap()` on an `Err` value");
}

void* take(CGUI_Result result) {
    if (is_ok(result))
    {
        void* value = result.value;
        result.value = NULL;
        return value;
    }
    destroyError(result.error);
    return NULL;
}

CGUI_Option create_none(void)
{
    CGUI_Option option;
    option.type = CGUI_OPTION_TYPE_NONE;
    option.value = NULL;
    return option;
}

CGUI_Option create_some(void* value)
{
    CGUI_Option option;
    option.type = CGUI_OPTION_TYPE_SOME;
    option.value = value;
    return option;
}

bool is_some(CGUI_Option option)
{
    return option.type == CGUI_OPTION_TYPE_SOME;
}

bool is_none(CGUI_Option option)
{
    return option.type == CGUI_OPTION_TYPE_NONE;
}

void* unwrap_option(CGUI_Option option)
{
    if (is_some(option))
    {
        return option.value;
    }
    panic("Called `unwrap()` on a `None` value");
}

void* unwrap_option_or(CGUI_Option option, void* defaultValue)
{
    if (is_some(option))
    {
        return option.value;
    }
    return defaultValue;
}

void* take_option(CGUI_Option option)
{
    if (is_some(option))
    {
        void* value = option.value;
        option.value = NULL;
        return value;
    }
    return NULL;
}
