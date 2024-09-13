/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliAccelerator.h"

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
        struct mliTracerConfig tracer_config = mliTracerConfig_init();
        struct mliColorObserver color_observer = mliColorObserver_init();
        struct mliColorMaterials color_materials = mliColorMaterials_init();
        struct mliTracer tracer = mliTracer_init();

        CHECK(mliScenery_malloc_from_path_tar(
                &scenery,
                "libs/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));

        CHECK(mliColorObserver_malloc_cie1931(&color_observer));
        CHECK(mliColorMaterials_malloc_from_Materials(
                &color_materials, &scenery.materials, &color_observer));

        tracer.scenery = &scenery;
        tracer.scenery_color_materials = &color_materials;
        tracer.config = &tracer_config;

        ray = mliRay_set(
                mliVec_init(0.0, 0.0, -5.0), mliVec_init(0.0, 0.0, 1.0));

        color = mliTracer_trace_ray(&tracer, ray, &prng);

        CHECK_MARGIN(color.r, 11.0, 1.0);
        CHECK_MARGIN(color.g, 45.5, 1.0);
        CHECK_MARGIN(color.b, 74.5, 1.0);

        mliColorObserver_free(&color_observer);
        mliColorMaterials_free(&color_materials);
        mliScenery_free(&scenery);
}
