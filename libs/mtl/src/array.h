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
        void LIB##Array##NAME##_free(struct LIB##Array##NAME *self);           \
                                                                               \
        int LIB##Array##NAME##_malloc(                                         \
                struct LIB##Array##NAME *self, const uint64_t size);           \
                                                                               \
        int LIB##Array##NAME##_realloc(                                        \
                struct LIB##Array##NAME *self, const uint64_t size);           \
                                                                               \
        int LIB##Array##NAME##_set(                                            \
                struct LIB##Array##NAME *self,                                 \
                const uint64_t at,                                             \
                PAYLOAD_TYPE item);                                            \
                                                                               \
        int LIB##Array##NAME##_get(                                            \
                struct LIB##Array##NAME *self,                                 \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item);

#define MTL_ARRAY_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                      \
                                                                               \
        struct LIB##Array##NAME LIB##Array##NAME##_init(void)                  \
        {                                                                      \
                struct LIB##Array##NAME out;                                   \
                out.size = 0u;                                                 \
                out.array = NULL;                                              \
                return out;                                                    \
        }                                                                      \
                                                                               \
        void LIB##Array##NAME##_free(struct LIB##Array##NAME *self)            \
        {                                                                      \
                free(self->array);                                             \
                (*self) = LIB##Array##NAME##_init();                           \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_malloc(                                         \
                struct LIB##Array##NAME *self, const uint64_t size)            \
        {                                                                      \
                LIB##Array##NAME##_free(self);                                 \
                self->size = size;                                             \
                chk_malloc(self->array, PAYLOAD_TYPE, self->size);             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_realloc(                                        \
                struct LIB##Array##NAME *self, const uint64_t size)            \
        {                                                                      \
                PAYLOAD_TYPE *new_array = (PAYLOAD_TYPE *)realloc(             \
                        (void *)self->array, size * sizeof(PAYLOAD_TYPE));     \
                chk_mem(new_array);                                            \
                self->array = new_array;                                       \
                self->size = size;                                             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_set(                                            \
                struct LIB##Array##NAME *self,                                 \
                const uint64_t at,                                             \
                PAYLOAD_TYPE item)                                             \
        {                                                                      \
                chk_msg(at < self->size, "Out of range.");                     \
                self->array[at] = item;                                        \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Array##NAME##_get(                                            \
                struct LIB##Array##NAME *self,                                 \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item)                                            \
        {                                                                      \
                chk_msg(at < self->size, "Out of range.");                     \
                (*item) = self->array[at];                                     \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#endif
