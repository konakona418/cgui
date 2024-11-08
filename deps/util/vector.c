//
// Created on: 2024/11/8, 10:31.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#include <stdlib.h>
#include <string.h>
#include "vector.h"

Vector* create_vector(size_t element_size, size_t capacity) {
    Vector* vector = malloc(sizeof(Vector));

    vector->capacity = capacity;
    vector->data = malloc(capacity * element_size);
    vector->element_size = element_size;
    vector->size = 0;

    vector->clear = vector_clear;
    vector->get = vector_get;
    vector->set = vector_set;
    vector->remove = vector_remove;
    vector->resize = vector_resize;
    vector->push_back = vector_push_back;
    vector->pop_back = vector_pop_back;
    vector->iter = vector_iter;
    vector->iter_result = vector_iter_result;

    return vector;
}

void destroy_vector(Vector* vector) {
    if (vector == NULL) return;
    free(vector);
}

void destroy_vector_with_destructor(Vector* vector, void (* destructor)(void* element)) {
    if (vector == NULL) return;
    size_t i = 0;
    while (vector->size > 0) {
        destructor(vector->data + i * vector->element_size);
        i++;
    }
    free(vector);
}

void vector_clear(Vector* vector) {
    memset(vector->data, 0, vector->capacity * vector->element_size);
    vector->size = 0;
}

void* vector_get(Vector* vector, size_t index) {
    if (index < vector->size) {
        return vector->data + index * vector->element_size;
    }
    return NULL;
}

void vector_set(Vector* vector, size_t index, const void* element) {
    if (index < vector->size) {
        memcpy(vector->data + index * vector->element_size, element, vector->element_size);
    }
}

void* vector_remove(Vector* vector, size_t index) {
    if (index < vector->size) {
        void* element = vector->data + index * vector->element_size;
        memmove(vector->data + index * vector->element_size, vector->data + (index + 1) * vector->element_size,
                (vector->size - index - 1) * vector->element_size);
        vector->size--;
        return element;
    }
    return NULL;
}

void vector_resize(Vector* vector, size_t new_size) {
    if (new_size > vector->capacity) {
        vector->data = realloc(vector->data, vector->element_size * new_size);
        vector->capacity = new_size;
    }
}

void vector_push_back(Vector* vector, const void* element) {
    vector_resize(vector, vector->size + 1);
    memcpy(vector->data + vector->size * vector->element_size, element, vector->element_size);
    vector->size++;
}

void* vector_pop_back(Vector* vector) {
    if (vector->size > 0) {
        vector->size--;
        return vector->data + vector->size * vector->element_size;
    }
    return NULL;
}

void vector_iter(Vector* vector, void (* callback)(void* element)) {
    size_t i = 0;
    while (vector->size > 0) {
        callback(vector->data + i * vector->element_size);
        i++;
    }
}

CGUI_Result vector_iter_result(Vector* vector, CGUI_Result (* callback)(void* element)) {
    size_t i = 0;
    while (vector->size > 0) {
        CGUI_Result result = callback(vector->data + i * vector->element_size);
        if (is_err(&result)) return result;
        i++;
    }
    return create_ok(NULL);
}
