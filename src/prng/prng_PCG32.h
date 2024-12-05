/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PRNG_PCG32_H_
#define MLI_PRNG_PCG32_H_

#include <stdint.h>
#include "prng_pcg_variants_32bit_subset.h"

/*      Wrapping the pcg implementation by Melissa O'Neill in
 *      pcg_variants_32bit_subset.h
 */

struct mli_prng_PCG32 {
        struct mli_prng_pcg_state_setseq_64 state_setseq_64;
};

struct mli_prng_PCG32 mli_prng_PCG32_init(const uint32_t seed);
uint32_t mli_prng_PCG32_generate_uint32(struct mli_prng_PCG32 *pcg32);
void mli_prng_PCG32_reinit(struct mli_prng_PCG32 *pcg32, const uint32_t seed);

#endif
