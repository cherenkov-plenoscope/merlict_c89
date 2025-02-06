/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VECTOR_TESTING_H_
#define MLI_VECTOR_TESTING_H_

#include <stdint.h>

#define MTL_VEC_TESTING_DEFINITON(NAME, PAYLOAD_TYPE)                          \
                                                                               \
        int NAME##_test_init(struct NAME *dh);                                 \
                                                                               \
        int NAME##_test_malloc(struct NAME *dh, const uint64_t capacity);      \
                                                                               \
        int NAME##_test_free(struct NAME *dh);

#define MTL_VEC_TESTING_IMPLEMENTATION(NAME, PAYLOAD_TYPE)                     \
                                                                               \
        int NAME##_test_init(struct NAME *dh)                                  \
        {                                                                      \
                chk(dh->capacity == 0u);                                       \
                chk(dh->size == 0u);                                           \
                chk(dh->array == NULL);                                        \
                return CHK_SUCCESS;                                            \
        chk_error:                                                             \
                return CHK_FAIL;                                               \
        }                                                                      \
                                                                               \
        int NAME##_test_malloc(struct NAME *dh, const uint64_t capacity)       \
        {                                                                      \
                chk(dh->capacity >= dh->size);                                 \
                if (capacity < 2) {                                            \
                        chk(dh->capacity == 2);                                \
                } else {                                                       \
                        chk(dh->capacity == capacity);                         \
                }                                                              \
                chk(dh->array != NULL);                                        \
                return CHK_SUCCESS;                                            \
        chk_error:                                                             \
                return CHK_FAIL;                                               \
        }                                                                      \
                                                                               \
        int NAME##_test_free(struct NAME *dh) { return NAME##_test_init(dh); }

#endif
