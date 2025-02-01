/* Copyright 2019-2024 Sebastian Achim Mueller                                */

CASE("mli_Materials_init")
{
        struct mli_Materials mtl = mli_Materials_init();
        CHECK(mtl.spectra.size == 0u);
        CHECK(mtl.surfaces.size == 0u);
        CHECK(mtl.media.size == 0u);
        CHECK(mtl.boundary_layers.size == 0u);
        CHECK(mtl.default_medium == 0u);
}
