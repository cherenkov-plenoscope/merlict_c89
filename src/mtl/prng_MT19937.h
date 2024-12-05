/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_PRNG_MT19937_H_
#define MTL_PRNG_MT19937_H_

#include <stdint.h>

struct mtl_prng_MT19937 {
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

uint32_t mtl_prng_MT19937_generate_uint32(struct mtl_prng_MT19937 *mt);
void mtl_prng_MT19937_twist(struct mtl_prng_MT19937 *mt);
struct mtl_prng_MT19937 mtl_prng_MT19937_init(const uint32_t seed);
void mtl_prng_MT19937_reinit(struct mtl_prng_MT19937 *mt, const uint32_t seed);
void mtl_prng_MT19937_set_constants(struct mtl_prng_MT19937 *mt);
#endif
