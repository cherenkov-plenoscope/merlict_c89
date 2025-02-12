/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Accelerator, init")
{
        struct mli_Accelerator accel = mli_Accelerator_init();
        CHECK(accel.num_robjects == 0u);
}

CASE("mli_Accelerator, init")
{
        struct mli_Scenery scenery = mli_Scenery_init();
        struct mli_Color color;
        struct mli_Ray ray;
        struct mli_Prng prng = mli_Prng_init_MT19937(0);
        struct mli_pathtracer_Config tracer_config =
                mli_pathtracer_Config_init();
        struct mli_ColorMaterials color_materials = mli_ColorMaterials_init();
        struct mli_PathTracer tracer = mli_pathtracer_init();

        chk_dbg;
        CHECK(mli_Scenery__from_path_cstr(
                &scenery,
                "data/"
                "sceneries/"
                "001.tar"));
        chk_dbg;

        CHECK(mli_ColorMaterials_malloc_from_Materials(
                &color_materials, &scenery.materials));

        tracer.scenery = &scenery;
        tracer.scenery_color_materials = &color_materials;
        tracer.config = &tracer_config;

        ray = mli_Ray_set(
                mli_Vec_set(0.0, 0.0, -5.0), mli_Vec_set(0.0, 0.0, 1.0));

        color = mli_pathtracer_trace_ray(&tracer, ray, &prng);

        fprintf(stdout, "color (%e,%e,%e)\n", color.r, color.g, color.b);
        /*
        CHECK_MARGIN(color.r, 1.28e-2, 1e-3);
        CHECK_MARGIN(color.g, 1.55e-2, 1e-3);
        CHECK_MARGIN(color.b, 2.48e-2, 1e-3);
        */

        mli_ColorMaterials_free(&color_materials);
        mli_Scenery_free(&scenery);
}
