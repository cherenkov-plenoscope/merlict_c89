/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_PRNG_PCG32_H_
#define MTL_PRNG_PCG32_H_

#include <stdint.h>
#include "prng_pcg_variants_32bit_subset.h"

/*      Wrapping the pcg implementation by Melissa O'Neill in
 *      pcg_variants_32bit_subset.h
 */

struct mtl_prng_PCG32 {
        struct mtl_prng_pcg_state_setseq_64 state_setseq_64;
};

struct mtl_prng_PCG32 mtl_prng_PCG32_init(const uint32_t seed);
uint32_t mtl_prng_PCG32_generate_uint32(struct mtl_prng_PCG32 *pcg32);
void mtl_prng_PCG32_reinit(struct mtl_prng_PCG32 *pcg32, const uint32_t seed);

#endif
