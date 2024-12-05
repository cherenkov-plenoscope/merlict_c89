#include "prng.h"
#include <assert.h>
#include <math.h>

uint32_t mli_Prng_generate_uint32(struct mli_Prng *prng)
{
        return prng->generate_uint32(&prng->_storage);
}

void mli_Prng_reinit(struct mli_Prng *prng, const uint32_t seed)
{
        prng->reinit(&prng->_storage, seed);
}

/**
 *      Mersenne Twister 19937
 *      ----------------------
 */

struct mli_Prng mli_Prng_init_MT19937(const uint32_t seed)
{
        struct mli_Prng prng;
        prng._storage.mt19937 = mli_prng_MT19937_init(seed);
        prng.generate_uint32 = mli_Prng_MT19937_generate_uint32;
        prng.reinit = mli_Prng_MT19937_reinit;
        return prng;
}

uint32_t mli_Prng_MT19937_generate_uint32(void *mt)
{
        return mli_prng_MT19937_generate_uint32((struct mli_prng_MT19937 *)mt);
}

void mli_Prng_MT19937_reinit(void *mt, const uint32_t seed)
{
        mli_prng_MT19937_reinit((struct mli_prng_MT19937 *)mt, seed);
}

/**
 *      PCG32
 *      -----
 */

struct mli_Prng mli_Prng_init_PCG32(const uint32_t seed)
{
        struct mli_Prng prng;
        prng._storage.pcg32 = mli_prng_PCG32_init(seed);
        prng.generate_uint32 = mli_Prng_PCG32_generate_uint32;
        prng.reinit = mli_Prng_PCG32_reinit;
        return prng;
}

uint32_t mli_Prng_PCG32_generate_uint32(void *pcg)
{
        return mli_prng_PCG32_generate_uint32((struct mli_prng_PCG32 *)pcg);
}

void mli_Prng_PCG32_reinit(void *pcg, const uint32_t seed)
{
        mli_prng_PCG32_reinit((struct mli_prng_PCG32 *)pcg, seed);
}

/**
 *      API
 *      ---
 *
 */

double mli_Prng_uniform(struct mli_Prng *prng)
{
        uint32_t rn_int = mli_Prng_generate_uint32(prng);
        const double rn = (double)rn_int;
        const double max_uint32 = (double)UINT32_MAX;
        return rn / max_uint32;
}

double mli_Prng_expovariate(struct mli_Prng *prng, const double rate)
{
        /*      Sampling from a poisson distribution */
        return -log(mli_Prng_uniform(prng)) / rate;
}

double mli_Prng_normal_Irwin_Hall_approximation(
        struct mli_Prng *prng,
        const double mean,
        const double std)
{
        uint64_t i;
        double sum_of_12 = 0.;
        double std1 = 0.;
        for (i = 0; i < 12; i++) {
                sum_of_12 += mli_Prng_uniform(prng);
        }
        std1 = sum_of_12 - 6.;
        return mean + std1 * std;
}

/*
        uniform linear range
        ====================
        Draw a uniform distribution within a limited range.
*/
struct mli_prng_UniformRange mli_prng_UniformRange_set(
        double start,
        double stop)
{
        struct mli_prng_UniformRange p;
        p.start = start;
        p.range = stop - start;
        assert(p.range >= 0.0);
        return p;
}

double mli_prng_draw_uniform(
        const struct mli_prng_UniformRange uniform_range,
        struct mli_Prng *prng)
{
        return uniform_range.range * mli_Prng_uniform(prng) +
               uniform_range.start;
}

/*
        zenith range
        ============
        Draw zenith-distances for an even distribution of points on a sphere.
*/
struct mli_prng_ZenithRange mli_prng_ZenithRange_set(
        const double min_zenith_distance,
        const double max_zenith_distance)
{
        struct mli_prng_ZenithRange zp;
        zp.z_min = (cos(min_zenith_distance) + 1.0) / 2.0;
        zp.z_range = (cos(max_zenith_distance) + 1.0) / 2.0 - zp.z_min;
        return zp;
}

double mli_prng_draw_zenith(
        const struct mli_prng_ZenithRange range,
        struct mli_Prng *prng)
{
        const double z = (range.z_range * mli_Prng_uniform(prng)) + range.z_min;
        return acos(2.0 * z - 1.0);
}
