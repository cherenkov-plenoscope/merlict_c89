/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_VECTOR_H_
#define MTL_VECTOR_H_

#include <stdint.h>
#include <stdlib.h>
#include "../../chk/src/chk.h"

#define MTL_MAX2(a, b) (((a) > (b)) ? (a) : (b))

#define MTL_VECTOR_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                          \
                                                                               \
        struct LIB##Dyn##NAME {                                                \
                uint64_t capacity;                                             \
                uint64_t size;                                                 \
                PAYLOAD_TYPE *array;                                           \
        };                                                                     \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void);                     \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *self);               \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *self, const uint64_t size);             \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *self, const uint64_t size);             \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *self, PAYLOAD_TYPE item);               \
                                                                               \
        int LIB##Dyn##NAME##_set(                                              \
                struct LIB##Dyn##NAME *self,                                   \
                const uint64_t at,                                             \
                PAYLOAD_TYPE item);                                            \
                                                                               \
        int LIB##Dyn##NAME##_get(                                              \
                struct LIB##Dyn##NAME *self,                                   \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item);


#define MTL_VECTOR_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                     \
                                                                               \
        struct LIB##Dyn##NAME LIB##Dyn##NAME##_init(void)                      \
        {                                                                      \
                struct LIB##Dyn##NAME out;                                     \
                out.capacity = 0u;                                             \
                out.size = 0u;                                                 \
                out.array = NULL;                                              \
                return out;                                                    \
        }                                                                      \
                                                                               \
        void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *self)                \
        {                                                                      \
                free(self->array);                                             \
                (*self) = LIB##Dyn##NAME##_init();                             \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc(                                           \
                struct LIB##Dyn##NAME *self, const uint64_t size)              \
        {                                                                      \
                LIB##Dyn##NAME##_free(self);                                   \
                self->capacity = MTL_MAX2(2, size);                            \
                self->size = 0;                                                \
                chk_malloc(self->array, PAYLOAD_TYPE, self->capacity);         \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_malloc_set_size(                                  \
                struct LIB##Dyn##NAME *self, const uint64_t size)              \
        {                                                                      \
                chk(LIB##Dyn##NAME##_malloc(self, size));                      \
                self->size = size;                                             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_push_back(                                        \
                struct LIB##Dyn##NAME *self, PAYLOAD_TYPE item)                \
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
        int LIB##Dyn##NAME##_set(                                              \
                struct LIB##Dyn##NAME *self,                                   \
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
        int LIB##Dyn##NAME##_get(                                              \
                struct LIB##Dyn##NAME *self,                                   \
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
