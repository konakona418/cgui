//
// Created on: 2024/11/6, 13:42.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <malloc.h>
#include <time.h>
#include "common.h"

static int random_initialized = 0;

void init_random() {
    srand((unsigned) time(NULL));
}

int random(int min, int max) {
    if (!random_initialized) {
        init_random();
        random_initialized = 1;
    }
    return min + rand() % (max - min + 1);
}

LPCSTR cgui_concatString(LPCSTR str1, LPCSTR str2) {
    unsigned long long len1 = strlen(str1);
    unsigned long long len2 = strlen(str2);
    char* result = malloc(len1 + len2 + 1);
    strcpy(result, str1);
    strcpy(result + len1, str2);
    return result;
}

LPCSTR cgui_toLowercase(LPCSTR str) {
    unsigned long long len = strlen(str);
    char* result = strdup(str);
    for (int i = 0; i < len; ++i) {
        if (str[i] == '\0') {
            break;
        }
        if (str[i] >= 'A' && str[i] <= 'Z') {
            result[i] += 32;
        }
    }
    result[len] = '\0';
    return result;
}

LPCSTR cgui_toUppercase(LPCSTR str) {
    unsigned long long len = strlen(str);
    char* result = strdup(str);
    for (int i = 0; i < len; ++i) {
        if (str[i] == '\0') {
            break;
        }
        if (str[i] >= 'a' && str[i] <= 'z') {
            result[i] -= 32;
        }
    }
    result[len] = '\0';
    return result;
}

LPCSTR cgui_digitToString(long long digit) {
    char* buffer = malloc(sizeof(char) * 32);
    sprintf(buffer, "%lld", digit);
    const char* result = strdup(buffer);
    free(buffer);
    return result;
}

bool eq_any(int value, size_t count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        if (value == va_arg(args, int)) {
            return true;
        }
    }

    va_end(args);
    return false;
}

CGUI_Box* into_box(void* ptr) {
    CGUI_Box* box = malloc(sizeof(CGUI_Box));
    box->_inner = ptr;
    return box;
}

CGUI_Box* clone_box(CGUI_Box* box) {
    return into_box(box->_inner);
}

void* unbox(CGUI_Box* box) {
    void* ptr = box->_inner;
    //free(box);
    return ptr;
}

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


CGUI_Result create_ok(void* value) {
    CGUI_Result result;
    result.type = CGUI_RESULT_TYPE_OK;
    result.value = value;
    return result;
}

CGUI_Result create_err(CGUI_Error* error) {
    CGUI_Result result;
    result.type = CGUI_RESULT_TYPE_ERR;
    result.value = error;
    return result;
}

bool is_ok(CGUI_Result* result) {
    return result->type == CGUI_RESULT_TYPE_OK;
}

bool is_err(CGUI_Result* result) {
    return result->type == CGUI_RESULT_TYPE_ERR;
}

void* unwrap_or(CGUI_Result result, void* defaultValue) {
    if (is_ok(&result)) {
        return result.value;
    }
    if (result.error != NULL) {
        destroyError(result.error);
    }
    return defaultValue;
}

void* unwrap(CGUI_Result result) {
    if (is_ok(&result)) {
        return result.value;
    }
    if (result.error != NULL) {
        printf("Fatal error occurred, details: %s\n", result.error->message);
        destroyError(result.error);
    }
    panic("Called `unwrap()` on an `Err` value.");
}

void* take(CGUI_Result* result) {
    if (is_ok(result)) {
        void* value = result->value;
        result->value = NULL;
        result->type = CGUI_RESULT_TYPE_ERR;
        result->error = NULL;
        return value;
    }
    destroyError(result->error);
    result->error = NULL;
    return NULL;
}

CGUI_Option create_none(void) {
    CGUI_Option option;
    option.type = CGUI_OPTION_TYPE_NONE;
    option.value = NULL;
    return option;
}

CGUI_Option create_some(void* value) {
    CGUI_Option option;
    option.type = CGUI_OPTION_TYPE_SOME;
    option.value = value;
    return option;
}

bool is_some(CGUI_Option* option) {
    return option->type == CGUI_OPTION_TYPE_SOME;
}

bool is_none(CGUI_Option* option) {
    return option->type == CGUI_OPTION_TYPE_NONE;
}

void* unwrap_option(CGUI_Option option) {
    if (is_some(&option)) {
        return option.value;
    }
    panic("Called `unwrap()` on a `None` value");
}

void* unwrap_option_or(CGUI_Option option, void* defaultValue) {
    if (is_some(&option)) {
        return option.value;
    }
    return defaultValue;
}

void* take_option(CGUI_Option* option) {
    if (is_some(option)) {
        void* value = option->value;
        option->value = NULL;
        option->type = CGUI_OPTION_TYPE_NONE;
        return value;
    }
    return NULL;
}

CGUI_Singleton* cgui_createSingleton() {
    CGUI_Singleton* ptr = malloc(sizeof(CGUI_Singleton));
    ptr->isInitialized = false;
    ptr->value = NULL;
    return ptr;
}

void cgui_initSingleton(CGUI_Singleton* singleton, void* value) {
    singleton->isInitialized = true;
    singleton->value = value;
}

void* cgui_getSingletonValue(CGUI_Singleton* singleton) {
    return singleton->value;
}

bool cgui_isSingletonInitialized(CGUI_Singleton* singleton) {
    return singleton->isInitialized;
}

void cgui_destroySingleton(CGUI_Singleton* singleton) {
    free(singleton);
}
