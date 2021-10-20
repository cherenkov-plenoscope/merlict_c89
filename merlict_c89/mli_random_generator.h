/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_RANDOM_GENERATOR_H_
#define MERLICT_C89_MLI_RANDOM_GENERATOR_H_

#include <stdint.h>
#include "mli_random_MT19937.h"
#include "mli_random_PCG32.h"

/**
 *      mliPrng is a transparent container to use different
 *      pseudo-random-number-generators (PRNGs) within merlict.
 *      It defines a minimal interface:
 *
 *      1) (Re)initializing with a seed.
 *      2) Generating the next random number uint32.
 *
 *      Merlict ships with two Prngs:
 *
 *      1) Mersenne Twister 19937
 *      2) PCG32
 *
 *      If you want to use your own, wrapp it here using mliPrng. See below.
 */

union mliPrngStorage {
        struct mliMT19937 mt19937;
        struct mliPCG32 pcg32;
        /* Add your own prng here */
};

struct mliPrng {
        union mliPrngStorage _storage;
        uint32_t (*generate_uint32)(void *);
        void (*reinit)(void *, const uint32_t);
};

uint32_t mliPrng_generate_uint32(struct mliPrng *prng);
void mliPrng_reinit(struct mliPrng *prng, const uint32_t seed);

/**
 *      Mersenne Twister 19937
 *      ----------------------
 */
struct mliPrng mliPrng_init_MT19937(const uint32_t seed);
uint32_t mliPrng_MT19937_generate_uint32(void *mt);
void mliPrng_MT19937_reinit(void *mt, const uint32_t seed);

/**
 *      PCG32
 *      -----
 */
struct mliPrng mliPrng_init_PCG32(const uint32_t seed);
uint32_t mliPrng_PCG32_generate_uint32(void *pcg);
void mliPrng_PCG32_reinit(void *pcg, const uint32_t seed);

/**
 *      Add your own prng here
 *      ----------------------
 */

#endif
