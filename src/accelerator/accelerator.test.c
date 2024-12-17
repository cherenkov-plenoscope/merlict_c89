/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Accelerator, init")
{
        struct mli_Accelerator accel = mli_Accelerator_init();
        CHECK(accel.num_robjects == 0u);
}

CASE("mli_Accelerator, init")
{
        struct mliScenery scenery = mliScenery_init();
        struct mli_Color color;
        struct mli_Ray ray;
        struct mli_Prng prng = mli_Prng_init_MT19937(0);
        struct mliTracerConfig tracer_config = mliTracerConfig_init();
        struct mli_ColorObserver color_observer = mli_ColorObserver_init();
        struct mli_ColorMaterials color_materials = mli_ColorMaterials_init();
        struct mliTracer tracer = mliTracer_init();

        chk_dbg CHECK(mliScenery_malloc_from_path_tar(
                &scenery,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));
        chk_dbg

                CHECK(mli_ColorObserver_malloc_cie1931(&color_observer));
        CHECK(mli_ColorMaterials_malloc_from_Materials(
                &color_materials, &scenery.materials, &color_observer));

        tracer.scenery = &scenery;
        tracer.scenery_color_materials = &color_materials;
        tracer.config = &tracer_config;

        ray = mli_Ray_set(
                mli_Vec_init(0.0, 0.0, -5.0), mli_Vec_init(0.0, 0.0, 1.0));

        color = mliTracer_trace_ray(&tracer, ray, &prng);

        CHECK_MARGIN(color.r, 1.28e-2, 1e-3);
        CHECK_MARGIN(color.g, 1.55e-2, 1e-3);
        CHECK_MARGIN(color.b, 2.48e-2, 1e-3);

        mli_ColorObserver_free(&color_observer);
        mli_ColorMaterials_free(&color_materials);
        mliScenery_free(&scenery);
}
