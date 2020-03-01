/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIDYNARRAY_TEST_H_
#define MERLICT_MLIDYNARRAY_TEST_H_

#include "mliDynArray.h"
#include "mliColor.h"

struct mliDynColor {
        struct mliDynArray dyn;
        struct mliColor *arr;
};

struct mliDynColor mliDynColor_init() {
        struct mliDynColor dh;
        mliDynArray_init_in_host(&dh.dyn, (void **)&dh.arr, sizeof(*dh.arr));
        return dh;
}

void mliDynColor_free(struct mliDynColor *dh) {
        mliDynArray_free_in_host(&dh->dyn, (void **)&dh->arr);
}

int mliDynColor_malloc(struct mliDynColor *dh, const uint64_t capacity) {
        return mliDynArray_malloc_in_host(
                &dh->dyn, (void **)&dh->arr, capacity);
}

int mliDynColor_push_back(struct mliDynColor *dh, struct mliColor hans) {
        return mliDynArray_push_back_in_host(
                &dh->dyn, (void **)&dh->arr, &hans);
}

int _mliDynColor_test_after_init(const struct mliDynColor *dh) {
        return _mliDynArray_test_after_init(
                &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr));
}

int _mliDynColor_test_after_malloc(const struct mliDynColor *dh, uint64_t cap) {
        return _mliDynArray_test_after_malloc(
                &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr), cap);
}

int _mliDynColor_test_after_free(const struct mliDynColor *dh) {
        return _mliDynArray_test_after_free(
                &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr));
}


#endif
