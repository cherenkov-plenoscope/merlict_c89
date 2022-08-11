/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RANDOM_MT19937_H_
#define MLI_RANDOM_MT19937_H_

#include <stdint.h>

struct mliMT19937 {
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

uint32_t mliMT19937_generate_uint32(struct mliMT19937 *mt);
void mliMT19937_twist(struct mliMT19937 *mt);
struct mliMT19937 mliMT19937_init(const uint32_t seed);
void mliMT19937_reinit(struct mliMT19937 *mt, const uint32_t seed);
void mliMT19937_set_constants(struct mliMT19937 *mt);
#endif
