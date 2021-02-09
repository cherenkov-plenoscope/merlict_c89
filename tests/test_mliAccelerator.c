/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliAccelerator, init")
{
        struct mliAccelerator accel = mliAccelerator_init();
        CHECK(accel.num_robjects == 0u);
}

CASE("mliAccelerator, init")
{
        struct mliScenery combine = mliScenery_init();
        struct mliColor color;
        struct mliRay ray;

        CHECK(mliScenery_malloc_from_tar(
                &combine,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        ray = mliRay_set(mliVec_set(0.0, 0.0, -5.0), mliVec_set(0.0, 0.0, 1.0));

        color = mli_trace(&combine, ray);

        CHECK_MARGIN(color.r, 11.0, 1.0);
        CHECK_MARGIN(color.g, 45.5, 1.0);
        CHECK_MARGIN(color.b, 74.5, 1.0);

        mliScenery_free(&combine);
}
