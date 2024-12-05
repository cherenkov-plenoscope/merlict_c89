/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_PRNG_H_
#define MTL_PRNG_H_

#include <stdint.h>
#include "prng_MT19937.h"
#include "prng_PCG32.h"

/**
 *      mtl_Prng is a transparent container to use different
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
 *      If you want to use your own, wrapp it here using mtl_Prng. See below.
 */

union mtl_PrngStorage {
        struct mtl_prng_MT19937 mt19937;
        struct mtl_prng_PCG32 pcg32;
        /* Add your own prng here */
};

struct mtl_Prng {
        union mtl_PrngStorage _storage;
        uint32_t (*generate_uint32)(void *);
        void (*reinit)(void *, const uint32_t);
};

uint32_t mtl_Prng_generate_uint32(struct mtl_Prng *prng);
void mtl_Prng_reinit(struct mtl_Prng *prng, const uint32_t seed);

/**
 *      Mersenne Twister 19937
 *      ----------------------
 */
struct mtl_Prng mtl_Prng_init_MT19937(const uint32_t seed);
uint32_t mtl_Prng_MT19937_generate_uint32(void *mt);
void mtl_Prng_MT19937_reinit(void *mt, const uint32_t seed);

/**
 *      PCG32
 *      -----
 */
struct mtl_Prng mtl_Prng_init_PCG32(const uint32_t seed);
uint32_t mtl_Prng_PCG32_generate_uint32(void *pcg);
void mtl_Prng_PCG32_reinit(void *pcg, const uint32_t seed);

/**
 *      Add your own prng here
 *      ----------------------
 */

/**
 *      API
 *      ---
 */
struct mtl_prng_UniformRange {
        double start;
        double range;
};

struct mtl_prng_ZenithRange {
        double z_min;
        double z_range;
};

double mtl_prng_draw_zenith(
        const struct mtl_prng_ZenithRange range,
        struct mtl_Prng *prng);
struct mtl_prng_ZenithRange mtl_prng_ZenithRange_set(
        const double min_zenith_distance,
        const double max_zenith_distance);
double mtl_prng_draw_uniform(
        const struct mtl_prng_UniformRange uniform_range,
        struct mtl_Prng *prng);
struct mtl_prng_UniformRange mtl_prng_UniformRange_set(
        double start,
        double stop);
double mtl_Prng_normal_Irwin_Hall_approximation(
        struct mtl_Prng *prng,
        const double mean,
        const double std);
double mtl_Prng_expovariate(struct mtl_Prng *prng, const double rate);
double mtl_Prng_uniform(struct mtl_Prng *prng);

#endif
