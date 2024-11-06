//
// Created on: 2024/11/6, 13:42.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_COMMON_H
#define CGUI_COMMON_H

#include <assert.h>
#include <stdio.h>

#define panic(msg) (panic_impl(msg, __FILE_NAME__, __LINE__))
#define panic_impl(msg, file, line) \
({                                                              \
printf("Program panicked at %s:%d: %s\n", file, line, msg);     \
assert(0);                                                      \
})

/* This macro is intended for casting any type into a pointer type. */
#define ref(_T, _V) (_T*) _V

/* This macro is intended for dereferencing a pointer type into any type. */
#define deref(_T, _V) (*(_T*) _V)

/* This macro is intended for creating a pointer type from a primitive type. */
#define as_ptr(_T, _V) ({ \
_T* _primitive_ptr = malloc(sizeof(_T));               \
*_primitive_ptr = _V;                                  \
_primitive_ptr;                                        \
})

/* This macro is intended for returning a result from a function.
 * When the result is an error, the function will return immediately.
 * Otherwise, the macro will continue to the next line. */
#define bubble(_R) if ((_R).type == CGUI_RESULT_TYPE_ERR) { \
    return _R;                                              \
} else {                                                    \
    unwrap(_R);                                             \
}

/* This macro is intended for returning an option from a function.
 * When the option is none, the function will return immediately.
 * Otherwise, the macro will continue to the next line. */
#define bubble_option(_O) if ((_O).type == CGUI_OPTION_TYPE_NONE) { \
    return _O;                                                      \
} else {                                                            \
    unwrap_option(_O);                                              \
}

typedef int bool;

typedef enum ResultType CGUI_ResultType;

typedef enum ResultType {
    CGUI_RESULT_TYPE_OK,
    CGUI_RESULT_TYPE_ERR
} CGUI_ResultType;

typedef enum OptionType {
    CGUI_OPTION_TYPE_NONE,
    CGUI_OPTION_TYPE_SOME
} CGUI_OptionType;

typedef struct Error {
    int code;
    const char* message;
} CGUI_Error;

CGUI_Error* createError(int code, const char* message);

CGUI_Error* createErrorFromError(CGUI_Error* error);

CGUI_Error createErrorInstance(int code, const char* message);

CGUI_Error createErrorInstanceFromError(CGUI_Error* error);

void destroyError(CGUI_Error* error);

typedef struct Result {
    union {
        void* value;
        CGUI_Error* error;
    };
    CGUI_ResultType type;
} CGUI_Result;

void* unwrap(CGUI_Result result);
void* unwrap_or(CGUI_Result result, void* defaultValue);
void* take(CGUI_Result result);

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
void* take_option(CGUI_Option option);

CGUI_Option create_some(void* value);
CGUI_Option create_none(void);

bool is_some(CGUI_Option option);
bool is_none(CGUI_Option option);


#endif //CGUI_COMMON_H
