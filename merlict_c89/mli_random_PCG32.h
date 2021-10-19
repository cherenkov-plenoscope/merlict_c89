/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_RANDOM_PCG32_H_
#define MERLICT_C89_MLI_RANDOM_PCG32_H_

#include <stdint.h>
#include "mli_random_pcg_variants_32bit_subset.h"

/*      Wrapping the pcg implementation by Melissa O'Neill in
 *      pcg_variants_32bit_subset.h
 */

struct mliPCG32 {
        struct pcg_state_setseq_64 state_setseq_64;
};

struct mliPCG32 mliPCG32_init(const uint32_t seed);
uint32_t mliPCG32_generate_uint32(struct mliPCG32 *pcg32);
void mliPCG32_reinit(struct mliPCG32 *pcg32, const uint32_t seed);

#endif
