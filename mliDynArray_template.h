/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIDYNARRAY_TEMPLATE_H_
#define MERLICT_MLIDYNARRAY_TEMPLATE_H_

#include "mliDynArray.h"
#include "mliColor.h"
#include "mliVec.h"

#define MLIDYNARRAY_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                         \
                                                                               \
        struct LIB##Dyn##NAME {                                                \
                struct mliDynArray dyn;                                        \
                PAYLOAD_TYPE *arr;                                             \
        };                                                                     \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void);                     \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh);                 \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *dh, const uint64_t cap);                \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *dh, const PAYLOAD_TYPE pl);             \
                                                                               \
        int _##LIB##Dyn##NAME##_test_after_init(                               \
                const struct LIB##Dyn##NAME *dh);                              \
                                                                               \
        int _##LIB##Dyn##NAME##_test_after_free(                               \
                const struct LIB##Dyn##NAME *dh);                              \
                                                                               \
        int _##LIB##Dyn##NAME##_test_after_malloc(                             \
                const struct LIB##Dyn##NAME *dh, uint64_t cap);


#define MLIDYNARRAY_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                    \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void)                      \
        {                                                                      \
                struct LIB##Dyn##NAME dh;                                      \
                mliDynArray_init_in_host(                                      \
                        &dh.dyn, (void **)&dh.arr, sizeof(*dh.arr));           \
                return dh;                                                     \
        }                                                                      \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh)                  \
        {                                                                      \
                mliDynArray_free_in_host(&dh->dyn, (void **)&dh->arr);         \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *dh, const uint64_t cap)                 \
        {                                                                      \
                return mliDynArray_malloc_in_host(                             \
                        &dh->dyn, (void **)&dh->arr, cap);                     \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *dh, const PAYLOAD_TYPE pl)              \
        {                                                                      \
                return mliDynArray_push_back_in_host(                          \
                        &dh->dyn, (void **)&dh->arr, &pl);                     \
        }                                                                      \
                                                                               \
        int _##LIB##Dyn##NAME##_test_after_init(                               \
                const struct LIB##Dyn##NAME *dh)                               \
        {                                                                      \
                return _mliDynArray_test_after_init(                           \
                        &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr));  \
        }                                                                      \
                                                                               \
        int _##LIB##Dyn##NAME##_test_after_free(                               \
                const struct LIB##Dyn##NAME *dh)                               \
        {                                                                      \
                return _mliDynArray_test_after_free(                           \
                        &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr));  \
        }                                                                      \
                                                                               \
        int _##LIB##Dyn##NAME##_test_after_malloc(                             \
                const struct LIB##Dyn##NAME *dh, uint64_t cap)                 \
        {                                                                      \
                return _mliDynArray_test_after_malloc(                         \
                        &dh->dyn,                                              \
                        (const void **)&dh->arr,                               \
                        sizeof(*dh->arr),                                      \
                        cap);                                                  \
        }

MLIDYNARRAY_DEFINITON(mli, Double, double)
MLIDYNARRAY_IMPLEMENTATION(mli, Double, double)

MLIDYNARRAY_DEFINITON(mli, Vec, struct mliVec)
MLIDYNARRAY_IMPLEMENTATION(mli, Vec, struct mliVec)

MLIDYNARRAY_DEFINITON(mli, Color, struct mliColor)
MLIDYNARRAY_IMPLEMENTATION(mli, Color, struct mliColor)

MLIDYNARRAY_DEFINITON(mli, ColorPtr, struct mliColor *)
MLIDYNARRAY_IMPLEMENTATION(mli, ColorPtr, struct mliColor *)

MLIDYNARRAY_DEFINITON(mli, Int64, int64_t)
MLIDYNARRAY_IMPLEMENTATION(mli, Int64, int64_t)

MLIDYNARRAY_DEFINITON(mli, Char, char)
MLIDYNARRAY_IMPLEMENTATION(mli, Char, char)

MLIDYNARRAY_DEFINITON(mli, Uint8, uint8_t)
MLIDYNARRAY_IMPLEMENTATION(mli, Uint8, uint8_t)

MLIDYNARRAY_DEFINITON(mli, Uint32, uint32_t)
MLIDYNARRAY_IMPLEMENTATION(mli, Uint32, uint32_t)

#endif
