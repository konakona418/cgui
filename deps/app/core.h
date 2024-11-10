//
// Created on: 2024/11/7, 14:54.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_CORE_H
#define CGUI_CORE_H

#include "../util/common.h"
#include "../util/error.h"

#include "../win32/window.h"

/* Prototypes */
typedef struct Core CGUI_Core;

typedef struct RuntimeContext CGUI_RuntimeContext;

/* Structures of Core */
typedef struct Core {
    CGUI_RuntimeContext* ctx;

    CGUI_WindowManager* wndManager;
    CGUI_WindowFactory* wndFactory;

    CGUI_WindowClassManager* wndClassManager;
    CGUI_WindowClassFactory* wndClassFactory;
} CGUI_Core;

/* Constructors and Destructors */

/* Create Core */
CGUI_Result cgui_createCore(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);

/* Create Core from RuntimeContext
 * Note that the RuntimeContext will be destroyed by Core.
 * So the Runtime passed in shall be allocated using malloc().
 * Consider using cgui_createRuntimeContext(). */
CGUI_Result cgui_createCoreFromContext(CGUI_RuntimeContext* ctx);

/* Destroy Core */
void cgui_destroyCore(CGUI_Core* core);

/* Structures of RuntimeContext */

typedef struct RuntimeContext {
    HINSTANCE hInstance;
    LPSTR lpCmdLine;
    int nCmdShow;
} CGUI_RuntimeContext;

/* Constructors and Destructors */

/* Create RuntimeContext */
CGUI_Result cgui_createRuntimeContext(HINSTANCE hInstance, LPSTR lpCmdLine, int nCmdShow);

/* Destroy RuntimeContext */
void cgui_destroyRuntimeContext(CGUI_RuntimeContext* ctx);

#endif //CGUI_CORE_H
