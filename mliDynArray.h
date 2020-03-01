/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIDYNARRAY_H_
#define MERLICT_MLIDYNARRAY_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include "mli_debug.h"
#include "mli_math.h"

#define MLI_DYNARRAY_MIN_CAPACITY 2
#define MLI_DYNARRAY_GROWTH_FACTOR 2

struct mliDynArray {
        uint64_t size;
        uint64_t capacity;
        uint64_t sizeof_element;
};

/* internal implementation */
/* ----------------------- */

uint64_t _mliDynArray_byte_size(const struct mliDynArray *dyn)
{
        return dyn->size*dyn->sizeof_element;
}

int _mliDynArray_should_grow(const struct mliDynArray *dyn)
{
        assert(dyn->size <= dyn->capacity);
        return dyn->size == dyn->capacity ? 1 : 0;
}

int _mliDynArray_reallocate(
        struct mliDynArray *dyn,
        void **arr,
        uint64_t new_capacity)
{
        uint64_t new_capacity_in_bytes = new_capacity*dyn->sizeof_element;
        fprintf(stderr, "next size %ld\n", new_capacity_in_bytes);
        (*arr) = realloc((*arr), new_capacity_in_bytes);
        mli_check_mem((*arr));
        dyn->capacity = new_capacity;
        return 1;
error:
        return 0;
}

void *_mliDynArray_offset(
        struct mliDynArray *dyn,
        void **arr,
        const uint64_t index)
{
        return (uint8_t *)(*arr) + (index*dyn->sizeof_element);
}

void _mliDynArray_assign(
        struct mliDynArray *dyn,
        void **arr,
        const uint64_t index,
        const void *element)
{
        void* offset = _mliDynArray_offset(dyn, arr, index);
        memcpy(offset, element, dyn->sizeof_element);
}

/* Interface to be used in host */
/* ---------------------------- */

void mliDynArray_init_in_host(
        struct mliDynArray *dyn,
        void **arr,
        const uint64_t sizeof_element)
{
        dyn->size = 0u;
        dyn->capacity = 0u;
        dyn->sizeof_element = sizeof_element;
        (*arr) = NULL ;
}

int mliDynArray_malloc_in_host(
        struct mliDynArray *dyn,
        void **arr,
        uint64_t capacity)
{
        uint64_t new_capacity;
        dyn->size = 0;
        new_capacity = MLI_MAX2(MLI_DYNARRAY_MIN_CAPACITY, capacity);
        (*arr) = (void **)malloc(new_capacity*dyn->sizeof_element);
        mli_check_mem((*arr));
        dyn->capacity = new_capacity;
        return 1;
error:
        return 0;
}

int mliDynArray_push_back_in_host(
        struct mliDynArray *dyn,
        void **arr,
        const void *element)
{
        if (_mliDynArray_should_grow(dyn)) {
                mli_check(_mliDynArray_reallocate(
                        dyn,
                        arr,
                        dyn->size*MLI_DYNARRAY_GROWTH_FACTOR),
                        "Failed to grow dynamic array");
        }
        _mliDynArray_assign(dyn, arr, dyn->size, element);
        dyn->size += 1;
        return 1;
error:
        return 0;
}

void mliDynArray_free_in_host(struct mliDynArray *dyn, void **arr)
{
        free((*arr));
        (*arr) = NULL;
        dyn->size = 0u;
        dyn->capacity = 0u;
}

/* testing */
/* ------- */

int _mliDynArray_test_after_init(
        const struct mliDynArray *dyn,
        const void **arr,
        const uint64_t sizeof_element)
{
        mli_c(dyn->size == 0u);
        mli_c(dyn->capacity == 0u);
        mli_c(dyn->sizeof_element == sizeof_element);
        mli_c((*arr) == NULL);
        return 1;
error:
        return 0;
}

int _mliDynArray_test_after_malloc(
        const struct mliDynArray *dyn,
        const void **arr,
        const uint64_t sizeof_element,
        const uint64_t capacity)
{
        mli_c(dyn->size == 0u);
        if (capacity < MLI_DYNARRAY_MIN_CAPACITY) {
                mli_c(dyn->capacity == MLI_DYNARRAY_MIN_CAPACITY);
        } else {
                mli_c(dyn->capacity == capacity);
        }
        mli_c(dyn->sizeof_element == sizeof_element);
        mli_c((*arr) != NULL);
        return 1;
error:
        return 0;
}

int _mliDynArray_test_after_free(
        const struct mliDynArray *dyn,
        const void **arr,
        const uint64_t sizeof_element)
{
        return _mliDynArray_test_after_init(dyn, arr, sizeof_element);
}




#endif
