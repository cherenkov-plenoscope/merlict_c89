/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_ARRAY_H_
#define MTL_ARRAY_H_

#include <stdint.h>
#include <stdlib.h>
#include "../../chk/src/chk.h"

#define MTL_ARRAY_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                           \
                                                                               \
        struct LIB##Array##NAME {                                              \
                uint64_t size;                                                 \
                PAYLOAD_TYPE *array;                                           \
        };                                                                     \
                                                                               \
        struct LIB##Array##NAME LIB##Array##NAME##_init(void);                 \
                                                                               \
        void LIB##Array##NAME##_free(struct LIB##Array##NAME *dh);             \
                                                                               \
        int LIB##Array##NAME##_malloc(                                         \
                struct LIB##Array##NAME *dh, const uint64_t size);             \
                                                                               \
        int LIB##Array##NAME##_realloc(                                        \
                struct LIB##Array##NAME *dh, const uint64_t size);             \
                                                                               \
        int LIB##Array##NAME##_set(                                            \
                struct LIB##Array##NAME *dh,                                   \
                const uint64_t at,                                             \
                PAYLOAD_TYPE item);                                            \
                                                                               \
        int LIB##Array##NAME##_get(                                            \
                struct LIB##Array##NAME *dh,                                   \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item);

#define MTL_ARRAY_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                      \
                                                                               \
        struct LIB##Array##NAME LIB##Array##NAME##_init(void)                  \
        {                                                                      \
                struct LIB##Array##NAME dh;                                    \
                dh.size = 0u;                                                  \
                dh.array = NULL;                                               \
                return dh;                                                     \
        }                                                                      \
                                                                               \
        void LIB##Array##NAME##_free(struct LIB##Array##NAME *dh)              \
        {                                                                      \
                free(dh->array);                                               \
                (*dh) = LIB##Array##NAME##_init();                             \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_malloc(                                         \
                struct LIB##Array##NAME *dh, const uint64_t size)              \
        {                                                                      \
                LIB##Array##NAME##_free(dh);                                   \
                dh->size = size;                                               \
                chk_malloc(dh->array, PAYLOAD_TYPE, dh->size);                 \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_realloc(                                        \
                struct LIB##Array##NAME *dh, const uint64_t size)              \
        {                                                                      \
                PAYLOAD_TYPE *new_array = (PAYLOAD_TYPE *)realloc(             \
                        (void *)dh->array, size * sizeof(PAYLOAD_TYPE));       \
                chk_mem(new_array);                                            \
                dh->array = new_array;                                         \
                dh->size = size;                                               \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_set(                                            \
                struct LIB##Array##NAME *dh,                                   \
                const uint64_t at,                                             \
                PAYLOAD_TYPE item)                                             \
        {                                                                      \
                chk_msg(at < dh->size, "Out of range.");                       \
                dh->array[at] = item;                                          \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_get(                                            \
                struct LIB##Array##NAME *dh,                                   \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item)                                            \
        {                                                                      \
                chk_msg(at < dh->size, "Out of range.");                       \
                (*item) = dh->array[at];                                       \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#endif
