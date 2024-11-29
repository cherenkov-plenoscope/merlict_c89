/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_ARRAY_H_
#define MTL_ARRAY_H_

#include <stdint.h>
#include <stdlib.h>
#include "../../chk/src/chk.h"

#define MTL_ARRAY_DEFINITON(NAME, PAYLOAD_TYPE)                                \
                                                                               \
        struct NAME {                                                          \
                uint64_t size;                                                 \
                PAYLOAD_TYPE *array;                                           \
        };                                                                     \
                                                                               \
        struct NAME NAME##_init(void);                                         \
                                                                               \
        void NAME##_free(struct NAME *self);                                   \
                                                                               \
        int NAME##_malloc(                                                     \
                struct NAME *self, const uint64_t size);                       \
                                                                               \
        int NAME##_realloc(                                                    \
                struct NAME *self, const uint64_t size);                       \
                                                                               \
        int NAME##_set(                                                        \
                struct NAME *self,                                             \
                const uint64_t at,                                             \
                PAYLOAD_TYPE item);                                            \
                                                                               \
        int NAME##_get(                                                        \
                struct NAME *self,                                             \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item);

#define MTL_ARRAY_IMPLEMENTATION(NAME, PAYLOAD_TYPE)                           \
                                                                               \
        struct NAME NAME##_init(void)                                          \
        {                                                                      \
                struct NAME out;                                               \
                out.size = 0u;                                                 \
                out.array = NULL;                                              \
                return out;                                                    \
        }                                                                      \
                                                                               \
        void NAME##_free(struct NAME *self)                                    \
        {                                                                      \
                free(self->array);                                             \
                (*self) = NAME##_init();                                       \
        }                                                                      \
                                                                               \
        int NAME##_malloc(                                                     \
                struct NAME *self, const uint64_t size)                        \
        {                                                                      \
                NAME##_free(self);                                             \
                self->size = size;                                             \
                chk_malloc(self->array, PAYLOAD_TYPE, self->size);             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_realloc(                                                    \
                struct NAME *self, const uint64_t size)                        \
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
        int NAME##_set(                                                        \
                struct NAME *self,                                             \
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
        int NAME##_get(                                                        \
                struct NAME *self,                                             \
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
