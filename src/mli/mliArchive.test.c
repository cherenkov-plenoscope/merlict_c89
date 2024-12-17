/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliArchive, read tar")
{
        struct mliArchive arc = mliArchive_init();
        struct mli_String *data = NULL;
        struct mli_IO io = mli_IO_init();
        struct mli_Object triangle = mli_Object_init();
        struct mli_String tmp = mli_String_init();

        CHECK(mliArchive_from_path(
                &arc,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "000.tar"));
        CHECK(mliArchive_size(&arc) == 13);

        CHECK(1 == mliArchive_num_filename_prefix_sufix(
                           &arc, "materials/media", ".json"));

        CHECK(1 == mliArchive_num_filename_prefix_sufix(
                           &arc, "geometry/objects/", ".obj"));

        CHECK(1 == mliArchive_num_filename_prefix_sufix(&arc, "", ".md"));

        CHECK(mli_String_from_cstr(&tmp, "README.md"));
        CHECK(mliArchive_has(&arc, &tmp));

        CHECK(mli_String_from_cstr(&tmp, "geometry/objects/"));
        CHECK(mliArchive_has(&arc, &tmp));

        CHECK(mli_String_from_cstr(&tmp, "geometry/objects/triangle.obj"));
        CHECK(mliArchive_has(&arc, &tmp));

        CHECK(mliArchive_get(&arc, &tmp, &data));

        CHECK(mli_IO_open_memory(&io));
        CHECK(mli_IO_text_write_String(&io, data));
        mli_IO_rewind(&io);
        CHECK(mli_Object_malloc_from_wavefront(&triangle, &io));
        CHECK(triangle.num_vertices == 3u);
        CHECK(triangle.num_vertex_normals == 1u);
        CHECK(triangle.num_faces == 1);

        CHECK(mli_String_from_cstr(&tmp, "does-not-exist"));
        CHECK(!mliArchive_has(&arc, &tmp));

        mli_String_free(&tmp);
        mli_IO_close(&io);
        mliArchive_free(&arc);
        mli_Object_free(&triangle);
}
