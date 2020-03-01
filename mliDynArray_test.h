/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIDYNARRAY_TEST_H_
#define MERLICT_MLIDYNARRAY_TEST_H_

#include "mliDynArray.h"
#include "mliColor.h"


#define MLIDYNARRAY_TEMPLATE(LIB, NAME, PAYLOAD_TYPE) \
 \
struct LIB##Dyn##NAME { \
        struct mliDynArray dyn; \
        PAYLOAD_TYPE *arr; \
}; \
 \
struct LIB##Dyn##NAME LIB##Dyn##NAME##_init() { \
        struct LIB##Dyn##NAME dh; \
        mliDynArray_init_in_host(&dh.dyn, (void **)&dh.arr, sizeof(*dh.arr)); \
        return dh; \
} \
 \
void LIB##Dyn##NAME##_free(struct LIB##Dyn##NAME *dh) { \
        mliDynArray_free_in_host(&dh->dyn, (void **)&dh->arr); \
} \
 \
int LIB##Dyn##NAME##_malloc(struct LIB##Dyn##NAME *dh, const uint64_t cap) { \
        return mliDynArray_malloc_in_host(&dh->dyn, (void **)&dh->arr, cap); \
} \
 \
int LIB##Dyn##NAME##_push_back( \
        struct LIB##Dyn##NAME *dh, \
        const PAYLOAD_TYPE *pl) { \
        return mliDynArray_push_back_in_host( \
                &dh->dyn, (void **)&dh->arr, pl); \
} \
 \
int _##LIB##Dyn##NAME##_test_after_init(const struct LIB##Dyn##NAME *dh) { \
        return _mliDynArray_test_after_init( \
                &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr)); \
} \
 \
int _##LIB##Dyn##NAME##_test_after_free(const struct LIB##Dyn##NAME *dh) { \
        return _mliDynArray_test_after_free( \
                &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr)); \
} \
 \
int _##LIB##Dyn##NAME##_test_after_malloc( \
        const struct LIB##Dyn##NAME *dh, \
        uint64_t cap) \
{ \
        return _mliDynArray_test_after_malloc( \
                &dh->dyn, (const void **)&dh->arr, sizeof(*dh->arr), cap); \
} \


MLIDYNARRAY_TEMPLATE(mli, Color, struct mliColor)


#define MLIDYNARRAY_2D_TEMPLATE(LIB, NAME, PAYLOAD_NAME) \
 \
struct LIB##NAME##Channels { \
        uint64_t num_channels; \
        struct PAYLOAD_NAME *channels; \
}; \
 \
struct LIB##NAME##Channels LIB##NAME##Channels_init() \
{ \
        struct LIB##NAME##Channels phs; \
        phs.num_channels = 0u; \
        phs.channels = NULL; \
        return phs; \
} \
 \
void LIB##NAME##Channels_free(struct LIB##NAME##Channels *phs) \
{ \
        uint64_t ch; \
        for (ch = 0u; ch < phs->num_channels; ch++) { \
                PAYLOAD_NAME##_free(&phs->channels[ch]); \
        } \
        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__); \
        free(phs->channels); \
        phs->num_channels = 0u; \
        phs->channels = NULL; \
} \
 \
int LIB##NAME##Channels_malloc( \
        struct LIB##NAME##Channels *phs, \
        const uint64_t num_channels) \
{ \
        uint64_t ch = 0u; \
        LIB##NAME##Channels_free(phs); \
        phs->num_channels = num_channels; \
        mli_malloc(phs->channels, struct PAYLOAD_NAME, phs->num_channels); \
        for (ch = 0; ch < phs->num_channels; ch++) { \
                phs->channels[ch] = PAYLOAD_NAME##_init(); \
        } \
        for (ch = 0; ch < phs->num_channels; ch++) { \
                mli_c(PAYLOAD_NAME##_malloc(&phs->channels[ch], 0u)); \
        } \
        return 1; \
    error: \
        return 0; \
} \
 \
uint64_t LIB##NAME##Channels_total_num(const struct LIB##NAME##Channels *phs) \
{ \
        uint64_t num_pulses = 0u; \
        uint64_t ch = 0u; \
        for (ch = 0; ch < phs->num_channels; ch++) { \
                num_pulses += phs->channels[ch].dyn.size; \
        } \
        return num_pulses; \
} \

MLIDYNARRAY_2D_TEMPLATE(mli, Color, mliDynColor)

#endif
