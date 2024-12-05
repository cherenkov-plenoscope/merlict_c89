/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PRNG_MT19937_H_
#define MLI_PRNG_MT19937_H_

#include <stdint.h>

struct mli_prng_MT19937 {
        uint32_t N;
        uint32_t M;
        int R;
        int A;
        int F;
        int U;
        int S;
        int B;
        int T;
        int C;
        int L;
        int MASK_LOWER;
        int MASK_UPPER;
        uint32_t mt[624];
        uint16_t index;
};

uint32_t mli_prng_MT19937_generate_uint32(struct mli_prng_MT19937 *mt);
void mli_prng_MT19937_twist(struct mli_prng_MT19937 *mt);
struct mli_prng_MT19937 mli_prng_MT19937_init(const uint32_t seed);
void mli_prng_MT19937_reinit(struct mli_prng_MT19937 *mt, const uint32_t seed);
void mli_prng_MT19937_set_constants(struct mli_prng_MT19937 *mt);
#endif
