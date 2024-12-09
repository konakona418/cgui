//
// Created on: 2024/11/12, 16:14.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_THREAD_POOL_H
#define CGUI_THREAD_POOL_H

#include <windows.h>
#include "../util/vector.h"
#include "thread.h"

typedef struct ThreadPool CGUI_ThreadPool;

typedef struct ThreadPool {
    int maxNumThreads;
    int currentNumThreads;
    CGUI_Thread** threads;

    void (* addTask)(CGUI_ThreadPool* pool, void* (* func)(void*), void* arg);

    void (* wait)(CGUI_ThreadPool* pool);

    void (* join)(CGUI_ThreadPool* pool);

    void (* joinTimeout)(CGUI_ThreadPool* pool, DWORD timeout);
} CGUI_ThreadPool;

CGUI_ThreadPool* CGUI_createThreadPool(int maxNumThreads);

void CGUI_destroyThreadPool(CGUI_ThreadPool* pool);

void CGUI_threadPool_addTask(CGUI_ThreadPool* pool, void* (* func)(void*), void* arg);

void CGUI_threadPool_wait(CGUI_ThreadPool* pool);

void CGUI_threadPool_join(CGUI_ThreadPool* pool);

void CGUI_threadPool_joinTimeout(CGUI_ThreadPool* pool, DWORD timeout);

#endif //CGUI_THREAD_POOL_H
