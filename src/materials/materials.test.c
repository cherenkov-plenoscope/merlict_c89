/* Copyright 2019-2024 Sebastian Achim Mueller                                */

CASE("mli_Materials_init")
{
        struct mli_Materials resources = mli_Materials_init();
        CHECK(resources.num_spectra == 0u);
        CHECK(resources.num_surfaces == 0u);
        CHECK(resources.surfaces == NULL);
        CHECK(resources.num_media == 0u);
        CHECK(resources.media == NULL);
}
