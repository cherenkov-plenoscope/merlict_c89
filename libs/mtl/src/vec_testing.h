/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_VEC_TESTING_H_
#define MTL_VEC_TESTING_H_

#include <stdint.h>

#define MTL_VEC_TESTING_DEFINITON(LIB, NAME, PAYLOAD_TYPE)                     \
                                                                               \
        int LIB##Dyn##NAME##_test_init(struct LIB##Dyn##NAME *dh);             \
                                                                               \
        int LIB##Dyn##NAME##_test_malloc(                                      \
                struct LIB##Dyn##NAME *dh, const uint64_t capacity);           \
                                                                               \
        int LIB##Dyn##NAME##_test_free(struct LIB##Dyn##NAME *dh);

#define MTL_VEC_TESTING_IMPLEMENTATION(LIB, NAME, PAYLOAD_TYPE)                \
                                                                               \
        int LIB##Dyn##NAME##_test_init(struct LIB##Dyn##NAME *dh)              \
        {                                                                      \
                chk(dh->capacity == 0u);                                       \
                chk(dh->size == 0u);                                           \
                chk(dh->array == NULL);                                        \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_test_malloc(                                      \
                struct LIB##Dyn##NAME *dh, const uint64_t capacity)            \
        {                                                                      \
                chk(dh->capacity >= dh->size);                                 \
                if (capacity < 2) {                                            \
                        chk(dh->capacity == 2);                                \
                } else {                                                       \
                        chk(dh->capacity == capacity);                         \
                }                                                              \
                chk(dh->array != NULL);                                        \
                return 1;                                                      \
        chk_error:                                                             \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        int LIB##Dyn##NAME##_test_free(struct LIB##Dyn##NAME *dh)              \
        {                                                                      \
                return LIB##Dyn##NAME##_test_init(dh);                         \
        }

#endif
