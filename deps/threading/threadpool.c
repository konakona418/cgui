//
// Created on: 2024/11/12, 16:14.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "threadpool.h"

CGUI_ThreadPool* CGUI_createThreadPool(int maxNumThreads) {
    CGUI_ThreadPool* pool = (CGUI_ThreadPool*) malloc(sizeof(CGUI_ThreadPool));

    pool->maxNumThreads = maxNumThreads;
    pool->currentNumThreads = 0;

    pool->threads = (CGUI_Thread**) malloc(sizeof(CGUI_Thread*) * maxNumThreads);

    pool->addTask = CGUI_threadPool_addTask;
    pool->join = CGUI_threadPool_join;
    pool->joinTimeout = CGUI_threadPool_joinTimeout;
    pool->wait = CGUI_threadPool_wait;

    return pool;
}

void CGUI_destroyThreadPool(CGUI_ThreadPool* pool) {
    for (int i = 0; i < pool->maxNumThreads; ++i) {
        cgui_destroyThread(pool->threads[i]);
    }
    free(pool->threads);
    free(pool);
}

void CGUI_threadPool_addTask(CGUI_ThreadPool* pool, void* (*func)(void*), void* arg) {
    if (pool->currentNumThreads >= pool->maxNumThreads) {
        return;
    }
    CGUI_Thread* thread = cgui_createThread(func, arg);
    pool->threads[pool->currentNumThreads++] = thread;
}

void CGUI_threadPool_join(CGUI_ThreadPool* pool) {
    HANDLE handles[pool->currentNumThreads];
    for (int i = 0; i < pool->currentNumThreads; ++i) {
        handles[i] = pool->threads[i]->thread_handle;
    }
    WaitForMultipleObjects(pool->currentNumThreads, handles, TRUE, INFINITE);
}

void CGUI_threadPool_joinTimeout(CGUI_ThreadPool* pool, DWORD timeout) {
    HANDLE handles[pool->currentNumThreads];
    for (int i = 0; i < pool->currentNumThreads; ++i) {
        handles[i] = pool->threads[i]->thread_handle;
    }
    WaitForMultipleObjects(pool->currentNumThreads, handles, TRUE, timeout);
}
