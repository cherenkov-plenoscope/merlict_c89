/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("struct, constructor")
{
        struct mli_Vec a = {1., 2., 3.};
        CHECK_MARGIN(a.x, 1.0, 1e-6);
        CHECK_MARGIN(a.y, 2.0, 1e-6);
        CHECK_MARGIN(a.z, 3.0, 1e-6);
}

CASE("add")
{
        struct mli_Vec a = {1., 2., 3.};
        struct mli_Vec b = {4., 5., 6.};
        struct mli_Vec out = mli_Vec_add(a, b);
        CHECK_MARGIN(out.x, 5.0, 1e-6);
        CHECK_MARGIN(out.y, 7.0, 1e-6);
        CHECK_MARGIN(out.z, 9.0, 1e-6);
}

CASE("substract")
{
        struct mli_Vec a = {1., 2., 3.};
        struct mli_Vec b = {4., 6., 8.};
        struct mli_Vec out = mli_Vec_substract(a, b);
        CHECK_MARGIN(out.x, -3.0, 1e-6);
        CHECK_MARGIN(out.y, -4.0, 1e-6);
        CHECK_MARGIN(out.z, -5.0, 1e-6);
}

CASE("dot-product")
{
        struct mli_Vec a = {1., 2., 3.};
        struct mli_Vec b = {4., 5., 6.};
        CHECK_MARGIN(mli_Vec_dot(a, b), 32.0, 1e-6);
}

CASE("dot-product, perpendicular")
{
        struct mli_Vec a = {0., 0., 1.};
        struct mli_Vec b = {0., 1., 0.};
        CHECK_MARGIN(mli_Vec_dot(a, b), 0., 1e-6);
}

CASE("dot-product, perpendicular, 2")
{
        struct mli_Vec a = {0., 0., 1.};
        struct mli_Vec b = {1., 0., 0.};
        CHECK_MARGIN(mli_Vec_dot(a, b), 0., 1e-6);
}

CASE("dot-product, parallel")
{
        struct mli_Vec a = {0., 0., 1.};
        struct mli_Vec b = {0., 0., 1.};
        CHECK_MARGIN(mli_Vec_dot(a, b), 1., 1e-6);
}

CASE("cross-product, perpendicular")
{
        struct mli_Vec a = {1., 0., 0.};
        struct mli_Vec b = {0., 1., 0.};
        struct mli_Vec out = mli_Vec_cross(a, b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
}

CASE("cross-product, perpendicular, 2")
{
        struct mli_Vec a = {1., 0., 0.};
        struct mli_Vec b = {0., 0., 1.};
        struct mli_Vec out = mli_Vec_cross(a, b);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, -1., 1e-6);
        CHECK_MARGIN(out.z, 0., 1e-6);
}

CASE("norm, only x")
{
        struct mli_Vec a = {1., 0., 0.};
        CHECK_MARGIN(mli_Vec_norm(a), 1., 1e-6);
}

CASE("norm, x, and y")
{
        struct mli_Vec a = {3., 4., 0.};
        CHECK_MARGIN(mli_Vec_norm(a), 5., 1e-6);
}

CASE("norm, negative z")
{
        struct mli_Vec a = {0., 0., -1.};
        CHECK_MARGIN(mli_Vec_norm(a), 1., 1e-6);
}

CASE("mirror")
{
        struct mli_Vec in = {0., 0., -1.};
        struct mli_Vec surface_normal = {0., 0., 1.};
        struct mli_Vec out = mli_Vec_mirror(in, surface_normal);
        CHECK_MARGIN(out.x, 0., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
}

CASE("mirror, 2")
{
        struct mli_Vec in = {1., 0., -1.};
        struct mli_Vec surface_normal = {0., 0., 1.};
        struct mli_Vec out = mli_Vec_mirror(in, surface_normal);
        CHECK_MARGIN(out.x, 1., 1e-6);
        CHECK_MARGIN(out.y, 0., 1e-6);
        CHECK_MARGIN(out.z, 1., 1e-6);
}

CASE("equal_margin, null-vectors")
{
        struct mli_Vec a = {0., 0., 0.};
        struct mli_Vec b = {0., 0., 0.};
        CHECK(mli_Vec_equal_margin(a, b, 1e-6));
}

CASE("equal_margin, expect false")
{
        struct mli_Vec a = {1., 0., 0.};
        struct mli_Vec b = {1. + 1e-5, 0., 0.};
        CHECK(!mli_Vec_equal_margin(a, b, 1e-6));
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
        struct mli_Vec a;

        const double p = +1.;
        const double n = -1.;

        a.x = n;
        a.y = n;
        a.z = n;
        CHECK(mli_Vec_octant(a) == 0u);

        a.x = n;
        a.y = n;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 1u);

        a.x = n;
        a.y = p;
        a.z = n;
        CHECK(mli_Vec_octant(a) == 2u);

        a.x = n;
        a.y = p;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 3u);

        a.x = p;
        a.y = n;
        a.z = n;
        CHECK(mli_Vec_octant(a) == 4u);

        a.x = p;
        a.y = n;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 5u);

        a.x = p;
        a.y = p;
        a.z = n;
        CHECK(mli_Vec_octant(a) == 6u);

        a.x = p;
        a.y = p;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 7u);

        a.x = 0.;
        a.y = 0.;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 7u);

        a.x = 0.;
        a.y = p;
        a.z = 0.;
        CHECK(mli_Vec_octant(a) == 7u);

        a.x = p;
        a.y = 0.;
        a.z = 0.;
        CHECK(mli_Vec_octant(a) == 7u);

        a.x = 0.;
        a.y = p;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 7u);

        a.x = p;
        a.y = p;
        a.z = 0.;
        CHECK(mli_Vec_octant(a) == 7u);

        a.x = p;
        a.y = 0.;
        a.z = p;
        CHECK(mli_Vec_octant(a) == 7u);
}

CASE("ncpy")
{
        int i;
        struct mli_Vec a[3];
        struct mli_Vec b[3];
        a[0] = mli_Vec_init(0., 1., 2.);
        a[1] = mli_Vec_init(3., 4., 5.);
        a[2] = mli_Vec_init(6., 7., 8.);

        MLI_MATH_NCPY(a, b, 3); /* <--- to be tested */

        for (i = 0; i < 3; i++) {
                CHECK(a[i].x == b[i].x);
                CHECK(a[i].y == b[i].y);
                CHECK(a[i].z == b[i].z);
        }
}

CASE("mean")
{
        int i;
        struct mli_Vec a[10];
        struct mli_Vec m = {0.0, 0.0, 0.0};
        for (i = 0; i < 10; i++) {
                double j = (double)i;
                a[i] = mli_Vec_init(j, -j, j * j);
        }
        m = mli_Vec_mean(a, 10);
        CHECK_MARGIN(m.x, 4.5, MLI_MATH_EPSILON);
        CHECK_MARGIN(m.y, -4.5, MLI_MATH_EPSILON);
        CHECK_MARGIN(m.z, 28.5, MLI_MATH_EPSILON);
}

CASE("mean_of_zero_samples")
{
        struct mli_Vec a[1];
        struct mli_Vec m = {0.0, 0.0, 0.0};
        m = mli_Vec_mean(a, 0);
        CHECK(MLI_MATH_IS_NAN(m.x));
        CHECK(MLI_MATH_IS_NAN(m.y));
        CHECK(MLI_MATH_IS_NAN(m.z));
}
