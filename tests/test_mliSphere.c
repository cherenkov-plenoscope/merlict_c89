/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliSphere_intersection") {
    mliSphere sphere;
    mliIntersection intersection;
    mliHomTraComp local2root_comp;
    sphere.radius = 1.;
    local2root_comp.trans = mliVec_set(0., 0., 0.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 0.),
        0.);

    CHECK(!mliSphere_intersection(
        sphere,
        local2root_comp,
        mliRay_set(mliVec_set(0., 0., 5.), mliVec_set(0., 0., 1.)),
        &intersection));

    CHECK(mliSphere_intersection(
        sphere,
        local2root_comp,
        mliRay_set(mliVec_set(0., 0., -5.), mliVec_set(0., 0., 1.)),
        &intersection));
    CHECK_MARGIN(intersection.distance_of_ray, 4., 1e-6);
}

CASE("mliSphere_obb") {
    double c;
    for (c = -2.5; c < 2.5; c += .25) {
        mliOBB b;
        double radius = 1.;
        b = mliSphere_obb(radius, mliVec_set(c, c*2., c*3.));
        CHECK_MARGIN(b.lower.x, (-1. + c), 1e-6);
        CHECK_MARGIN(b.lower.y, (-1. + c*2), 1e-6);
        CHECK_MARGIN(b.lower.z, (-1. + c*3), 1e-6);
        CHECK_MARGIN(b.upper.x, ( 1. + c), 1e-6);
        CHECK_MARGIN(b.upper.y, ( 1. + c*2), 1e-6);
        CHECK_MARGIN(b.upper.z, ( 1. + c*3), 1e-6);
    }
}

CASE("mliSphere_has_overlap_obb") {
    double radius = 1.;
    mliOBB obb;
    obb.lower = mliVec_set(-1., -1., -1.);
    obb.upper = mliVec_set(1., 1., 1.);
    CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(0., 0., 0.), obb));
    CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(0., 0., .5), obb));
    CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(.5, .5, .5), obb));
    CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(.0, .0, 2.5), obb));
    CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(.0, 2.5, 0.), obb));
    CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(2.5, 0., 0.), obb));
    CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(-2.5, 0., 0.), obb));
    CHECK(mliSphere_has_overlap_obb(radius, mliVec_set(1.99, 0., 0.), obb));
    CHECK(!mliSphere_has_overlap_obb(radius, mliVec_set(2.01, 0., 0.), obb));
}
