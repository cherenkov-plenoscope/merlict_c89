/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_lens_maker.h"

/* Copyright 2019 Sebastian Achim Mueller */

double _mli_focal_length_for_curvature_thickness_refraction(
        const double R,
        const double t,
        const double n)
{
        double f_inv = (n - 1.0) * (2.0 / R - ((n - 1.0) * t) / (n * R * R));
        return 1.0 / f_inv;
}

double _mli_thickness_for_curvature_and_aperture_radius(
        const double R,
        const double r)
{
        return 2.0 * R - 2.0 * sqrt(R * R - r * r);
}

int mli_lens_maker_estimate_curvature_radius(
        const struct mliLensMakerConfig config,
        double *out_curvature_radius)
{
        double curvature_radius = config.focal_length; /* start point */
        double deviation = 0.0;
        double expected_focal_length = 0.0;
        uint64_t iteration = 0;

        do {
                mli_check(
                        iteration < config.max_iterations,
                        "Too many iterations in lens-maker.");

                /* adjust curvature radius */
                curvature_radius = curvature_radius - deviation * 0.1;

                /* expected focal length for adjusted curvature radius */
                expected_focal_length =
                        _mli_focal_length_for_curvature_thickness_refraction(
                                curvature_radius,
                                _mli_thickness_for_curvature_and_aperture_radius(
                                        curvature_radius,
                                        config.aperture_radius),
                                config.refractive_index);
                iteration++;

                deviation = expected_focal_length - config.focal_length;
        } while (fabs(deviation) > config.acceptable_focal_length_deviation);

        (*out_curvature_radius) = curvature_radius;
        return 1;
error:
        return 0;
}
