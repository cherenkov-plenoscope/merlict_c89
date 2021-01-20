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
        struct mliTmpOcTree tmptree = mliTmpOcTree_init();

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

        CHECK(mliTmpOcTree_malloc_from_bundle(
                &tmptree,
                &scenery.resources.objects[0]));

        mliTmpOcTree_print(&tmptree);

        mliScenery_free(&scenery);
        mliAccelerator_free(&accel);
}
