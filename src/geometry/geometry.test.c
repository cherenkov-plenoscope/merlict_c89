/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Geometry, init")
{
        struct mli_Geometry scenery = mli_Geometry_init();
        CHECK(scenery.num_objects == 0u);
        CHECK(scenery.objects == NULL);
        CHECK(scenery.num_robjects == 0u);
        CHECK(scenery.robjects == NULL);
        CHECK(scenery.robject_ids == NULL);
        CHECK(scenery.robject2root == NULL);
}
