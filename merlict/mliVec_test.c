/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliVec.h"

CASE("struct, constructor")
{
        struct mliVec a = {1., 2., 3.};
        CHECK_MARGIN(a.x, 1.0, 1e-6);
        CHECK_MARGIN(a.y, 2.0, 1e-6);
        CHECK_MARGIN(a.z, 3.0, 1e-6);
}

CASE("add")
{
        struct mliVec a = {1., 2., 3.};
        struct mliVec b = {4., 5., 6.};
        struct mliVec out = mliVec_add(a, b);
        CHECK_MARGIN(out.x, 5.0, 1e-6);
        CHECK_MARGIN(out.y, 7.0, 1e-6);
        CHECK_MARGIN(out.z, 9.0, 1e-6);
}

CASE("substract")
{
        struct mliVec a = {1., 2., 3.};
        struct mliVec b = {4., 6., 8.};
        struct mliVec out = mliVec_substract(a, b);
        CHECK_MARGIN(out.x, -3.0, 1e-6);
        CHECK_MARGIN(out.y, -4.0, 1e-6);
        CHECK_MARGIN(out.z, -5.0, 1e-6);
}

CASE("dot-product")
{
        struct mliVec a = {1., 2., 3.};
        struct mliVec b = {4., 5., 6.};
        CHECK_MARGIN(mliVec_dot(a, b), 32.0, 1e-6);
}

CASE("dot-product, perpendicular")
{
        struct mliVec a = {0., 0., 1.};
        struct mliVec b = {0., 1., 0.};
        CHECK_MARGIN(mliVec_dot(a, b), 0., 1e-6);
}

CASE("dot-product, perpendicular, 2")
{
        struct mliVec a = {0., 0., 1.};
        struct mliVec b = {1., 0., 0.};
        CHECK_MARGIN(mliVec_dot(a, b), 0., 1e-6);
}

CASE("dot-product, parallel")
{
        struct mliVec a = {0., 0., 1.};
        struct mliVec b = {0., 0., 1.};
        CHECK_MARGIN(mliVec_dot(a, b), 1., 1e-6);
}

CASE("cross-product, perpendicular")
{
        struct mliVec a = {1., 0., 0.};
        struct mliVec b = {0., 1., 0.};
        struct mliVec out = mliVec_cross(a, b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
}

CASE("cross-product, perpendicular, 2")
{
        struct mliVec a = {1., 0., 0.};
        struct mliVec b = {0., 0., 1.};
        struct mliVec out = mliVec_cross(a, b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, -1., 1e-6);
        CHECK_MARGIN(out.z, 0., 1e-6);
}

CASE("norm, only x")
{
        struct mliVec a = {1., 0., 0.};
        CHECK_MARGIN(mliVec_norm(a), 1., 1e-6);
}

CASE("norm, x, and y")
{
        struct mliVec a = {3., 4., 0.};
        CHECK_MARGIN(mliVec_norm(a), 5., 1e-6);
}

CASE("norm, negative z")
{
        struct mliVec a = {0., 0., -1.};
        CHECK_MARGIN(mliVec_norm(a), 1., 1e-6);
}

CASE("mirror")
{
        struct mliVec in = {0., 0., -1.};
        struct mliVec surface_normal = {0., 0., 1.};
        struct mliVec out = mliVec_mirror(in, surface_normal);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
}

CASE("mirror, 2")
{
        struct mliVec in = {1., 0., -1.};
        struct mliVec surface_normal = {0., 0., 1.};
        struct mliVec out = mliVec_mirror(in, surface_normal);
        CHECK_MARGIN(out.x, 1., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
}

CASE("equal_margin, null-vectors")
{
        struct mliVec a = {0., 0., 0.};
        struct mliVec b = {0., 0., 0.};
        CHECK(mliVec_equal_margin(a, b, 1e-6));
}

CASE("equal_margin, expect false")
{
        struct mliVec a = {1., 0., 0.};
        struct mliVec b = {1. + 1e-5, 0., 0.};
        CHECK(!mliVec_equal_margin(a, b, 1e-6));
}

CASE("octant")
{
        /*
            x y z octant
            - - -   0
            - - +   1
            - + -   2
            - + +   3
            + - -   4
            + - +   5
            + + -   6
            + + +   7
        */
        struct mliVec a;

        const double p = +1.;
        const double n = -1.;

        a.x = n;
        a.y = n;
        a.z = n;
        CHECK(mliVec_octant(a) == 0u);

        a.x = n;
        a.y = n;
        a.z = p;
        CHECK(mliVec_octant(a) == 1u);

        a.x = n;
        a.y = p;
        a.z = n;
        CHECK(mliVec_octant(a) == 2u);

        a.x = n;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(a) == 3u);

        a.x = p;
        a.y = n;
        a.z = n;
        CHECK(mliVec_octant(a) == 4u);

        a.x = p;
        a.y = n;
        a.z = p;
        CHECK(mliVec_octant(a) == 5u);

        a.x = p;
        a.y = p;
        a.z = n;
        CHECK(mliVec_octant(a) == 6u);

        a.x = p;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(a) == 7u);

        a.x = 0.;
        a.y = 0.;
        a.z = p;
        CHECK(mliVec_octant(a) == 7u);

        a.x = 0.;
        a.y = p;
        a.z = 0.;
        CHECK(mliVec_octant(a) == 7u);

        a.x = p;
        a.y = 0.;
        a.z = 0.;
        CHECK(mliVec_octant(a) == 7u);

        a.x = 0.;
        a.y = p;
        a.z = p;
        CHECK(mliVec_octant(a) == 7u);

        a.x = p;
        a.y = p;
        a.z = 0.;
        CHECK(mliVec_octant(a) == 7u);

        a.x = p;
        a.y = 0.;
        a.z = p;
        CHECK(mliVec_octant(a) == 7u);
}

CASE("ncpy")
{
        int i;
        struct mliVec a[3];
        struct mliVec b[3];
        a[0] = mliVec_set(0., 1., 2.);
        a[1] = mliVec_set(3., 4., 5.);
        a[2] = mliVec_set(6., 7., 8.);

        MLI_NCPY(a, b, 3); /* <--- to be tested */

        for (i = 0; i < 3; i++) {
                CHECK(a[i].x == b[i].x);
                CHECK(a[i].y == b[i].y);
                CHECK(a[i].z == b[i].z);
        }
}

CASE("mean")
{
        int i;
        struct mliVec a[10];
        struct mliVec m = {0.0, 0.0, 0.0};
        for (i = 0; i < 10; i++) {
                double j = (double)i;
                a[i] = mliVec_set(j, -j, j * j);
        }
        m = mliVec_mean(a, 10);
        CHECK_MARGIN(m.x, 4.5, MLI_EPSILON);
        CHECK_MARGIN(m.y, -4.5, MLI_EPSILON);
        CHECK_MARGIN(m.z, 28.5, MLI_EPSILON);
}

CASE("mean_of_zero_samples")
{
        struct mliVec a[1];
        struct mliVec m = {0.0, 0.0, 0.0};
        m = mliVec_mean(a, 0);
        CHECK(MLI_IS_NAN(m.x));
        CHECK(MLI_IS_NAN(m.y));
        CHECK(MLI_IS_NAN(m.z));
}
