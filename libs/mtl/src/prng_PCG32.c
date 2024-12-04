/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "prng_PCG32.h"
#include "prng_pcg_variants_32bit_subset.h"

struct mtl_prng_PCG32 mtl_prng_PCG32_init(const uint32_t seed)
{
        struct mtl_prng_PCG32 pcg32;
        mtl_prng_pcg_setseq_64_srandom_r(&pcg32.state_setseq_64, seed, 0u);
        return pcg32;
}

uint32_t mtl_prng_PCG32_generate_uint32(struct mtl_prng_PCG32 *pcg32)
{
        return mtl_prng_pcg_setseq_64_xsh_rr_32_random_r(
                &pcg32->state_setseq_64);
}

void mtl_prng_PCG32_reinit(struct mtl_prng_PCG32 *pcg32, const uint32_t seed)
{
        mtl_prng_pcg_setseq_64_srandom_r(&pcg32->state_setseq_64, seed, 0u);
}
