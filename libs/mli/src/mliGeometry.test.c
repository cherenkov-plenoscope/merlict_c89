/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliGeometry.h"

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
