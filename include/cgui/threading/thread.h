//
// Created on: 2024/11/12, 16:06.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_THREAD_H
#define CGUI_THREAD_H

#include <windows.h>

typedef struct Thread CGUI_Thread;

typedef struct Thread {
    HANDLE thread_handle;
    DWORD thread_id;

    void* (* thread_func)(void*);

    void (* join)(CGUI_Thread* thread);

    void (* joinTimeout)(CGUI_Thread* thread, DWORD milliseconds);
} CGUI_Thread;

CGUI_Thread* cgui_createThread(void* (* thread_func)(void*), void* arg);

void cgui_thread_joinThread(CGUI_Thread* thread);

void cgui_thread_joinThreadTimeout(CGUI_Thread* thread, DWORD milliseconds);

void cgui_thread_forceKill(CGUI_Thread* thread);

void cgui_destroyThread(CGUI_Thread* thread);

#endif //CGUI_THREAD_H
