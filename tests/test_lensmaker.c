/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("check_sebastians_paper_and_pen_calculation")
{
        struct mliLensMakerConfig cfg;
        double expected_curvature_radius = 0.125;
        double result_curvature_radius = 0.0;
        cfg.focal_length = 0.1335;
        cfg.aperture_radius = 0.071;
        cfg.refractive_index = 1.49;
        cfg.acceptable_focal_length_deviation = cfg.focal_length * 1e-3;
        cfg.max_iterations = 100;

        CHECK(mli_lens_maker_estimate_curvature_radius(
                cfg, &result_curvature_radius));

        CHECK_MARGIN(
                expected_curvature_radius,
                result_curvature_radius,
                expected_curvature_radius * 3e-2);
}

CASE("lens_thicknes")
{
        const double expected_curvature_radius = 0.125;
        const double expected_thickness = 0.0445;
        const double aperture_radius = 0.071;

        CHECK_MARGIN(
                expected_thickness,
                _mli_thickness_for_curvature_and_aperture_radius(
                        expected_curvature_radius, aperture_radius),
                expected_thickness * 3e-2);
}

CASE("check_abort_too_many_iterations")
{
        struct mliLensMakerConfig cfg;
        double result_curvature_radius = 0.0;
        cfg.focal_length = 0.1335;
        cfg.aperture_radius = 0.071;
        cfg.refractive_index = 1.49;
        cfg.acceptable_focal_length_deviation = cfg.focal_length * 1e-3;
        cfg.max_iterations = 3;

        CHECK(0 == mli_lens_maker_estimate_curvature_radius(
                           cfg, &result_curvature_radius));
}
