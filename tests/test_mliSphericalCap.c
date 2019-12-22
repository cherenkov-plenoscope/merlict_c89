/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliSphericalCap") {
    const double radius = 0.3;
    const mliVec support = {0., 0., 1.};
    const mliVec direction = {0., 0., -1.};
    const mliRay ray = mliRay_set(support, direction);
    double plus_solution, minus_solution;
    mliVec intersection_point;
    mliVec surface_normal;
    CHECK(mli_spherical_cap_equation(
            ray,
            radius,
            &plus_solution,
            &minus_solution));
    CHECK_MARGIN(plus_solution, 1.0, 1e-9);
    CHECK_MARGIN(minus_solution, (1.0 - 2.*radius), 1e-9);

    intersection_point = mliRay_at(&ray, plus_solution);
    surface_normal = mli_spherical_cap_surface_normal(
        intersection_point,
        radius);
    CHECK_MARGIN(surface_normal.x, 0., 1e-9);
    CHECK_MARGIN(surface_normal.y, 0., 1e-9);
    CHECK_MARGIN(surface_normal.z, 1., 1e-9);
}
