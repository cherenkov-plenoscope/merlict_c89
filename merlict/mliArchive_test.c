/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliArchive.h"

CASE("mliArchive, read tar")
{
        struct mliArchive arc = mliArchive_init();
        struct mliIo *data = NULL;
        struct mliObject triangle = mliObject_init();

        CHECK(mliArchive_malloc_from_path(
                &arc,
                "merlict/"
                "tests/"
                "resources/"
                "sceneries/"
                "000.tar"));
        CHECK(mliArchive_num(&arc) == 13);

        CHECK(1 == mliArchive_num_filename_prefix_sufix(
                           &arc, "materials/media", ".json"));

        CHECK(1 == mliArchive_num_filename_prefix_sufix(
                           &arc, "geometry/objects/", ".obj"));

        CHECK(1 == mliArchive_num_filename_prefix_sufix(&arc, NULL, ".md"));

        CHECK(mliArchive_has(&arc, "README.md"));

        CHECK(mliArchive_has(&arc, "geometry/objects/"));
        CHECK(mliArchive_has(&arc, "geometry/objects/triangle.obj"));
        CHECK(mliArchive_get(&arc, "geometry/objects/triangle.obj", &data));
        /*CHECK(data->capacity == 119 + 1);*/

        CHECK(mliObject_malloc_from_wavefront(&triangle, (char *)data->cstr));
        CHECK(triangle.num_vertices == 3u);
        CHECK(triangle.num_vertex_normals == 1u);
        CHECK(triangle.num_faces == 1);

        CHECK(!mliArchive_has(&arc, "does-not-exist"));

        mliArchive_free(&arc);
        mliObject_free(&triangle);
}
