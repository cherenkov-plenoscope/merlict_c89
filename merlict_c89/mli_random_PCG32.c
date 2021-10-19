/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_random_PCG32.h"
#include "mli_random_pcg_variants_32bit_subset.c"

struct mliPCG32 mliPCG32_init(const uint32_t seed)
{
        struct mliPCG32 pcg32;
        pcg_setseq_64_srandom_r(&pcg32.state_setseq_64, seed, 0u);
        return pcg32;
}

uint32_t mliPCG32_generate_uint32(struct mliPCG32 *pcg32)
{
        return pcg_setseq_64_xsh_rr_32_random_r(&pcg32->state_setseq_64);
}

void mliPCG32_reinit(struct mliPCG32 *pcg32, const uint32_t seed)
{
        pcg_setseq_64_srandom_r(&pcg32->state_setseq_64, seed, 0u);
}
