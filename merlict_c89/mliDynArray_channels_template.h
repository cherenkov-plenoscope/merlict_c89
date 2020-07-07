/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDYNARRAY_CHANNELS_TEMPLATE_H_
#define MERLICT_C89_MLIDYNARRAY_CHANNELS_TEMPLATE_H_

#include "mliDynArray.h"
#include "mliDynArray_template.h"
#include "mliColor.h"

#define MLIDYNARRAY_2D_DEFINITON(LIB, NAME, PAYLOAD_NAME)                      \
                                                                               \
        struct LIB##NAME##Channels {                                           \
                uint64_t num_channels;                                         \
                struct PAYLOAD_NAME *channels;                                 \
        };                                                                     \
                                                                               \
        struct LIB##NAME##Channels LIB##NAME##Channels_init(void);             \
                                                                               \
        void LIB##NAME##Channels_free(struct LIB##NAME##Channels *phs);        \
                                                                               \
        int LIB##NAME##Channels_malloc(                                        \
                struct LIB##NAME##Channels *phs, const uint64_t num_channels); \
                                                                               \
        uint64_t LIB##NAME##Channels_total_num(                                \
                const struct LIB##NAME##Channels *phs);

#define MLIDYNARRAY_2D_IMPLEMENTATION(LIB, NAME, PAYLOAD_NAME)                 \
                                                                               \
        struct LIB##NAME##Channels LIB##NAME##Channels_init(void)              \
        {                                                                      \
                struct LIB##NAME##Channels phs;                                \
                phs.num_channels = 0u;                                         \
                phs.channels = NULL;                                           \
                return phs;                                                    \
        }                                                                      \
                                                                               \
        void LIB##NAME##Channels_free(struct LIB##NAME##Channels *phs)         \
        {                                                                      \
                uint64_t ch;                                                   \
                for (ch = 0u; ch < phs->num_channels; ch++) {                  \
                        PAYLOAD_NAME##_free(&phs->channels[ch]);               \
                }                                                              \
                free(phs->channels);                                           \
                phs->num_channels = 0u;                                        \
                phs->channels = NULL;                                          \
        }                                                                      \
                                                                               \
        int LIB##NAME##Channels_malloc(                                        \
                struct LIB##NAME##Channels *phs, const uint64_t num_channels)  \
        {                                                                      \
                uint64_t ch = 0u;                                              \
                LIB##NAME##Channels_free(phs);                                 \
                phs->num_channels = num_channels;                              \
                mli_malloc(                                                    \
                        phs->channels,                                         \
                        struct PAYLOAD_NAME,                                   \
                        phs->num_channels);                                    \
                for (ch = 0; ch < phs->num_channels; ch++) {                   \
                        phs->channels[ch] = PAYLOAD_NAME##_init();             \
                }                                                              \
                for (ch = 0; ch < phs->num_channels; ch++) {                   \
                        mli_c(PAYLOAD_NAME##_malloc(&phs->channels[ch], 0u));  \
                }                                                              \
                return 1;                                                      \
        error:                                                                 \
                return 0;                                                      \
        }                                                                      \
                                                                               \
        uint64_t LIB##NAME##Channels_total_num(                                \
                const struct LIB##NAME##Channels *phs)                         \
        {                                                                      \
                uint64_t num_pulses = 0u;                                      \
                uint64_t ch = 0u;                                              \
                for (ch = 0; ch < phs->num_channels; ch++) {                   \
                        num_pulses += phs->channels[ch].dyn.size;              \
                }                                                              \
                return num_pulses;                                             \
        }

MLIDYNARRAY_2D_DEFINITON(mli, Color, mliDynColor)

#endif
