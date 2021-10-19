/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_RANDOM_H_
#define MERLICT_C89_MLI_RANDOM_H_

#include "mli_random_generator.h"
#include "mliVec.h"

struct mliRandomUniformRange {
        double start;
        double range;
};

struct mliRandomZenithRange {
        double z_min;
        double z_range;
};

struct mliVec mli_random_position_on_disc(
        const double radius,
        struct mliPrng *prng);
struct mliVec mli_random_draw_direction_in_zenith_azimuth_range(
        const struct mliRandomZenithRange zenith,
        const struct mliRandomUniformRange azimuth,
        struct mliPrng *prng);
double mli_random_draw_zenith(
        const struct mliRandomZenithRange range,
        struct mliPrng *prng);
struct mliRandomZenithRange mliRandomZenithRange_set(
        const double min_zenith_distance,
        const double max_zenith_distance);
double mli_random_draw_uniform(
        const struct mliRandomUniformRange uniform_range,
        struct mliPrng *prng);
struct mliRandomUniformRange mliRandomUniformRange_set(
        double start,
        double stop);
double mli_random_normal_Irwin_Hall_approximation(
        struct mliPrng *prng,
        const double mean,
        const double std);
double mli_random_expovariate(struct mliPrng *prng, const double rate);
double mli_random_uniform(struct mliPrng *prng);
struct mliVec mli_random_position_inside_unit_sphere(struct mliPrng *prng);
#endif
