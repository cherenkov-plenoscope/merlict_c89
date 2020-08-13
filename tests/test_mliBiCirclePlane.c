/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliBiCirclePlane_obb")
{
        struct mliOBB obb;
        struct mliBiCirclePlane plane;
        struct mliHomTraComp local2root_comp;
        plane.x_height = 1.;
        plane.y_width = 2.;

        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(0.));
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, +.5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, 1., 1e-6);
        CHECK_MARGIN(obb.lower.z, 0., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0., 1e-6);

        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(90.)); /* <-- rotation */
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -1., 1e-6);
        CHECK_MARGIN(obb.upper.x, 1., 1e-6);
        CHECK_MARGIN(obb.lower.y, -.5, 1e-6);
        CHECK_MARGIN(obb.upper.y, .5, 1e-6);
        CHECK_MARGIN(obb.lower.z, 0., 1e-6);
        CHECK_MARGIN(obb.upper.z, 0., 1e-6);

        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(1., 0., 0.), mli_deg2rad(45.));
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, .5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -sqrt(2.) / 2, 1e-6);
        CHECK_MARGIN(obb.upper.y, sqrt(2.) / 2, 1e-6);
        CHECK_MARGIN(obb.lower.z, -sqrt(2.) / 2, 1e-6);
        CHECK_MARGIN(obb.upper.z, sqrt(2.) / 2, 1e-6);

        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 1., 0.), mli_deg2rad(45.));
        obb = mliBiCirclePlane_obb(plane, local2root_comp);
        CHECK_MARGIN(obb.lower.x, -sqrt(2.) / 4, 1e-6);
        CHECK_MARGIN(obb.upper.x, sqrt(2.) / 4, 1e-6);
        CHECK_MARGIN(obb.lower.y, -1., 1e-6);
        CHECK_MARGIN(obb.upper.y, 1., 1e-6);
        CHECK_MARGIN(obb.lower.z, -sqrt(2.) / 4, 1e-6);
        CHECK_MARGIN(obb.upper.z, sqrt(2.) / 4, 1e-6);
}

CASE("mliBiCirclePlane_has_overlap_obb")
{
        struct mliOBB obb;
        struct mliBiCirclePlane plane;
        struct mliHomTraComp local2root_comp;
        plane.x_height = 1.;
        plane.y_width = 2.;
        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(0.));

        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., 1.);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));

        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(-0.51, 1., 1.);
        CHECK(!mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(-0.49, 1., 1.);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));

        obb.lower = mliVec_set(-1., -2., -1.);
        obb.upper = mliVec_set(1., -1.1, 1.);
        CHECK(!mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
        obb.lower = mliVec_set(-1., -2., -1.);
        obb.upper = mliVec_set(1., -0.9, 1.);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));

        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., -.01);
        CHECK(!mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
        obb.lower = mliVec_set(-1., -1., -1.);
        obb.upper = mliVec_set(1., 1., .01);
        CHECK(mliBiCirclePlane_has_overlap_obb(plane, local2root_comp, obb));
}

CASE("mliBiCirclePlane_intersection")
{
        struct mliBiCirclePlane plane;
        struct mliHomTraComp local2root_comp;
        struct mliIntersection intersection;
        plane.x_height = 1.;
        plane.y_width = 2.;
        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(0.));

        CHECK(mliBiCirclePlane_intersection(
                plane,
                local2root_comp,
                mliRay_set(mliVec_set(0., 0., -1.), mliVec_set(0., 0., 1.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

        CHECK(!mliBiCirclePlane_intersection(
                plane,
                local2root_comp,
                mliRay_set(mliVec_set(0., 3., -1.), mliVec_set(0., 0., 1.)),
                &intersection));
}
