/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_RANDOM_H_
#define MERLICT_MLI_RANDOM_H_

#include <math.h>
#include <stdint.h>
#include <assert.h>
#include "mliVec.h"

double mli_random_uniform(struct mliMT19937 *prng)
{
        uint32_t rn_int = mliMT19937_uint32(prng);
        const double rn = (double)rn_int;
        const double max_uint32 = (double)UINT32_MAX;
        return rn / max_uint32;
}

double mli_random_expovariate(struct mliMT19937 *prng, const double rate)
{
        /*      Sampling from a poisson distribution */
        return -log(mli_random_uniform(prng)) / rate;
}

double mli_random_normal_Irwin_Hall_approximation(
        struct mliMT19937 *prng,
        const double mean,
        const double std)
{
        uint64_t i;
        double sum_of_12 = 0.;
        double std1 = 0.;
        for (i = 0; i < 12; i++) {
                sum_of_12 += mli_random_uniform(prng);
        }
        std1 = sum_of_12 - 6.;
        return mean + std1 * std;
}

/*
        uniform linear range
        ====================
        Draw a uniform distribution within a limited range.
*/
struct mliRandomUniformRange {
        double start;
        double range;
};

struct mliRandomUniformRange mliRandomUniformRange_set(
        double start,
        double stop)
{
        struct mliRandomUniformRange p;
        p.start = start;
        p.range = stop - start;
        assert(p.range >= 0.0);
        return p;
}

double mli_random_draw_uniform(
        const struct mliRandomUniformRange uniform_range,
        struct mliMT19937 *prng)
{
        return uniform_range.range * mli_random_uniform(prng) +
               uniform_range.start;
}

/*
        zenith range
        ============
        Draw zenith-distances for an even distribution of points on a sphere.
*/
struct mliRandomZenithRange {
        double z_min;
        double z_range;
};

struct mliRandomZenithRange mliRandomZenithRange_set(
        const double min_zenith_distance,
        const double max_zenith_distance)
{
        struct mliRandomZenithRange zp;
        zp.z_min = (cos(min_zenith_distance) + 1.0) / 2.0;
        zp.z_range = (cos(max_zenith_distance) + 1.0) / 2.0 - zp.z_min;
        return zp;
}

double mli_random_draw_zenith(
        const struct mliRandomZenithRange range,
        struct mliMT19937 *prng)
{
        const double z =
                (range.z_range * mli_random_uniform(prng)) + range.z_min;
        return acos(2.0 * z - 1.0);
}

/*
        direction
        ==========
*/
struct mliVec mli_random_draw_direction_in_zenith_azimuth_range(
        const struct mliRandomZenithRange zenith,
        const struct mliRandomUniformRange azimuth,
        struct mliMT19937 *prng)
{
        const double az = mli_random_draw_uniform(azimuth, prng);
        const double zd = mli_random_draw_zenith(zenith, prng);
        const double sin_zd = sin(zd);
        return mliVec_set(sin_zd * cos(az), sin_zd * sin(az), cos(zd));
}

struct mliVec mli_random_position_on_disc(
        const double radius,
        struct mliMT19937 *prng)
{
        const double r = sqrt(mli_random_uniform(prng)) * radius;
        const double azimuth = mli_random_uniform(prng) * MLI_2PI;
        return mliVec_set(r * cos(azimuth), r * sin(azimuth), 0.0);
}

#endif
