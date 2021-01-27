/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("barycentric coordinates on vertices")
{
        const struct mliVec a = {-1.0, 0.0, 0.0};
        const struct mliVec b = {1.0, 0.0, 0.0};
        const struct mliVec c = {0.0, 2.0, 0.0};
        struct mliVec test_point;
        struct mliVec bary;

        test_point = a;
        bary = mli_barycentric_weights(a, b, c, test_point);
        CHECK(bary.x == 1.0);
        CHECK(bary.y == 0.0);
        CHECK(bary.z == 0.0);

        test_point = b;
        bary = mli_barycentric_weights(a, b, c, test_point);
        CHECK(bary.x == 0.0);
        CHECK(bary.y == 1.0);
        CHECK(bary.z == 0.0);

        test_point = c;
        bary = mli_barycentric_weights(a, b, c, test_point);
        CHECK(bary.x == 0.0);
        CHECK(bary.y == 0.0);
        CHECK(bary.z == 1.0);
}

CASE("barycentric coordinates in center")
{
        const double phi = 2.0 * MLI_PI / 3.0;
        const struct mliVec a = mliVec_set(cos(0.0 * phi), sin(0.0 * phi), 0.0);
        const struct mliVec b = mliVec_set(cos(1.0 * phi), sin(1.0 * phi), 0.0);
        const struct mliVec c = mliVec_set(cos(2.0 * phi), sin(2.0 * phi), 0.0);
        struct mliVec bary;

        bary = mli_barycentric_weights(a, b, c, mliVec_set(0.0, 0.0, 0.0));
        CHECK_MARGIN(bary.x, 1.0 / 3.0, 1e-6);
        CHECK_MARGIN(bary.y, 1.0 / 3.0, 1e-6);
        CHECK_MARGIN(bary.z, 1.0 / 3.0, 1e-6);
}

CASE("barycentric coordinates out of triangle")
{
        const double phi = 2.0 * MLI_PI / 3.0;
        const struct mliVec a = mliVec_set(cos(0.0 * phi), sin(0.0 * phi), 0.0);
        const struct mliVec b = mliVec_set(cos(1.0 * phi), sin(1.0 * phi), 0.0);
        const struct mliVec c = mliVec_set(cos(2.0 * phi), sin(2.0 * phi), 0.0);
        struct mliVec bary;

        bary = mli_barycentric_weights(a, b, c, mliVec_multiply(a, 1.1));
        CHECK(bary.x > 0.0);
        CHECK(bary.y < 0.0);
        CHECK(bary.z < 0.0);

        bary = mli_barycentric_weights(a, b, c, mliVec_multiply(b, 1.1));
        CHECK(bary.x < 0.0);
        CHECK(bary.y > 0.0);
        CHECK(bary.z < 0.0);

        bary = mli_barycentric_weights(a, b, c, mliVec_multiply(c, 1.1));
        CHECK(bary.x < 0.0);
        CHECK(bary.y < 0.0);
        CHECK(bary.z > 0.0);
}
