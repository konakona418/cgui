//
// Created on: 2024/11/6, 13:42.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_COMMON_H
#define CGUI_COMMON_H

#include <assert.h>
#include <stdio.h>

/* This macro is intended for panicking. */
#define panic(msg) (panic_impl(msg, __FILE_NAME__, __LINE__))

/* This macro is intended for panicking. */
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
 * Otherwise, the macro will continue to the next line.
 * Note that using this macro may lead to danging pointers. */
#define bubble(_R) if ((_R).type == CGUI_RESULT_TYPE_ERR) { \
    return _R;                                              \
} else {                                                    \
    unwrap(_R);                                             \
}

/* This macro is intended for returning an option from a function.
 * When the option is none, the function will return immediately.
 * Otherwise, the macro will continue to the next line.
 * Note that using this macro may lead to danging pointers. */
#define bubble_option(_O) if ((_O).type == CGUI_OPTION_TYPE_NONE) { \
    return _O;                                                      \
} else {                                                            \
    unwrap_option(_O);                                              \
}

int random(int min, int max);

/* This type is intended for representing a boolean value. */
typedef int bool;

#define true 1

#define false 0

/* Prototypes of functions and types. */
typedef enum ResultType CGUI_ResultType;

typedef enum OptionType CGUI_OptionType;

typedef struct Error CGUI_Error;

typedef struct Result CGUI_Result;

typedef struct Option CGUI_Option;

/* This enum is intended for representing the type of Result. */
typedef enum ResultType {
    CGUI_RESULT_TYPE_OK,
    CGUI_RESULT_TYPE_ERR
} CGUI_ResultType;

/* This enum is intended for representing the type of Option. */
typedef enum OptionType {
    CGUI_OPTION_TYPE_NONE,
    CGUI_OPTION_TYPE_SOME
} CGUI_OptionType;

/* This struct is intended for representing an error. */
typedef struct Error {
    int code;
    const char* message;
} CGUI_Error;

/* Creates an error. */
CGUI_Error* createError(int code, const char* message);

/* Creates an error from another error. */
CGUI_Error* createErrorFromError(CGUI_Error* error);

/* Creates an error. */
CGUI_Error createErrorInstance(int code, const char* message);

/* Creates an error from another error. */
CGUI_Error createErrorInstanceFromError(CGUI_Error* error);

/* Destroys an error. */
void destroyError(CGUI_Error* error);

/* This struct is intended for representing a result. */
typedef struct Result {
    union {
        void* value;
        CGUI_Error* error;
    };
    CGUI_ResultType type;
} CGUI_Result;

/* Unwraps a result.
 * If the result is an error, the function will panic.
 * Otherwise, the function will return the value.*/
void* unwrap(CGUI_Result result);

/* Unwraps a result or returns a default value.
 * If the result is an error, the function will return the default value.
 * Otherwise, the function will return the value.*/
void* unwrap_or(CGUI_Result result, void* defaultValue);

/* Takes a result.
 * If the result is an error, the function will simply return **NULL**.
 * Otherwise, the function will return the value.*/
void* take(CGUI_Result* result);

/* Creates an ok result. */
CGUI_Result create_ok(void* value);

/* Creates an error result. */
CGUI_Result create_err(CGUI_Error* error);

/* Checks if the result is an ok result. */
bool is_ok(CGUI_Result* result);

/* Checks if the result is an error result. */
bool is_err(CGUI_Result* result);

/* This struct is intended for representing an option. */
typedef struct Option {
    void* value;
    CGUI_OptionType type;
} CGUI_Option;

/* Unwraps an option.
 * If the option is none, the function will panic.
 * Otherwise, the function will return the value.*/
void* unwrap_option(CGUI_Option option);

/* Unwraps an option or returns a default value.
 * If the option is none, the function will return the default value.
 * Otherwise, the function will return the value.*/
void* unwrap_option_or(CGUI_Option option, void* defaultValue);

/* Takes an option.
 * If the option is none, the function will simply return **NULL**.
 * Otherwise, the function will return the value.*/
void* take_option(CGUI_Option* option);

/* Creates a some option. */
CGUI_Option create_some(void* value);

/* Creates a none option. */
CGUI_Option create_none(void);

/* Checks if the option is some. */
bool is_some(CGUI_Option* option);

/* Checks if the option is none. */
bool is_none(CGUI_Option* option);


#endif //CGUI_COMMON_H
