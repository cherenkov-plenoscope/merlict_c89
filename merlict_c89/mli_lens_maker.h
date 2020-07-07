/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_LENS_MAKER_H_
#define MERLICT_C89_MLI_LENS_MAKER_H_

#include <math.h>
#include <stdint.h>

struct mliLensMakerConfig {
        double focal_length;
        double aperture_radius;
        double refractive_index;
        double acceptable_focal_length_deviation;
        uint64_t max_iterations;
};

int mli_lens_maker_estimate_curvature_radius(
        const struct mliLensMakerConfig config,
        double *out_curvature_radius);
double _mli_thickness_for_curvature_and_aperture_radius(
        const double R,
        const double r);
double _mli_focal_length_for_curvature_thickness_refraction(
        const double R,
        const double t,
        const double n);
#endif
