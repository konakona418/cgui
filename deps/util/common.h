//
// Created on: 2024/11/6, 13:42.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_COMMON_H
#define CGUI_COMMON_H

#include <assert.h>
#include <stdio.h>
#include <windows.h>

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
#define deref(_T, _V) (*((_T*) _V))

/* This macro is intended for creating a pointer type from a primitive type. */
#define as_ptr(_T, _V) ({ \
_T* _primitive_ptr = malloc(sizeof(_T));               \
*_primitive_ptr = _V;                                  \
_primitive_ptr;                                        \
})

/* This macro is intended for casting any pointer type into another type. */
#define into(_T, _V) ((_T*)_V)

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

/* This macro is intended for better branch prediction.
 * As this macro utilizes the GNU extension __builtin_expect,
 * it may not work on other compilers.
 * Well the optimization is little, but it's pretty cool anyway lol. */
#ifdef __GNUC__
#define unlikely(_Condition) (__builtin_expect(!!(_Condition), 0))
#define likely(_Condition) (__builtin_expect(!!(_Condition), 1))

#else
#define unlikely(_Condition) (_Condition)
#define likely(_Condition) (_Condition)
#endif

/* This macro is intended for indicating that the return value of a function should be used. */
#define nodiscard __attribute__((warn_unused_result))

int random(int min, int max);

/* This type is intended for representing a boolean value. */
typedef int bool;

/* This type is intended for representing an exit flag of an iterator.
 * When set to true, the iterator will stop iterating. */
typedef bool IterExitFlag;

/* This type is intended for representing the result of a predicate function.
 * When set to false, will continue to the next item.
 * When set to true, will stop iterating. */
typedef bool IterPredicateResult;

#define true 1

#define false 0

LPCSTR cgui_concatString(LPCSTR str1, LPCSTR str2);
LPCSTR cgui_toLowercase(LPCSTR str);
LPCSTR cgui_toUppercase(LPCSTR str);

/**
 * This type is intended for representing a boxed pointer.
 * It is used to store a pointer to a value.
 * Intended for providing a type with a fixed size to be passed as array parameter. */
typedef struct Box CGUI_Box;

typedef struct Box {
    void* _inner;
} CGUI_Box;

/**
 * Creates a boxed pointer.
 * This function is intended for providing a type with a fixed size to be passed as array parameter.
 * @param ptr The pointer to be boxed.
 * @return A boxed pointer.
 */
CGUI_Box* into_box(void* ptr);

/**
 * Clones a boxed pointer.
 * @param box The boxed pointer.
 * @return A cloned boxed pointer.
 */
CGUI_Box* clone_box(CGUI_Box* box);

/**
 * Unboxes a boxed pointer.
 * @warning This function will deallocate the box.
 * @param box The boxed pointer.
 * @return The unboxed pointer.
 */
void* unbox(CGUI_Box* box);

/**
 * Dereferences a boxed pointer.
 * @param _InnerType The type of the inner value.
 * @param _Box The boxed pointer.
 * @return The dereferenced pointer.
 */
#define deref_box(_InnerType, _Box) (deref(_InnerType, unbox(_Box)))

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

typedef struct Singleton CGUI_Singleton;

typedef struct Singleton {
    void* value;
    bool isInitialized;
} CGUI_Singleton;

/**
 * Creates a singleton.
 * @return singleton.
 */
CGUI_Singleton* cgui_createSingleton();

/**
 * Initializes a singleton.
 * @param singleton singleton.
 * @param value designated value, dynamically allocated.
 */
void cgui_initSingleton(CGUI_Singleton* singleton, void* value);

/**
 * Gets the value of a singleton.
 * @param singleton singleton.
 * @return value.
 */
void* cgui_getSingletonValue(CGUI_Singleton* singleton);

bool cgui_isSingletonInitialized(CGUI_Singleton* singleton);

/**
 * Destroys a singleton.
 * @note this will not deallocate the value.
 * @param singleton singleton.
 */
void cgui_destroySingleton(CGUI_Singleton* singleton);

typedef void (* CGUI_ApplicationMessageCallback)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif //CGUI_COMMON_H
