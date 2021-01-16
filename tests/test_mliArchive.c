/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliArchive, read tar")
{
        struct mliArchive arc = mliArchive_init();
        struct mliString *data = NULL;
        struct mliObject hexagonal_mirror_facet = mliObject_init();

        CHECK(
                mliArchive_malloc_from_tar(
                        &arc,
                        "tests/"
                        "resources/"
                        "sceneries/"
                        "001.tar"
                )
        );
        CHECK(mliArchive_num(&arc) == 8);

        CHECK(mliArchive_has(&arc, "README.md"));

        CHECK(mliArchive_has(&arc, "functions/"));
        CHECK(mliArchive_has(&arc, "functions/refractive_index_water.csv"));
        CHECK(mliArchive_has(&arc, "functions/zero.csv"));

        CHECK(mliArchive_has(&arc, "objects/"));
        CHECK(mliArchive_has(&arc, "objects/teapot.obj"));
        CHECK(mliArchive_has(&arc, "objects/hexagonal_mirror_facet.obj"));
        CHECK(mliArchive_get(&arc, "objects/hexagonal_mirror_facet.obj", &data));
        CHECK(data->capacity == 36527 + 1);

        CHECK(mliObject_malloc_from_string(&hexagonal_mirror_facet, data->c_str));
        CHECK(hexagonal_mirror_facet.num_vertices == 331u);
        CHECK(hexagonal_mirror_facet.num_vertex_normals == 331u);
        CHECK(hexagonal_mirror_facet.num_faces == 600);

        CHECK(!mliArchive_has(&arc, "does-not-exist"));

        mliArchive_free(&arc);
        mliObject_free(&hexagonal_mirror_facet);
}
