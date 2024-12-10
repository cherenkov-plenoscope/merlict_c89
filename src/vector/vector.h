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
        int NAME##_malloc(struct NAME *self, const uint64_t capacity);         \
        int NAME##_realloc(struct NAME *self, const uint64_t capacity);        \
                                                                               \
        int NAME##_push_back(struct NAME *self, PAYLOAD_TYPE item);            \
                                                                               \
        int NAME##_set(                                                        \
                struct NAME *self, const uint64_t at, PAYLOAD_TYPE item);      \
                                                                               \
        int NAME##_get(                                                        \
                const struct NAME *self,                                       \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item);                                           \
        int NAME##_copy(struct NAME *dst, const struct NAME *src);             \
        int NAME##_copyn(                                                      \
                struct NAME *dst,                                              \
                const struct NAME *src,                                        \
                const uint64_t start,                                          \
                const uint64_t length);

#define MLI_VECTOR_IMPLEMENTATION_MALLOC(NAME, PAYLOAD_TYPE)                   \
        int NAME##_malloc(struct NAME *self, const uint64_t capacity)          \
        {                                                                      \
                NAME##_free(self);                                             \
                self->capacity = MLI_MATH_MAX2(2, capacity);                   \
                self->size = 0;                                                \
                chk_malloc(self->array, PAYLOAD_TYPE, self->capacity);         \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_realloc(struct NAME *self, const uint64_t capacity)         \
        {                                                                      \
                PAYLOAD_TYPE *new_array = (PAYLOAD_TYPE *)realloc(             \
                        (void *)self->array, capacity * sizeof(PAYLOAD_TYPE)); \
                chk_mem(new_array);                                            \
                self->array = new_array;                                       \
                self->capacity = capacity;                                     \
                if (self->capacity < self->size) {                             \
                        self->size = self->capacity;                           \
                }                                                              \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#define MLI_VECTOR_IMPLEMENTATION_MALLOC_ZERO_TERMINATION(NAME, PAYLOAD_TYPE)  \
        int NAME##_malloc(struct NAME *self, const uint64_t capacity)          \
        {                                                                      \
                NAME##_free(self);                                             \
                self->capacity = MLI_MATH_MAX2(2, capacity);                   \
                self->size = 0;                                                \
                chk_malloc(self->array, PAYLOAD_TYPE, self->capacity + 1);     \
                memset(self->array,                                            \
                       '\0',                                                   \
                       (self->capacity + 1) * sizeof(PAYLOAD_TYPE));           \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int NAME##_realloc(struct NAME *self, const uint64_t capacity)         \
        {                                                                      \
                PAYLOAD_TYPE *new_array = (PAYLOAD_TYPE *)realloc(             \
                        (void *)self->array,                                   \
                        (capacity + 1) * sizeof(PAYLOAD_TYPE));                \
                chk_mem(new_array);                                            \
                self->array = new_array;                                       \
                self->capacity = capacity;                                     \
                                                                               \
                if (self->capacity < self->size) {                             \
                        self->size = self->capacity;                           \
                } else {                                                       \
                        int64_t num_fields_after_size =                        \
                                ((int64_t)self->capacity -                     \
                                 (int64_t)self->size);                         \
                        memset(&self->array[self->size],                       \
                               '\0',                                           \
                               num_fields_after_size * sizeof(PAYLOAD_TYPE));  \
                }                                                              \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#define MLI_VECTOR_IMPLEMENTATION_BASICS(NAME, PAYLOAD_TYPE)                   \
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
        int NAME##_push_back(struct NAME *self, PAYLOAD_TYPE item)             \
        {                                                                      \
                if (self->size == self->capacity) {                            \
                        chk_msg(NAME##_realloc(self, self->capacity * 2),      \
                                "Failed to grow vector.");                     \
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
                const struct NAME *self,                                       \
                const uint64_t at,                                             \
                PAYLOAD_TYPE *item)                                            \
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
                        "Expected start + length <= src->size.")               \
                        NAME##_malloc(dst, length);                            \
                memcpy(dst->array,                                             \
                       &src->array[start],                                     \
                       length * sizeof(PAYLOAD_TYPE));                         \
                dst->size = length;                                            \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }

#define MLI_VECTOR_IMPLEMENTATION(NAME, PAYLOAD_TYPE)                          \
        MLI_VECTOR_IMPLEMENTATION_MALLOC(NAME, PAYLOAD_TYPE)                   \
        MLI_VECTOR_IMPLEMENTATION_BASICS(NAME, PAYLOAD_TYPE)

#define MLI_VECTOR_IMPLEMENTATION_ZERO_TERMINATION(NAME, PAYLOAD_TYPE)         \
        MLI_VECTOR_IMPLEMENTATION_MALLOC_ZERO_TERMINATION(NAME, PAYLOAD_TYPE)  \
        MLI_VECTOR_IMPLEMENTATION_BASICS(NAME, PAYLOAD_TYPE)

#endif
