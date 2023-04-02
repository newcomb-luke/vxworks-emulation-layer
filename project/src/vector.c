#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define INITIAL_CAPACITY 4
#define RESIZE_FACTOR    2

void _vector_resize(vector* this);

void vector_init(vector * this, size_t sizeOfItem) {
    this->capacity = INITIAL_CAPACITY;
    this->size = 0;
    this->sizeOfItem = sizeOfItem;
    this->start = malloc(sizeOfItem * INITIAL_CAPACITY);
}

void vector_delete(vector* this) {
    this->capacity = 0;
    this->size = 0;
    this->sizeOfItem = 0;

    free(this->start);
    
    this->start = NULL;
}

void vector_push(vector* this, void* itemPtr) {
    if (this->size >= this->capacity) {
        _vector_resize(this);
    }

    uint8_t* nextSpacePtr = ((uint8_t*) this->start) + this->sizeOfItem * this->size;

    memcpy(nextSpacePtr, (uint8_t*) itemPtr, this->sizeOfItem);

    this->size += 1;
}

void* vector_get(vector* this, size_t index) {
    if (index >= this->size) {
        return NULL;
    }

    return (void*)(((uint8_t*) this->start) + this->sizeOfItem * index);
}

size_t vector_size(vector* this) {
    return this->size;
}

void _vector_resize(vector* this) {
    this->capacity *= RESIZE_FACTOR;
    this->start = realloc(this->start, this->capacity);
}
