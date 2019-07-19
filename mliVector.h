/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIVECTOR_H_
#define MERLICT_MLIVECTOR_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "mli_debug.h"
#include "mliMath.h"

#define MLI_VECTOR_ERROR 0
#define MLI_VECTOR_SUCCESS 1
#define MLI_VECTOR_MINIMUM_CAPACITY 2
#define MLI_VECTOR_GROWTH_FACTOR 2

typedef struct {
    uint64_t size;
    uint64_t capacity;
    uint64_t sizeof_element;
    void* data;
} mliVector;

uint64_t mliVector_byte_size(const mliVector* vector) {
    return vector->size * vector->sizeof_element;
}

int mliVector_free(mliVector* vector) {
    assert(vector != NULL);
    if (vector == NULL) return MLI_VECTOR_ERROR;
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    return MLI_VECTOR_SUCCESS;
}

int mliVector_set(
    mliVector* vector,
    uint64_t capacity,
    uint64_t sizeof_element) {
    assert(vector != NULL);
    if (vector == NULL) return MLI_VECTOR_ERROR;
    vector->size = 0;
    vector->capacity = MLI_MAX2(MLI_VECTOR_MINIMUM_CAPACITY, capacity);
    vector->sizeof_element = sizeof_element;
    vector->data = malloc(vector->capacity*sizeof_element);
    return vector->data == NULL ? MLI_VECTOR_ERROR : MLI_VECTOR_SUCCESS;
}

int mliVector_should_grow(mliVector* vector) {
    assert(vector->size <= vector->capacity);
    if (vector->size == vector->capacity)
        return 1;
    else
        return 0;
}

int mliVector_reallocate(mliVector* vector, uint64_t new_capacity) {
    uint64_t new_capacity_in_bytes;
    void* old;
    assert(vector != NULL);
    if (new_capacity < MLI_VECTOR_MINIMUM_CAPACITY) {
        if (vector->capacity > MLI_VECTOR_MINIMUM_CAPACITY) {
            new_capacity = MLI_VECTOR_MINIMUM_CAPACITY;
        } else {
            /* NO-OP */
            return MLI_VECTOR_SUCCESS;
        }
    }
    new_capacity_in_bytes = new_capacity*vector->sizeof_element;
    old = vector->data;
    if ((vector->data = malloc(new_capacity_in_bytes)) == NULL) {
        return MLI_VECTOR_ERROR;
    }
    memcpy(vector->data, old, mliVector_byte_size(vector));
    vector->capacity = new_capacity;
    free(old);
    return MLI_VECTOR_SUCCESS;
}

int mliVector_adjust_capacity(mliVector* vector) {
    return mliVector_reallocate(
        vector,
        MLI_MAX2(1, vector->size*MLI_VECTOR_GROWTH_FACTOR));
}

void* mliVector_offset(const mliVector* vector, uint64_t index) {
    return (uint8_t*)vector->data + (index * vector->sizeof_element);
}

void mliVector_assign(mliVector* vector, uint64_t index, void* element) {
    /* Insert the element */
    void* offset = mliVector_offset(vector, index);
    memcpy(offset, element, vector->sizeof_element);
}

int mliVector_push_back(mliVector* vector, void* element) {
    assert(vector != NULL);
    assert(element != NULL);
    if (mliVector_should_grow(vector)) {
        if (mliVector_adjust_capacity(vector) == MLI_VECTOR_ERROR) {
            return MLI_VECTOR_ERROR;
        }
    }
    mliVector_assign(vector, vector->size, element);
    ++vector->size;
    return MLI_VECTOR_SUCCESS;
}

void* mliVector_get(const mliVector* vector, uint64_t index) {
    assert(vector != NULL);
    assert(index < vector->size);
    if (vector == NULL) return NULL;
    if (vector->sizeof_element == 0) return NULL;
    if (index >= vector->size) return NULL;
    return mliVector_offset(vector, index);
}

#endif
