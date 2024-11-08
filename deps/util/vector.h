//
// Created on: 2024/11/8, 10:31.
// Author: Zimeng LI (zimengli@mail.nwpu.edu.cn)
// Project: cgui
//

#ifndef CGUI_VECTOR_H
#define CGUI_VECTOR_H

#include <stdlib.h>

#include "../util/common.h"

/* Prototype */
typedef struct Vector Vector;

/* Definition of Vector.
 * Please pay attention that `size` is not guaranteed to be static.
 * So when iterating, use iter_result or iter,
 * instead of writing a loop manually. */
typedef struct Vector {
    void* data;
    size_t element_size;
    size_t size;
    size_t capacity;

    void (* push_back)(Vector* vector, const void* element);

    void* (* pop_back)(Vector* vector);

    void* (* remove)(Vector* vector, size_t index);

    void (* clear)(Vector* vector);

    void (* resize)(Vector* vector, size_t new_size);

    void (* set)(Vector* vector, size_t index, const void* element);

    void* (* get)(Vector* vector, size_t index);

    void (* iter)(Vector* vector, void (* callback)(void* element));

    CGUI_Result (* iter_result)(Vector* vector, CGUI_Result (* callback)(void* element));
} Vector;

/* Constructor and Destructor */
/**
 * Create a vector with given element size and capacity.
 * @param element_size: size of each element in bytes.
 * @param capacity: initial capacity of the vector.
 * @return: a pointer to the created vector. This should be freed by calling destroy_vector. */
Vector* create_vector(size_t element_size, size_t capacity);

/**
 * Destroy the vector.
 * Note that this function DOES NOT explicitly free the memory of the elements.
 * It is the caller's responsibility to free the memory of the elements.
 * For automated memory management, use destroy_vector_with_destructor.
 * @param vector: the vector to be destroyed. */
void destroy_vector(Vector* vector);

/**
 * Destroy the vector and free the memory of the elements.
 * This function is useful when the elements are dynamically allocated.
 * Note that the parameter passed into the destructor is not guaranteed to be not-null.
 * @param vector: the vector to be destroyed.
 * @param destructor: a function pointer to the destructor of the elements.
 * The destructor should free the memory of the element. */
void destroy_vector_with_destructor(Vector* vector, void (* destructor)(void* element));

void vector_push_back(Vector* vector, const void* element);

void* vector_pop_back(Vector* vector);

void* vector_remove(Vector* vector, size_t index);

/* Clear all elements in the vector
 * This is not safe, only use it when you are sure that the memory is deallocated. */
void vector_clear(Vector* vector);

void vector_resize(Vector* vector, size_t new_size);

void vector_set(Vector* vector, size_t index, const void* element);

void* vector_get(Vector* vector, size_t index);

void vector_iter(Vector* vector, void (* callback)(void* element));

CGUI_Result vector_iter_result(Vector* vector, CGUI_Result (* callback)(void* element));

#endif //CGUI_VECTOR_H
