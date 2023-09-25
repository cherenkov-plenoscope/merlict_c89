/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/include/mli_testing.h"
#include "mliMedium.h"

CASE("mliMedium_json")
{
        struct mliMedium med = mliMedium_init();
        char json_str[] = "{"
                          "\"refraction\": [[0, 10], [1, 11], [2, 12]],"
                          "\"absorbtion\": [[5, 50], [6, 51], [7, 52]]"
                          "}";
        CHECK(mliMedium_malloc_from_json_str(&med, json_str));

        CHECK(med.refraction.num_points == 3);
        CHECK(med.refraction.x[0] == 0);
        CHECK(med.refraction.x[1] == 1);
        CHECK(med.refraction.x[2] == 2);

        CHECK(med.refraction.y[0] == 10);
        CHECK(med.refraction.y[1] == 11);
        CHECK(med.refraction.y[2] == 12);

        CHECK(med.absorbtion.num_points == 3);
        CHECK(med.absorbtion.x[0] == 5);
        CHECK(med.absorbtion.x[1] == 6);
        CHECK(med.absorbtion.x[2] == 7);

        CHECK(med.absorbtion.y[0] == 50);
        CHECK(med.absorbtion.y[1] == 51);
        CHECK(med.absorbtion.y[2] == 52);

        mliMedium_free(&med);
}
