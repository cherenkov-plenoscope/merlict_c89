/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIMT19937_H_INCLUDED__
#define MERLICT_MLIMT19937_H_INCLUDED__

#include <stdint.h>

/*
 *      Adopted from https://en.wikipedia.org/wiki/Mersenne_Twister
 */
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

void _mliMT19937_set_constants(struct mliMT19937 *mt)
{
        /*
         *      Define MT19937 constants (32-bit RNG)
         *      Assumes W = 32 (omitting this)
         */
        mt->N = 624,
        mt->M = 397;
        mt->R = 31;
        mt->A = 0x9908B0DF;
        mt->F = 1812433253;
        mt->U = 11;
        /*       Assumes D = 0xFFFFFFFF (omitting this) */
        mt->S = 7;
        mt->B = 0x9D2C5680;
        mt->T = 15;
        mt->C = 0xEFC60000;
        mt->L = 18;
        mt->MASK_LOWER = (1u << mt->R) - 1;
        mt->MASK_UPPER = (1u << mt->R);
}

void _mliMT19937_reinit(struct mliMT19937 *mt, const uint32_t seed)
{
        uint32_t i;
        _mliMT19937_set_constants(mt);
        mt->mt[0] = seed;
        for (i = 1; i < mt->N; i++) {
                mt->mt[i] =
                        (mt->F * (mt->mt[i - 1] ^ (mt->mt[i - 1] >> 30)) + i);
        }
        mt->index = mt->N;
}

struct mliMT19937 mliMT19937_init(const uint32_t seed)
{
        struct mliMT19937 mt;
        _mliMT19937_reinit(&mt, seed);
        return mt;
}

void _mliMT19937_twist(struct mliMT19937 *mt)
{
        uint32_t i, x, xA;
        for (i = 0; i < mt->N; i++) {
                x = (mt->mt[i] & mt->MASK_UPPER) +
                        (mt->mt[(i + 1) % mt->N] & mt->MASK_LOWER);
                xA = x >> 1;
                if (x & 0x1) {
                        xA ^= mt->A;
                }
                mt->mt[i] = mt->mt[(i + mt->M) % mt->N] ^ xA;
        }
        mt->index = 0;
}

uint32_t mliMT19937_uint32(struct mliMT19937 *mt)
{
        uint32_t y;
        int i = mt->index;
        if (mt->index >= mt->N) {
                _mliMT19937_twist(mt);
                i = mt->index;
        }
        y = mt->mt[i];
        mt->index = i + 1;
        y ^= (mt->mt[i] >> mt->U);
        y ^= (y << mt->S) & mt->B;
        y ^= (y << mt->T) & mt->C;
        y ^= (y >> mt->L);
        return y;
}

#endif
