//
// Created on: 2024/11/7, 11:27.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_ITERATOR_H
#define CGUI_ITERATOR_H

typedef struct Iterator Iterator;

typedef struct Iterator {
    void* (* next)(Iterator* self);
    void (* destroy)(Iterator* self);
    int (* has_next)(Iterator* self);
    void (* reset)(Iterator* self);
} Iterator;

void iterator_destroy(Iterator* self);

int iterator_has_next(Iterator* self);

void* iterator_next(Iterator* self);

void iterator_reset(Iterator* self);

#endif //CGUI_ITERATOR_H
