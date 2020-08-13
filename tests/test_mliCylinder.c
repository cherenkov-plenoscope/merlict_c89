/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliCylinder_is_equal")
{
        struct mliCylinder a, b;
        a.radius = 1.;
        a.length = 2.;
        b.radius = 1.;
        b.length = 2.;
        CHECK(mliCylinder_is_equal(a, b));
        b.length = .5;
        CHECK(!mliCylinder_is_equal(a, b));
}

CASE("mliCylinder_start_local, mliCylinder_end_local")
{
        struct mliCylinder a;
        struct mliVec start_local;
        struct mliVec end_local;
        a.radius = .1;
        a.length = .5;
        start_local = mliCylinder_start_local(a);
        CHECK_MARGIN(start_local.x, 0., 1e-6);
        CHECK_MARGIN(start_local.y, 0., 1e-6);
        CHECK_MARGIN(start_local.z, -a.length / 2, 1e-6);
        end_local = mliCylinder_end_local(a);
        CHECK_MARGIN(end_local.x, 0., 1e-6);
        CHECK_MARGIN(end_local.y, 0., 1e-6);
        CHECK_MARGIN(end_local.z, a.length / 2, 1e-6);
}

CASE("slim cylinder obb")
{
        struct mliCylinder a;
        struct mliOBB obb;
        struct mliHomTraComp trafo;
        a.radius = .5;
        a.length = 12.;
        trafo.translation = mliVec_set(0., 0., 0.);
        trafo.rotation =
                mliQuaternion_set_rotaxis_and_angle(mliVec_set(0., 0., 1.), 0.);
        obb = mliCylinder_obb(a, trafo);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -.5, 1e-6);
        CHECK_MARGIN(obb.lower.z, -6.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, .5, 1e-6);
        CHECK_MARGIN(obb.upper.y, .5, 1e-6);
        CHECK_MARGIN(obb.upper.z, 6.5, 1e-6);
}

CASE("fat cylinder obb")
{
        struct mliCylinder a;
        struct mliOBB obb;
        struct mliHomTraComp trafo;
        a.radius = 5;
        a.length = 1.;
        trafo.translation = mliVec_set(0., 0., 0.);
        trafo.rotation =
                mliQuaternion_set_rotaxis_and_angle(mliVec_set(0., 0., 1.), 0.);
        obb = mliCylinder_obb(a, trafo);
        CHECK_MARGIN(obb.lower.x, -5., 1e-6);
        CHECK_MARGIN(obb.lower.y, -5., 1e-6);
        CHECK_MARGIN(obb.lower.z, -5.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, 5., 1e-6);
        CHECK_MARGIN(obb.upper.y, 5., 1e-6);
        CHECK_MARGIN(obb.upper.z, 5.5, 1e-6);
}

CASE("slim cylinder obb transformed")
{
        struct mliCylinder a;
        struct mliOBB obb;
        struct mliHomTraComp trafo;
        a.radius = .5;
        a.length = 12.;
        trafo.translation = mliVec_set(0., 0., 1.);
        trafo.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(1., 0., 0.), mli_deg2rad(90));
        obb = mliCylinder_obb(a, trafo);
        CHECK_MARGIN(obb.lower.x, -.5, 1e-6);
        CHECK_MARGIN(obb.lower.y, -6.5, 1e-6);
        CHECK_MARGIN(obb.lower.z, 0.5, 1e-6);
        CHECK_MARGIN(obb.upper.x, .5, 1e-6);
        CHECK_MARGIN(obb.upper.y, 6.5, 1e-6);
        CHECK_MARGIN(obb.upper.z, 1.5, 1e-6);
}

CASE("cylinder overlap with its own obb")
{
        struct mliCylinder a;
        struct mliOBB obb;
        struct mliHomTraComp local2root_comp;
        a.radius = .5;
        a.length = 12.;
        local2root_comp.translation = mliVec_set(0., 0., 1.);
        local2root_comp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(1., 0., 0.), mli_deg2rad(90));
        obb = mliCylinder_obb(a, local2root_comp);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));
}

CASE("cylinder overlap with its own obb, 2")
{
        struct mliCylinder a;
        struct mliOBB obb;
        struct mliHomTraComp local2root_comp;
        a.radius = .5;
        a.length = 12.;
        local2root_comp.translation = mliVec_set(0., 0., 1.);
        local2root_comp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(1., 0., 0.), mli_deg2rad(90));

        /* lower x */
        obb.lower = mliVec_set(-10, -10, -10);
        obb.upper = mliVec_set(-.51, 10, 10);
        CHECK(!mliCylinder_has_overlap_obb(a, local2root_comp, obb));
        obb.upper = mliVec_set(-0.49, 10, 10);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));

        /* upper x */
        obb.lower = mliVec_set(.51, -10, -10);
        obb.upper = mliVec_set(10, 10, 10);
        CHECK(!mliCylinder_has_overlap_obb(a, local2root_comp, obb));
        obb.lower = mliVec_set(.49, -10, -10);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));

        /* lower y */
        obb.lower = mliVec_set(-10, -10, -10);
        obb.upper = mliVec_set(10, -6.51, 10);
        CHECK(!mliCylinder_has_overlap_obb(a, local2root_comp, obb));
        obb.upper = mliVec_set(10, -6.49, 10);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));

        /* upper y */
        obb.lower = mliVec_set(-10, 6.51, -10);
        obb.upper = mliVec_set(10, 10, 10);
        CHECK(!mliCylinder_has_overlap_obb(a, local2root_comp, obb));
        obb.lower = mliVec_set(-10, 6.49, -10);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));

        /* lower z */
        obb.lower = mliVec_set(-10, -10, -10);
        obb.upper = mliVec_set(10, 10, .49);
        CHECK(!mliCylinder_has_overlap_obb(a, local2root_comp, obb));
        obb.upper = mliVec_set(10, 10, .51);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));

        /* upper z */
        obb.lower = mliVec_set(-10, -10, 1.51);
        obb.upper = mliVec_set(10, 10, 10);
        CHECK(!mliCylinder_has_overlap_obb(a, local2root_comp, obb));
        obb.lower = mliVec_set(-10, -10, 1.49);
        CHECK(mliCylinder_has_overlap_obb(a, local2root_comp, obb));
}

CASE("cylinder intersection")
{
        double plus_solution, minus_solution;
        /* ray starts infron of cylinder */
        CHECK(mli_cylinder_equation(
                1.,
                mliRay_set(mliVec_set(-5., 0., 0.), mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(minus_solution, 4., 1e-6);
        CHECK_MARGIN(plus_solution, 6., 1e-6);

        /* ray starts inside cylinder */
        CHECK(mli_cylinder_equation(
                1.,
                mliRay_set(mliVec_set(0., 0., 0.), mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(minus_solution, -1., 1e-6);
        CHECK_MARGIN(plus_solution, +1., 1e-6);

        /* ray starts behind cylinder */
        CHECK(mli_cylinder_equation(
                1.,
                mliRay_set(mliVec_set(5., 0., 0.), mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));
        CHECK_MARGIN(minus_solution, -6., 1e-6);
        CHECK_MARGIN(plus_solution, -4., 1e-6);

        /* ray runs beside cylinder */
        CHECK(!mli_cylinder_equation(
                1.,
                mliRay_set(mliVec_set(-5., 5., 0.), mliVec_set(1., 0., 0.)),
                &plus_solution,
                &minus_solution));

        /* ray runs inside and parallel to cylinder */
        CHECK(!mli_cylinder_equation(
                1.,
                mliRay_set(mliVec_set(0., 0., 0.), mliVec_set(0., 0., 1.)),
                &plus_solution,
                &minus_solution));
}

CASE("cylinder intersection, 2")
{
        struct mliCylinder cylinder;
        struct mliHomTraComp local2root_comp;
        struct mliIntersection intersection;
        local2root_comp.translation = mliVec_set(0., 0., 0.);
        local2root_comp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(0.));

        cylinder.radius = 1.;
        cylinder.length = 2.;
        CHECK(mliCylinder_intersection(
                cylinder,
                local2root_comp,
                mliRay_set(mliVec_set(0., 0., 0.), mliVec_set(1., 0., 0.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 1., 1e-6);

        CHECK(!mliCylinder_intersection(
                cylinder,
                local2root_comp,
                mliRay_set(mliVec_set(1., 0., 0.), mliVec_set(1., 0., 0.)),
                &intersection));

        CHECK(mliCylinder_intersection(
                cylinder,
                local2root_comp,
                mliRay_set(mliVec_set(1., 0., 0.), mliVec_set(-1., 0., 0.)),
                &intersection));
        CHECK_MARGIN(intersection.distance_of_ray, 2., 1e-6);
}
