/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliAccelerator, init")
{
        struct mliAccelerator accel = mliAccelerator_init();
        CHECK(accel.num_robjects == 0u);
}

CASE("mliAccelerator, init")
{
        struct mliScenery scenery = mliScenery_init();
        struct mliAccelerator accel = mliAccelerator_init();
        struct mliCombine combine;
        struct mliColor color;
        struct mliRay ray;

        CHECK(mliScenery_malloc_from_tape_archive(
                &scenery,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        mliScenery_fprint(stderr, &scenery);

        CHECK(mliAccelerator_malloc_from_scenery(&accel, &scenery));

        mliAccelerator_fprint(stderr, &accel);

        mliOcTree_print(&accel.scenery_octree);

        combine.scenery = &scenery;
        combine.accelerator = &accel;

        ray = mliRay_set(mliVec_set(0.0, 0.0, -5.0), mliVec_set(0.0, 0.0, 1.0));

        fprintf(stderr,
                "sup[%.1f, %.1f, %.1f] dir[%.1f, %.1f, %.1f]\n",
                ray.support.x,
                ray.support.y,
                ray.support.z,
                ray.direction.x,
                ray.direction.y,
                ray.direction.z);

        mliScenery_fprint(stderr, combine.scenery);

        fprintf(stderr, "========= start ======================\n");

        color = mli_trace(&combine, ray);

        mliScenery_free(&scenery);
        mliAccelerator_free(&accel);
}
