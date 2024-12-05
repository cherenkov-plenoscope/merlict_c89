/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VECTOR_H_
#define MLI_VECTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"

#define MLI_VECTOR_DEFINITON(NAME, PAYLOAD_TYPE)                               \
                                                                               \
        struct NAME {                                                          \
                uint64_t capacity;                                             \
                uint64_t size;                                                 \
                PAYLOAD_TYPE *array;                                           \
        };                                                                     \
                                                                               \
        struct NAME NAME##_init(void);                                         \
                                                                               \
        void NAME##_free(struct NAME *self);                                   \
                                                                               \
        int NAME##_malloc(struct NAME *self, const uint64_t size);             \
                                                                               \
        int NAME##_malloc_set_size(struct NAME *self, const uint64_t size);    \
                                                                               \
        int NAME##_push_back(struct NAME *self, PAYLOAD_TYPE item);            \
                                                                               \
        int NAME##_set(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE item);      \
                                                                               \
        int NAME##_get(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE *item);

#define MLI_VECTOR_IMPLEMENTATION(NAME, PAYLOAD_TYPE)                          \
                                                                               \
        struct NAME NAME##_init(void)                                          \
        {                                                                      \
                struct NAME out;                                               \
                out.capacity = 0u;                                             \
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
        int NAME##_malloc(struct NAME *self, const uint64_t size)              \
        {                                                                      \
                NAME##_free(self);                                             \
                self->capacity = MLI_MATH_MAX2(2, size);                       \
                self->size = 0;                                                \
                chk_malloc(self->array, PAYLOAD_TYPE, self->capacity);         \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_malloc_set_size(struct NAME *self, const uint64_t size)     \
        {                                                                      \
                chk(NAME##_malloc(self, size));                                \
                self->size = size;                                             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_push_back(struct NAME *self, PAYLOAD_TYPE item)             \
        {                                                                      \
                if (self->size == self->capacity) {                            \
                        self->capacity = self->capacity * 2;                   \
                        self->array = (PAYLOAD_TYPE *)realloc(                 \
                                (void *)self->array,                           \
                                self->capacity * sizeof(PAYLOAD_TYPE));        \
                        chk_mem(self->array);                                  \
                }                                                              \
                                                                               \
                self->array[self->size] = item;                                \
                self->size += 1;                                               \
                                                                               \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_set(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE item)       \
        {                                                                      \
                chk_msg(at < self->size, "Out of range.");                     \
                self->array[at] = item;                                        \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_get(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE *item)      \
        {                                                                      \
                chk_msg(at < self->size, "Out of range.");                     \
                (*item) = self->array[at];                                     \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#endif
