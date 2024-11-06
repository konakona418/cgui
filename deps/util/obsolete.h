//
// Created on: 2024/11/6, 21:51.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_OBSOLETE_H
#define CGUI_OBSOLETE_H

#include "common.h"


#define CGUI_Option_Define(_T) typedef struct Option_##_T {    \
    _T* value;                                                 \
    CGUI_OptionType type;                                      \
} CGUI_Option_##_T;

#define CGUI_Option_Prototype(_T) typedef struct Option_##_T CGUI_Option_##_T;

#define CGUI_Option(_T) CGUI_Option_##_T

#define CGUI_Some(_T, _V) (CGUI_Option_##_T) {.type = CGUI_OPTION_TYPE_SOME, .value = _V}
#define CGUI_None(_T) (CGUI_Option_##_T) {.type = CGUI_OPTION_TYPE_NONE, .value = ((__typeof__(_T)) NULL)}

#define CGUI_Option_destroy(_O)        \
    ({if (cgui_is_some(_O)) {            \
        free((_O).value);              \
        (_O).value = NULL;             \
    }})


#define cgui_unwrap_option(_O) \
((_O).type == CGUI_OPTION_TYPE_SOME ? (_O).value : (panic("Attempted to unwrap a None value."), ((__typeof__((_O).value)) NULL)))

#define cgui_unwrap_option_or(_O, _D) \
((_O).type == CGUI_OPTION_TYPE_SOME ? ((__typeof__((_O).value)) (_O).value) : ((__typeof__((_O).value)) (_D)))

#define cgui_take_option(_O)                    \
    ({                                          \
        __typeof__(_O.value) _tmp = NULL;       \
        if (cgui_is_some(_O)) {                 \
            _tmp = (_O).value;                  \
            (_O).value = NULL;                  \
            (_O).type = CGUI_OPTION_TYPE_NONE;              \
        }                                       \
        _tmp;                                   \
    })

#define cgui_is_some(_O) (_O).type == CGUI_OPTION_TYPE_SOME

#define cgui_is_none(_O) (_O).type == CGUI_OPTION_TYPE_NONE

#define CGUI_Option_NonPtr_Define(_T) typedef struct Option_##_T {    \
    _T value;                                                                   \
    CGUI_OptionType type;                                                       \
} CGUI_Option_##_T;

#define CGUI_Option_NonPtr_Prototype(_T) typedef struct Option_##_T CGUI_Option_##_T;

#define CGUI_Option_NonPtr(_T) CGUI_Option_##_T

#define CGUI_Option_NonPtr_destroy(_O)        \
    ({if (cgui_is_some(_O)) {                   \
        (_O).value = NULL;                    \
    }})

#define cgui_unwrap(_R) cgui_unwrap_result(_R)
#define cgui_unwrap_or(_R, _O) cgui_unwrap_result_or(_R, _O)
#define cgui_take(_R) cgui_take_result(_R)

#define CGUI_Capsule_Define(_T) typedef struct Capsule_##_T {    \
    _T value;                                                             \
} CGUI_Capsule_##_T;

#define CGUI_Capsule_Prototype(_T) typedef struct Capsule_##_T CGUI_Capsule_##_T;

#define capsule CGUI_Capsule_

#define CGUI_Capsule(_T) CGUI_Capsule_##_T

#define encapsulate(_T, _V) (CGUI_Capsule_##_T) {.value = _V}

#define decapsulate(_Capsule) (*_Capsule).value

#define ref(_T, _V) ((_T*) _V)

#define deref(_T, _V) (*((_T*) _V))

/* This macro is applied to create the definition for a Result type.
 * The type created will have an internal name of CGUI_Result_##T. \
 * Note that the data stored in this structure is a pointer. */
#define CGUI_Result_Define(_T, _E) typedef struct Result_##_T {    \
    union {                                                         \
             _T* value;                                             \
             _E* error;                                             \
    };                                                              \
    CGUI_ResultType type;                                           \
} CGUI_Result_##_T;

/* This macro is applied to create the prototype for a Result type. */
#define CGUI_Result_Prototype(_T, _E) typedef struct Result_##_T CGUI_Result_##_T;

/* This macro is applied to symbolize a Result type. */
#define CGUI_Result(_T) CGUI_Result_##_T

#define CGUI_Ok(_T, _V) (CGUI_Result_##_T) {.type = CGUI_RESULT_TYPE_OK, .value = _V}
#define CGUI_Err(_T, _E) (CGUI_Result_##_T) {.type = CGUI_RESULT_TYPE_ERR, .error = createErrorFromError(_E)}

#define CGUI_Result_destroy(_R)        \
    ({if (cgui_is_err(_R)) {             \
        destroyError((_R).error);      \
        (_R).error = NULL;             \
    }})

#define cgui_unwrap_result_no_destroy(_R) \
((_R).type == CGUI_RESULT_TYPE_OK ? (_R).value : (panic("Attempted to unwrap an Err value."), NULL))

#define cgui_unwrap_result(_R)    \
    ((_R).type == CGUI_RESULT_TYPE_OK ? (_R).value :      \
    (CGUI_Result_destroy(_R), panic("Attempted to unwrap an Err value."), (__typeof__((_R).value)) NULL))

#define cgui_unwrap_result_or(_R, _O) \
((_R).type == CGUI_RESULT_TYPE_OK ? ((__typeof__(_R.value)) (_R).value) : (CGUI_Result_destroy(_R), ((__typeof__((_R).value)) _O)))

#define cgui_is_ok(_R) (_R).type == CGUI_RESULT_TYPE_OK

#define cgui_is_err(_R) (_R).type == CGUI_RESULT_TYPE_ERR

#define cgui_take_result(_R)                    \
    ({                                          \
        __typeof__(_R.value) _tmp = NULL;       \
        if (cgui_is_ok(_R)) {                   \
            _tmp = (_R).value;                  \
            (_R).value = NULL;                  \
            (_R).type = CGUI_ERR;               \
        } else {                                \
            CGUI_Result_Destroy(_R);            \
        }                                       \
        _tmp;                                   \
    })

#define CGUI_Result_NonPtr_Define(_T, _E) typedef struct Result_##_T {    \
    union {                                                         \
             _T value;                                             \
             _E* error;                                             \
    };                                                              \
    CGUI_ResultType type;                                           \
} CGUI_Result_##_T;

#define CGUI_Result_NonPtr_Prototype(_T, _E) typedef struct Result_##_T CGUI_Result_##_T;

#define CGUI_Result_NonPtr(_T) CGUI_Result_##_T

#define CGUI_Result_NonPtr_destroy(_R)        \
    ({if (cgui_is_err(_R)) {                    \
        (_R).error = NULL;                    \
    }})




/* The following structures are pre-defined, for they might be used on various occasions. */

/* This is the Result type used for functions that do not return a value. */
CGUI_Result_Prototype(void, CGUI_Error)
CGUI_Result_Define(void, CGUI_Error);


#endif //CGUI_OBSOLETE_H
