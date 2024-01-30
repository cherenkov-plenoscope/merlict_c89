/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIDYNARRAY_H_
#define MLIDYNARRAY_H_

#include <stdint.h>
#include <stdlib.h>
#include "chk.h"
#include "mli_math.h"


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
                chk_malloc(dh->array, PAYLOAD_TYPE, dh->capacity);             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *dh, const uint64_t size)                \
        {                                                                      \
                chk(LIB##Dyn##NAME##_malloc(dh, size));                        \
                dh->size = size;                                               \
                return 1;                                                      \
        chk_error:                                                             \
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
                        chk_mem(dh->array);                                    \
                }                                                              \
                                                                               \
                dh->array[dh->size] = item;                                    \
                dh->size += 1;                                                 \
                                                                               \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#endif
