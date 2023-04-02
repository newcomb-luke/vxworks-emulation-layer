#pragma once

#include <stddef.h>

typedef struct {
    size_t capacity;
    size_t size;
    size_t sizeOfItem;
    void* start;
} vector;

/*
 * Initializes a new vector with items that are sizeOfItem bytes long
 */
void vector_init(vector* v, size_t sizeOfItem);

/*
 * Deletes a new vector and deallocates all items contained inside. This
 * vector should not be re-used without first calling vector_init() on it first.
 */
void vector_delete(vector* v);

/*
 * Pushes a new item to the back of the vector, reallocates if necessary
 */
void vector_push(vector* v, void* itemPtr);

/*
 * Initializes a new vector with items that are sizeOfItem bytes long
 */
void* vector_get(vector* v, size_t index);

/*
 * Initializes a new vector with items that are sizeOfItem bytes long
 */
size_t vector_size(vector* v);
