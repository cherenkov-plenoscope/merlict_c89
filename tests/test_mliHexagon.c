/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliHexagon") {
    struct mliHexagon a;
    struct mliHexagon b;
    struct mliHexagon c;
    a.inner_radius = 1.;
    b.inner_radius = 1.;
    c.inner_radius = 0.5;
    CHECK(mliHexagon_is_equal(a, b));
    CHECK(!mliHexagon_is_equal(a, c));
    CHECK(!mliHexagon_is_equal(b, c));
}

CASE("mliHexagon_intersection") {
    struct mliHexagon hex;
    struct mliHomTraComp local2root_comp;
    struct mliIntersection intersection;
    hex.inner_radius = 1.;
    local2root_comp.trans = mliVec_set(0., 0., 0.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 1.),
        mli_deg2rad(0.));

    CHECK(
        mliHexagon_intersection(
            hex,
            local2root_comp,
            mliRay_set(
                mliVec_set(0., 0., 1.),
                mliVec_set(0., 0., -1.)),
            &intersection));
    CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

    /*
                  /\ y
              ____|____
             /    |    \
            /     |     \   <- P is inside
        __ /______|_____P\___\ x
           \      |      /   /
            \     |     /
             \____|____/
                  |
    */
    CHECK(
        mliHexagon_intersection(
            hex,
            local2root_comp,
            mliRay_set(
                mliVec_set(1.001, 0., 1.),
                mliVec_set(0., 0., -1.)),
            &intersection));
    CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

    /*
                  /\ y
              ____P____ <- P is outside
             /    |    \
            /     |     \
        __ /______|______\___\ x
           \      |      /   /
            \     |     /
             \____|____/
                  |
    */
    CHECK(
        !mliHexagon_intersection(
            hex,
            local2root_comp,
            mliRay_set(
                mliVec_set(0., 1.001, 1.),
                mliVec_set(0., 0., -1.)),
            &intersection));
}

CASE("struct mliHexagon transformation") {
    struct mliHexagon hex;
    struct mliHomTraComp local2root_comp;
    struct mliOBB obb;
    hex.inner_radius = 1.;

    local2root_comp.trans = mliVec_set(0., 0., 0.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 1.),
        mli_deg2rad(0.));
    obb = mliHexagon_obb(hex, local2root_comp);
    CHECK_MARGIN(obb.lower.x, -MLI_2_OVER_SQRT3, 1e-6);
    CHECK_MARGIN(obb.upper.x, +MLI_2_OVER_SQRT3, 1e-6);
    CHECK_MARGIN(obb.lower.y, -1., 1e-6);
    CHECK_MARGIN(obb.upper.y, +1., 1e-6);
    CHECK_MARGIN(obb.lower.z, 0., 1e-6);
    CHECK_MARGIN(obb.upper.z, 0., 1e-6);

    /* translation */
    local2root_comp.trans = mliVec_set(1., 2., 3.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 1.),
        mli_deg2rad(0.));
    obb = mliHexagon_obb(hex, local2root_comp);
    CHECK_MARGIN(obb.lower.x, -MLI_2_OVER_SQRT3 + 1., 1e-6);
    CHECK_MARGIN(obb.upper.x, +MLI_2_OVER_SQRT3 + 1., 1e-6);
    CHECK_MARGIN(obb.lower.y, -1. + 2., 1e-6);
    CHECK_MARGIN(obb.upper.y, +1. + 2., 1e-6);
    CHECK_MARGIN(obb.lower.z, 0. + 3., 1e-6);
    CHECK_MARGIN(obb.upper.z, 0. + 3., 1e-6);

    /* rotation y-axis 90deg*/
    local2root_comp.trans = mliVec_set(0., 0., 0.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 1., 0.),
        mli_deg2rad(90.));
    obb = mliHexagon_obb(hex, local2root_comp);
    CHECK_MARGIN(obb.lower.x, 0., 1e-6);
    CHECK_MARGIN(obb.upper.x, 0., 1e-6);
    CHECK_MARGIN(obb.lower.y, -1., 1e-6);
    CHECK_MARGIN(obb.upper.y, +1., 1e-6);
    CHECK_MARGIN(obb.lower.z, -MLI_2_OVER_SQRT3, 1e-6);
    CHECK_MARGIN(obb.upper.z, +MLI_2_OVER_SQRT3, 1e-6);

    /* rotation y-axis 45deg */
    local2root_comp.trans = mliVec_set(0., 0., 0.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 1., 0.),
        mli_deg2rad(45.));
    obb = mliHexagon_obb(hex, local2root_comp);
    CHECK_MARGIN(obb.lower.x, -MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
    CHECK_MARGIN(obb.upper.x, +MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
    CHECK_MARGIN(obb.lower.y, -1., 1e-6);
    CHECK_MARGIN(obb.upper.y, +1., 1e-6);
    CHECK_MARGIN(obb.lower.z, -MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
    CHECK_MARGIN(obb.upper.z, +MLI_2_OVER_SQRT3/sqrt(2), 1e-6);
}

CASE("mliHexagon_has_overlap_obb") {
    struct mliHexagon hex;
    struct mliHomTraComp local2root_comp;
    struct mliOBB obb;
    hex.inner_radius = 1.;
    local2root_comp.trans = mliVec_set(0., 0., 0.);
    local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 1.),
        mli_deg2rad(0.));

    /*
        _obb____________________  + 2
       |                        |
       |                        |
       |       _________ + 1    |
       |      /         \       |
       |     /           \      |
       |    /             \     |
       |    \             /     |
       |     \           /      |
       |      \_________/ - 1   |
       |                        |
       |                        |
       |________________________| - 2

        y
        |__x
    */
    obb.lower = mliVec_set(-2., -2., -2.);
    obb.upper = mliVec_set(2., 2., 2.);
    CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

    /*
           obb   |                                                         *
                 |                                                         *
               __|______ + 1                                               *
        _____ /__|      \                                                  *
             /           \                                                 *
            /             \                                                *
            \             /                                                *
             \           /                                                 *
              \_________/ - 1                                              *
                                                                           *
                                                                           *
        y
        |__x
    */
    obb.lower = mliVec_set(-3., -3., -1.);
    obb.upper = mliVec_set(-0.5, 0.5, 1.);
    CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

    /*
                                                                           *
                                                                           *
               _________ + 1                                               *
              /  _____  \                                                  *
             /  | obb |  \                                                 *
            /   |     |   \                                                *
            \   |_____|   /                                                *
             \           /                                                 *
              \_________/ - 1                                              *
                                                                           *
                                                                           *
        y
        |__x
    */
    obb.lower = mliVec_set(-.5, -.5, -1.);
    obb.upper = mliVec_set(.5, .5, 1.);
    CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

    /*
                                                                           *
                                                                           *
               _________ + 1                                               *
              /         \         _____                                    *
             /           \       | obb |                                   *
            /             \      |     |                                   *
            \             /      |_____|                                   *
             \           /                                                 *
              \_________/ - 1                                              *
                                                                           *
                                                                           *
        y
        |__x
    */
    obb.lower = mliVec_set(6, -.5, -1.);
    obb.upper = mliVec_set(7, .5, 1.);
    CHECK(!mliHexagon_has_overlap_obb(hex, local2root_comp, obb));

    /*
                                                                           *
                                                                           *
                               _________ + 1                               *
                              /  _____  \                                  *
                             /  | obb |  \                                 *
                            /   |     |   \                                *
                            \   |_____|   /                                *
                             \           /                                 *
                              \_________/ - 1                              *
                                                                           *
                                                                           *
        y
        |__x
    */
    local2root_comp.trans = mliVec_set(6.5, 0., 0.);
    obb.lower = mliVec_set(6, -.5, -1.);
    obb.upper = mliVec_set(7, .5, 1.);
    CHECK(mliHexagon_has_overlap_obb(hex, local2root_comp, obb));
}
