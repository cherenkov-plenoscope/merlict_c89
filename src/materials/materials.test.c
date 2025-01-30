/* Copyright 2019-2024 Sebastian Achim Mueller                                */

CASE("mli_Materials_init")
{
        struct mli_Materials mtl = mli_Materials_init();
        CHECK(mtl.spectra.size == 0u);
        CHECK(mtl.surfaces2.size == 0u);

        CHECK(mtl.num_surfaces == 0u);
        CHECK(mtl.surfaces == NULL);
        CHECK(mtl.num_media == 0u);
        CHECK(mtl.media == NULL);

        CHECK(mtl.default_medium == 0u);
}
