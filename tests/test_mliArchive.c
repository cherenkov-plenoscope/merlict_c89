/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliArchive, read tar")
{
        struct mliArchive arc = mliArchive_init();
        struct mliString *f = NULL;
        struct mliObject triangle = mliObject_init();

        CHECK(mliArchive_malloc_from_tar(&arc, "tests/resources/minimal.tar"));
        CHECK(mliArchive_num(&arc) == 4);

        CHECK(mliArchive_has(&arc, "function.csv"));
        CHECK(mliArchive_has(&arc, "README.md"));
        CHECK(mliArchive_has(&arc, "scenery.json"));
        CHECK(mliArchive_has(&arc, "triangle.obj"));

        CHECK(mliArchive_get(&arc, "triangle.obj", &f));
        CHECK(f->capacity == 91);

        CHECK(mliObject_malloc_from_string(&triangle, f->c_str));
        CHECK(triangle.num_faces == 1u);
        CHECK(triangle.num_vertices == 3u);
        CHECK(triangle.num_vertex_normals == 1u);

        CHECK(!mliArchive_has(&arc, "does-not-exist"));
        mliArchive_free(&arc);
        mliObject_free(&triangle);
}
