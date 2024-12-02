//
// Created on: 2024/12/2, 14:02.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include "lock.h"

CGUI_SpinLock* cgui_createSpinLock()
{
    CGUI_SpinLock* lock = (CGUI_SpinLock*) malloc(sizeof(CGUI_SpinLock));
    lock->locked = false;

    lock->lock = (void (*)(CGUI_SpinLock*)) cgui_spinLock_lock;
    lock->tryLock = (bool (*)(CGUI_SpinLock*)) cgui_spinLock_tryLock;
    lock->unlock = (void (*)(CGUI_SpinLock*)) cgui_spinLock_unlock;
    return lock;
}

void cgui_destroySpinLock(CGUI_SpinLock* lock)
{
    free(lock);
}

void cgui_spinLock_lock(CGUI_SpinLock* lock) {
    while (lock->locked) {
        Sleep(0);
    };
    lock->locked = true;
}

bool cgui_spinLock_tryLock(CGUI_SpinLock* lock) {
    return !lock->locked;
}

void cgui_spinLock_unlock(CGUI_SpinLock* lock) {
    lock->locked = false;
}
