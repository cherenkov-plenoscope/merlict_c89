/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliAccelerator, init")
{
        struct mliAccelerator accel = mliAccelerator_init();
        CHECK(accel.num_robjects == 0u);
}

CASE("mliAccelerator, init")
{
        struct mliScenery scenery = mliScenery_init();
        struct mliColor color;
        struct mliRay ray;
        struct mliPrng prng = mliPrng_init_MT19937(0);
        struct mliTracerCongig tracer_config = mliTracerCongig_init();

        CHECK(mliScenery_malloc_from_tar(
                &scenery,
                "merlict_c89/"
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));

        ray = mliRay_set(mliVec_set(0.0, 0.0, -5.0), mliVec_set(0.0, 0.0, 1.0));

        color = mli_trace(&scenery, ray, &tracer_config, &prng);

        CHECK_MARGIN(color.r, 11.0, 1.0);
        CHECK_MARGIN(color.g, 45.5, 1.0);
        CHECK_MARGIN(color.b, 74.5, 1.0);

        mliScenery_free(&scenery);
}
