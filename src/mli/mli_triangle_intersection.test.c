/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("intersection")
{
        const struct mliVec a = {-1.0, 0.0, 0.0};
        const struct mliVec b = {1.0, 0.0, 0.0};
        const struct mliVec c = {0.0, 2.0, 0.0};
        double param;

        CHECK(mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(0.0, 0.0, 1.0),
                        mliVec_init(0.0, 0.0, -1.0)),
                a,
                b,
                c,
                &param));
        CHECK_MARGIN(param, 1.0, 1e-6);

        CHECK(mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(-0.99, 0.01, 1.0),
                        mliVec_init(0.0, 0.0, -1.0)),
                a,
                b,
                c,
                &param));
        CHECK_MARGIN(param, 1.0, 1e-6);

        CHECK(!mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(-1.01, 0.01, 1.0),
                        mliVec_init(0.0, 0.0, -1.0)),
                a,
                b,
                c,
                &param));

        CHECK(mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(0.0, 1.99, 1.0),
                        mliVec_init(0.0, 0.0, -1.0)),
                a,
                b,
                c,
                &param));
        CHECK_MARGIN(param, 1.0, 1e-6);

        CHECK(!mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(0.0, 2.01, 1.0),
                        mliVec_init(0.0, 0.0, -1.0)),
                a,
                b,
                c,
                &param));
}

CASE("ray parameter")
{
        int64_t i;
        const struct mliVec a = {-1.0, 0.0, 0.0};
        const struct mliVec b = {1.0, 0.0, 0.0};
        const struct mliVec c = {0.0, 2.0, 0.0};
        double param;

        for (i = -100; i < 0; i++) {
                const double float_i = (double)i;

                CHECK(mliRay_intersects_triangle(
                        mliRay_set(
                                mliVec_init(0.0, 1.0, float_i),
                                mliVec_init(0.0, 0.0, 1.0)),
                        a,
                        b,
                        c,
                        &param));
                CHECK_MARGIN(param, -float_i, 1e-6);
        }
}

CASE("causality")
{
        const struct mliVec a = {-1.0, 0.0, 0.0};
        const struct mliVec b = {1.0, 0.0, 0.0};
        const struct mliVec c = {0.0, 2.0, 0.0};
        double param;

        double z = 1.0;
        CHECK(!mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(0.0, 1.0, z), mliVec_init(0.0, 0.0, 1.0)),
                a,
                b,
                c,
                &param));

        z = -1.0;
        CHECK(mliRay_intersects_triangle(
                mliRay_set(
                        mliVec_init(0.0, 1.0, z), mliVec_init(0.0, 0.0, 1.0)),
                a,
                b,
                c,
                &param));
        CHECK_MARGIN(param, 1.0, 1e-6);
}
