//
// Created on: 2024/11/12, 16:06.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "thread.h"

CGUI_Thread* cgui_createThread(void* (* thread_func)(void*), void* arg) {
    CGUI_Thread* thread = (CGUI_Thread*) malloc(sizeof(CGUI_Thread));
    thread->thread_handle = CreateThread(
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) thread_func,
            arg, 0,
            &thread->thread_id);

    thread->join = cgui_thread_joinThread;
    thread->joinTimeout = cgui_thread_joinThreadTimeout;
    return thread;
}

void cgui_thread_joinThread(CGUI_Thread* thread) {
    WaitForSingleObject(thread->thread_handle, INFINITE);
}

void cgui_thread_joinThreadTimeout(CGUI_Thread* thread, DWORD milliseconds) {
    WaitForSingleObject(thread->thread_handle, milliseconds);
}

void cgui_thread_forceKill(CGUI_Thread* thread) {
    TerminateThread(thread->thread_handle, 0);
}

void cgui_destroyThread(CGUI_Thread* thread) {
    CloseHandle(thread->thread_handle);
    free(thread);
}
