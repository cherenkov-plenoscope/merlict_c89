/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliGeometry.h"

CASE("mliGeometry, init")
{
        struct mliGeometry scenery = mliGeometry_init();
        CHECK(scenery.num_objects == 0u);
        CHECK(scenery.objects == NULL);
        CHECK(scenery.num_robjects == 0u);
        CHECK(scenery.robjects == NULL);
        CHECK(scenery.robject_ids == NULL);
        CHECK(scenery.robject2root == NULL);
}

/*
CASE("mliGeometry, malloc from mliUserScenery")
{
        struct mliGeometry scn = mliGeometry_init();
        struct mliGeometry scn_back = mliGeometry_init();
        struct mliUserScenery uscn = mliUserScenery_init();

        CHECK(mliUserScenery_malloc_from_tar(
                &uscn,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        CHECK(mliGeometry_malloc_from_mliUserScenery(&scn, &uscn));

        CHECK(mliMaterials_equal(&scn.resources, &uscn.resources));
        mliUserScenery_free(&uscn);

        CHECK(mliGeometry_equal(&scn, &scn));

        CHECK(mliGeometry_write_to_path(
                &scn,
                "tests/"
                "resources/"
                "sceneries/"
                "001.mli.tmp"
        ));

        CHECK(mliGeometry_malloc_from_path(
                &scn_back,
                "tests/"
                "resources/"
                "sceneries/"
                "001.mli.tmp"
        ));

        CHECK(mliGeometry_equal(&scn, &scn_back));

        mliGeometry_free(&scn);
        mliGeometry_free(&scn_back);
}
*/
