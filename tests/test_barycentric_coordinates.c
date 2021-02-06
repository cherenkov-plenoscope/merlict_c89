/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("barycentric coordinates on vertices")
{
        const struct mliVec a = {-1.0, 0.0, 0.0};
        const struct mliVec b = {1.0, 0.0, 0.0};
        const struct mliVec c = {0.0, 2.0, 0.0};
        struct mliVec test_point;
        struct mliBarycentrigWeights weights;

        test_point = a;
        weights = mli_barycentric_weights(a, b, c, test_point);
        CHECK(weights.a == 1.0);
        CHECK(weights.b == 0.0);
        CHECK(weights.c == 0.0);

        test_point = b;
        weights = mli_barycentric_weights(a, b, c, test_point);
        CHECK(weights.a == 0.0);
        CHECK(weights.b == 1.0);
        CHECK(weights.c == 0.0);

        test_point = c;
        weights = mli_barycentric_weights(a, b, c, test_point);
        CHECK(weights.a == 0.0);
        CHECK(weights.b == 0.0);
        CHECK(weights.c == 1.0);
}

CASE("barycentric coordinates in center")
{
        const double phi = 2.0 * MLI_PI / 3.0;
        const struct mliVec a = mliVec_set(cos(0.0 * phi), sin(0.0 * phi), 0.0);
        const struct mliVec b = mliVec_set(cos(1.0 * phi), sin(1.0 * phi), 0.0);
        const struct mliVec c = mliVec_set(cos(2.0 * phi), sin(2.0 * phi), 0.0);
        struct mliBarycentrigWeights weights;

        weights = mli_barycentric_weights(a, b, c, mliVec_set(0.0, 0.0, 0.0));
        CHECK_MARGIN(weights.a, 1.0 / 3.0, 1e-6);
        CHECK_MARGIN(weights.b, 1.0 / 3.0, 1e-6);
        CHECK_MARGIN(weights.c, 1.0 / 3.0, 1e-6);
}

CASE("barycentric coordinates out of triangle")
{
        const double phi = 2.0 * MLI_PI / 3.0;
        const struct mliVec a = mliVec_set(cos(0.0 * phi), sin(0.0 * phi), 0.0);
        const struct mliVec b = mliVec_set(cos(1.0 * phi), sin(1.0 * phi), 0.0);
        const struct mliVec c = mliVec_set(cos(2.0 * phi), sin(2.0 * phi), 0.0);
        struct mliBarycentrigWeights weights;

        weights = mli_barycentric_weights(a, b, c, mliVec_multiply(a, 1.1));
        CHECK(weights.a > 0.0);
        CHECK(weights.b < 0.0);
        CHECK(weights.c < 0.0);

        weights = mli_barycentric_weights(a, b, c, mliVec_multiply(b, 1.1));
        CHECK(weights.a < 0.0);
        CHECK(weights.b > 0.0);
        CHECK(weights.c < 0.0);

        weights = mli_barycentric_weights(a, b, c, mliVec_multiply(c, 1.1));
        CHECK(weights.a < 0.0);
        CHECK(weights.b < 0.0);
        CHECK(weights.c > 0.0);
}

CASE("estimate surface normal")
{
        const double phi = 2.0 * MLI_PI / 3.0;
        const struct mliVec a = mliVec_set(cos(0.0 * phi), sin(0.0 * phi), 0.0);
        const struct mliVec b = mliVec_set(cos(1.0 * phi), sin(1.0 * phi), 0.0);
        const struct mliVec c = mliVec_set(cos(2.0 * phi), sin(2.0 * phi), 0.0);
        const struct mliVec unit_z = mliVec_set(0.0, 0.0, 1.0);
        const struct mliVec negative_unit_z = mliVec_set(0.0, 0.0, -1.0);
        struct mliBarycentrigWeights weights;
        struct mliVec normal;

        weights = mli_barycentric_weights(a, b, c, mliVec_set(0.0, 0.0, 0.0));
        normal = mliTriangle_interpolate_surface_normal(unit_z, unit_z, unit_z, weights);
        CHECK(normal.x == unit_z.x);
        CHECK(normal.y == unit_z.y);
        CHECK(normal.z == unit_z.z);

        weights = mli_barycentric_weights(a, b, c, mliVec_set(0.0, 0.0, 0.0));
        normal = mliTriangle_interpolate_surface_normal(a, b, c, weights);
        CHECK_MARGIN(normal.x, 0.0, 1e-6);
        CHECK_MARGIN(normal.y, 0.0, 1e-6);
        CHECK_MARGIN(normal.z, 0.0, 1e-6);

        weights = mli_barycentric_weights(a, b, c, c);
        normal = mliTriangle_interpolate_surface_normal(
                unit_z,
                unit_z,
                negative_unit_z,
                weights);
        CHECK_MARGIN(normal.x, negative_unit_z.x, 1e-6);
        CHECK_MARGIN(normal.y, negative_unit_z.y, 1e-6);
        CHECK_MARGIN(normal.z, negative_unit_z.z, 1e-6);

        weights = mli_barycentric_weights(a, b, c, a);
        normal = mliTriangle_interpolate_surface_normal(
                unit_z,
                unit_z,
                negative_unit_z,
                weights);
        CHECK_MARGIN(normal.x, unit_z.x, 1e-6);
        CHECK_MARGIN(normal.y, unit_z.y, 1e-6);
        CHECK_MARGIN(normal.z, unit_z.z, 1e-6);
}
