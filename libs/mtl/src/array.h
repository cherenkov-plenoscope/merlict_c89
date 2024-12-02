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
        void NAME##_free(struct NAME *self);                                   \
        int NAME##_malloc(struct NAME *self, const uint64_t size);             \
        int NAME##_realloc(struct NAME *self, const uint64_t size);            \
        int NAME##_set(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE item);      \
        int NAME##_get(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE *item);     \
        int NAME##_copy(struct NAME *dst, const struct NAME *src);             \
        int NAME##_copyn(                                                      \
                struct NAME *dst,                                              \
                const struct NAME *src,                                        \
                const uint64_t start,                                          \
                const uint64_t length);

#define MTL_ARRAY_IMPLEMENTATION_MALLOC(NAME, PAYLOAD_TYPE)                    \
        int NAME##_malloc(struct NAME *self, const uint64_t size)              \
        {                                                                      \
                NAME##_free(self);                                             \
                self->size = size;                                             \
                chk_malloc(self->array, PAYLOAD_TYPE, self->size);             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_realloc(struct NAME *self, const uint64_t size)             \
        {                                                                      \
                PAYLOAD_TYPE *new_array = (PAYLOAD_TYPE *)realloc(             \
                        (void *)self->array, size * sizeof(PAYLOAD_TYPE));     \
                chk_mem(new_array);                                            \
                self->array = new_array;                                       \
                self->size = size;                                             \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#define MTL_ARRAY_IMPLEMENTATION_MALLOC_ZERO_TERMINATION(NAME, PAYLOAD_TYPE)   \
        int NAME##_malloc(struct NAME *self, const uint64_t size)              \
        {                                                                      \
                NAME##_free(self);                                             \
                self->size = size;                                             \
                chk_malloc(self->array, PAYLOAD_TYPE, self->size + 1);         \
                memset(self->array,                                            \
                       '\0',                                                   \
                       (self->size + 1) * sizeof(PAYLOAD_TYPE));               \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_realloc(struct NAME *self, const uint64_t size)             \
        {                                                                      \
                PAYLOAD_TYPE *new_array = (PAYLOAD_TYPE *)realloc(             \
                        (void *)self->array,                                   \
                        (size + 1) * sizeof(PAYLOAD_TYPE));                    \
                chk_mem(new_array);                                            \
                self->array = new_array;                                       \
                self->size = size;                                             \
                memset(self->array,                                            \
                       '\0',                                                   \
                       (self->size + 1) * sizeof(PAYLOAD_TYPE));               \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#define MTL_ARRAY_IMPLEMENTATION_BASICS(NAME, PAYLOAD_TYPE)                    \
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
        }                                                                      \
                                                                               \
        int NAME##_copy(struct NAME *dst, const struct NAME *src)              \
        {                                                                      \
                return NAME##_copyn(dst, src, 0, src->size);                   \
        }                                                                      \
                                                                               \
        int NAME##_copyn(                                                      \
                struct NAME *dst,                                              \
                const struct NAME *src,                                        \
                const uint64_t start,                                          \
                const uint64_t length)                                         \
        {                                                                      \
                chk_msg(src->array != NULL, "Expected src to be allocated");   \
                chk_msg(start + length <= src->size,                           \
                        "Expected start + length < src->size.")                \
                        NAME##_malloc(dst, length);                            \
                memcpy(dst->array,                                             \
                       &src->array[start],                                     \
                       length * sizeof(PAYLOAD_TYPE));                         \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#define MTL_ARRAY_IMPLEMENTATION(NAME, PAYLOAD_TYPE)                           \
        MTL_ARRAY_IMPLEMENTATION_MALLOC(NAME, PAYLOAD_TYPE)                    \
        MTL_ARRAY_IMPLEMENTATION_BASICS(NAME, PAYLOAD_TYPE)

#define MTL_ARRAY_IMPLEMENTATION_ZERO_TERMINATION(NAME, PAYLOAD_TYPE)          \
        MTL_ARRAY_IMPLEMENTATION_MALLOC_ZERO_TERMINATION(NAME, PAYLOAD_TYPE)   \
        MTL_ARRAY_IMPLEMENTATION_BASICS(NAME, PAYLOAD_TYPE)

#endif
