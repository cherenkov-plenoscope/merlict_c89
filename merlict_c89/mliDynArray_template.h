/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDYNARRAY_TEMPLATE_H_
#define MERLICT_C89_MLIDYNARRAY_TEMPLATE_H_

#include <stdint.h>
#include "mliColor.h"

#define MLIDYNARRAY_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                         \
                                                                               \
        struct LIB##Dyn##NAME {                                                \
                uint64_t capacity;                                             \
                uint64_t size;                                                 \
                PAYLOAD_TYPE *array;                                           \
        };                                                                     \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void);                     \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh);                 \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *dh, const uint64_t size);               \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *dh, const uint64_t size);               \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *dh, PAYLOAD_TYPE item);

#define MLIDYNARRAY_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                    \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void)                      \
        {                                                                      \
                struct LIB##Dyn##NAME dh;                                      \
                dh.capacity = 0u;                                              \
                dh.size = 0u;                                                  \
                dh.array = NULL;                                               \
                return dh;                                                     \
        }                                                                      \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh)                  \
        {                                                                      \
                free(dh->array);                                               \
                (*dh) = LIB##Dyn##NAME##_init();                               \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *dh, const uint64_t size)                \
        {                                                                      \
                LIB##Dyn##NAME##_free(dh);                                     \
                dh->capacity = MLI_MAX2(2, size);                              \
                dh->size = 0;                                                  \
                mli_check_malloc(dh->array, PAYLOAD_TYPE, dh->capacity);             \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *dh, const uint64_t size)                \
        {                                                                      \
                mli_check(LIB##Dyn##NAME##_malloc(dh, size));                      \
                dh->size = size;                                               \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *dh, PAYLOAD_TYPE item)                  \
        {                                                                      \
                if (dh->size == dh->capacity) {                                \
                        dh->capacity = dh->capacity * 2;                       \
                        dh->array = (PAYLOAD_TYPE *)realloc(                   \
                                (void *)dh->array,                             \
                                dh->capacity * sizeof(PAYLOAD_TYPE));          \
                        mli_check_mem(dh->array);                              \
                }                                                              \
                                                                               \
                dh->array[dh->size] = item;                                    \
                dh->size += 1;                                                 \
                                                                               \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }

#define MLIDYNARRAY_TEST_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                    \
                                                                               \
        int LIB##Dyn##NAME##_test_init(struct LIB##Dyn##NAME *dh);             \
                                                                               \
        int LIB##Dyn##NAME##_test_malloc(                                      \
                struct LIB##Dyn##NAME *dh, const uint64_t capacity);           \
                                                                               \
        int LIB##Dyn##NAME##_test_free(struct LIB##Dyn##NAME *dh);

#define MLIDYNARRAY_TEST_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)               \
                                                                               \
        int LIB##Dyn##NAME##_test_init(struct LIB##Dyn##NAME *dh)              \
        {                                                                      \
                mli_check(dh->capacity == 0u);                                     \
                mli_check(dh->size == 0u);                                         \
                mli_check(dh->array == NULL);                                      \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_test_malloc(                                      \
                struct LIB##Dyn##NAME *dh, const uint64_t capacity)            \
        {                                                                      \
                mli_check(dh->capacity >= dh->size);                               \
                if (capacity < 2) {                                            \
                        mli_check(dh->capacity == 2);                              \
                } else {                                                       \
                        mli_check(dh->capacity == capacity);                       \
                }                                                              \
                mli_check(dh->array != NULL);                                      \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_test_free(struct LIB##Dyn##NAME *dh)              \
        {                                                                      \
                return LIB##Dyn##NAME##_test_init(dh);                         \
        }

MLIDYNARRAY_DEFINITON(mli, Uint32, uint32_t)
MLIDYNARRAY_DEFINITON(mli, Float, float)
MLIDYNARRAY_DEFINITON(mli, Double, double)
MLIDYNARRAY_DEFINITON(mli, Color, struct mliColor)
MLIDYNARRAY_TEST_DEFINITON(mli, Color, struct mliColor)
MLIDYNARRAY_DEFINITON(mli, ColorPtr, struct mliColor *)
MLIDYNARRAY_DEFINITON(mli, Vec, struct mliVec)
MLIDYNARRAY_DEFINITON(mli, Face, struct mliFace)

#endif
