//
// Created on: 2024/12/2, 14:02.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_LOCK_H
#define CGUI_LOCK_H

#include "../util/common.h"

typedef struct FakeSpinLock CGUI_SpinLock;

typedef struct FakeSpinLock {
    bool locked;

    void (* lock)(CGUI_SpinLock* lock);

    void (* unlock)(CGUI_SpinLock* lock);

    bool (* tryLock)(CGUI_SpinLock* lock);
} CGUI_SpinLock;

CGUI_SpinLock* cgui_createSpinLock();

void cgui_destroySpinLock(CGUI_SpinLock* lock);

void cgui_spinLock_lock(CGUI_SpinLock* lock);

void cgui_spinLock_unlock(CGUI_SpinLock* lock);

bool cgui_spinLock_tryLock(CGUI_SpinLock* lock);

typedef struct FakeMutex CGUI_Mutex;


#endif //CGUI_LOCK_H
