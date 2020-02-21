/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIVECTOR_H_
#define MERLICT_MLIVECTOR_H_

#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "mli_debug.h"
#include "mli_math.h"

#define MLI_VECTOR_MINIMUM_CAPACITY 2
#define MLI_VECTOR_GROWTH_FACTOR 2

struct mliVector{
        uint64_t size;
        uint64_t capacity;
        uint64_t sizeof_element;
        void* data;
};

struct mliVector mliVector_init() {
        struct mliVector v;
        v.size = 0;
        v.capacity = 0;
        v.sizeof_element = 0;
        v.data = NULL;
        return v;
}

void mliVector_free(struct mliVector *vector)
{
        free(vector->data);
        vector->data = NULL;
        vector->size = 0;
        vector->capacity = 0;
        vector->sizeof_element = 0;
}

uint64_t mliVector_byte_size(const struct mliVector *vector)
{
        return vector->size * vector->sizeof_element;
}

int mliVector_malloc(
        struct mliVector *vector,
        uint64_t capacity,
        uint64_t sizeof_element)
{
        vector->size = 0;
        vector->capacity = MLI_MAX2(MLI_VECTOR_MINIMUM_CAPACITY, capacity);
        vector->sizeof_element = sizeof_element;
        mli_malloc(vector->data, char, vector->capacity*sizeof_element);
        return 1;
error:
        return 0;
}

int mliVector_should_grow(struct mliVector *vector)
{
        assert(vector->size <= vector->capacity);
        return vector->size == vector->capacity ? 1 : 0;
}

int mliVector_reallocate(struct mliVector *vector, uint64_t new_capacity)
{
        uint64_t new_capacity_in_bytes;
        void* old;
        if (new_capacity < MLI_VECTOR_MINIMUM_CAPACITY) {
                if (vector->capacity > MLI_VECTOR_MINIMUM_CAPACITY) {
                        new_capacity = MLI_VECTOR_MINIMUM_CAPACITY;
                } else {
                        return 1;
                }
        }
        new_capacity_in_bytes = new_capacity*vector->sizeof_element;
        old = vector->data;
        mli_malloc(vector->data, char, new_capacity_in_bytes);
        memcpy(vector->data, old, mliVector_byte_size(vector));
        vector->capacity = new_capacity;
        free(old);
        return 1;
error:
        return 0;
}

int mliVector_adjust_capacity(struct mliVector *vector)
{
        return mliVector_reallocate(
                vector,
                MLI_MAX2(1, vector->size*MLI_VECTOR_GROWTH_FACTOR));
}

void *mliVector_offset(const struct mliVector *vector, uint64_t index)
{
        return (uint8_t*)vector->data + (index * vector->sizeof_element);
}

void mliVector_assign(
        struct mliVector *vector,
        uint64_t index,
        const void* element)
{
        /* Insert the element */
        void* offset = mliVector_offset(vector, index);
        memcpy(offset, element, vector->sizeof_element);
}

int mliVector_push_back(struct mliVector *vector, const void *element)
{
        if (mliVector_should_grow(vector)) {
                mli_check(
                        mliVector_adjust_capacity(vector),
                        "Failed to grow vector capacity.");
        }
        mliVector_assign(vector, vector->size, element);
        vector->size += 1;
        return 1;
error:
        return 0;
}

void *mliVector_at(const struct mliVector *vector, uint64_t index)
{
        assert(index < vector->size);
        return mliVector_offset(vector, index);
}

#endif
