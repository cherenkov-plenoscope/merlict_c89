/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDYNARRAY_H_
#define MERLICT_C89_MLIDYNARRAY_H_

#include <stdint.h>

#define MLI_DYNARRAY_MIN_CAPACITY 2
#define MLI_DYNARRAY_GROWTH_FACTOR 2

struct mliDynArray {
        uint64_t size;
        uint64_t capacity;
        uint64_t sizeof_element;
};

int _mliDynArray_test_after_free(
        const struct mliDynArray *dyn,
        const void **arr,
        const uint64_t sizeof_element);
int _mliDynArray_test_after_malloc(
        const struct mliDynArray *dyn,
        const void **arr,
        const uint64_t sizeof_element,
        const uint64_t capacity);
int _mliDynArray_test_after_init(
        const struct mliDynArray *dyn,
        const void **arr,
        const uint64_t sizeof_element);
void mliDynArray_free_in_host(struct mliDynArray *dyn, void **arr);
int mliDynArray_push_back_in_host(
        struct mliDynArray *dyn,
        void **arr,
        const void *element);
int mliDynArray_malloc_in_host(
        struct mliDynArray *dyn,
        void **arr,
        uint64_t capacity);
void mliDynArray_init_in_host(
        struct mliDynArray *dyn,
        void **arr,
        const uint64_t sizeof_element);
void _mliDynArray_assign(
        struct mliDynArray *dyn,
        void **arr,
        const uint64_t index,
        const void *element);
void *_mliDynArray_offset(
        struct mliDynArray *dyn,
        void **arr,
        const uint64_t index);
int _mliDynArray_reallocate(
        struct mliDynArray *dyn,
        void **arr,
        uint64_t new_capacity);
int _mliDynArray_should_grow(const struct mliDynArray *dyn);
#endif
