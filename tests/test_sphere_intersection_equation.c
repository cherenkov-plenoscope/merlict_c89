/* Copyright 2019-2020 Sebastian Achim Mueller                                */

{
    double radius = 1.;
    double plus_solution, minus_solution;
    mliRay ray;
    ray.support = mliVec_set(0., 0., -10);
    ray.direction = mliVec_set(0., 0., 1.);
    CHECK(
        mli_sphere_intersection_equation(
            radius,
            ray,
            &plus_solution,
            &minus_solution));
    CHECK_MARGIN(plus_solution, 11., 1e-6);
    CHECK_MARGIN(minus_solution, 9., 1e-6);
}

{
    double radius = 1.;
    double plus_solution, minus_solution;
    mliRay ray;
    ray.support = mliVec_set(0., 0., 10);
    ray.direction = mliVec_set(0., 0., 1.);
    CHECK(
        mli_sphere_intersection_equation(
            radius,
            ray,
            &plus_solution,
            &minus_solution));
}

{
    double radius = 1.;
    double plus_solution, minus_solution;
    mliRay ray;
    ray.support = mliVec_set(0., 2., -10);
    ray.direction = mliVec_set(0., 0., 1.);
    CHECK(
        !mli_sphere_intersection_equation(
            radius,
            ray,
            &plus_solution,
            &minus_solution));
}
