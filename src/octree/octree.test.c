/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("init mliCaOctree")
{
        struct mli_OcTree octree = mli_OcTree_init();
        CHECK(mli_Vec_equal(octree.cube.lower, mli_Vec_init(0.0, 0.0, 0.0)));
        CHECK(octree.cube.edge_length == 0.0);
        CHECK(octree.num_nodes == 0);
        CHECK(octree.leafs.num_leafs == 0);
        CHECK(octree.root_type == MLI_OCTREE_TYPE_NONE);
        CHECK(octree.nodes == NULL);
        CHECK(octree.leafs.adresses == NULL);
        CHECK(octree.leafs.num_object_links == 0);
        CHECK(octree.leafs.object_links == NULL);
}

CASE("sizeof mli_octree_Node")
{
        CHECK(sizeof(struct mli_octree_Node) == 5 * 8);
}

CASE("ray parallel to axis")
{
        struct mliScenery scenery = mliScenery_init();
        uint32_t i;
        uint32_t robj = 0;
        uint32_t obj;
        struct mliIntersection isec;
        struct mli_Vec sups[6];
        struct mli_Vec dirs[6];
        const double N = 0.0;

        sups[0] = mli_Vec_init(0, 0, -10);
        dirs[0] = mli_Vec_init(N, N, 1);

        sups[1] = mli_Vec_init(0, 0, 10);
        dirs[1] = mli_Vec_init(N, N, -1);

        sups[2] = mli_Vec_init(-10, 0, 0);
        dirs[2] = mli_Vec_init(1, N, N);

        sups[2] = mli_Vec_init(10, 0, 0);
        dirs[2] = mli_Vec_init(-1, N, N);

        sups[3] = mli_Vec_init(-10, 0, 0);
        dirs[3] = mli_Vec_init(1, N, N);

        sups[4] = mli_Vec_init(0, -10, 0);
        dirs[4] = mli_Vec_init(N, 1, N);

        sups[5] = mli_Vec_init(0, 10, 0);
        dirs[5] = mli_Vec_init(N, -1, N);

        CHECK(mliScenery_malloc_from_path_tar(
                &scenery,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));

        obj = scenery.geometry.robjects[robj];
        CHECK(mli_String_equal_cstr(
                &scenery.geometry.object_names[obj], "teapot"));

        for (i = 0; i < 6; i++) {
                struct mli_Ray ray;

                ray = mli_Ray_set(sups[i], dirs[i]);

                isec = mliIntersection_init();

                mli_query_object_reference(
                        &scenery.geometry.objects[obj],
                        &scenery.accelerator.object_octrees[obj],
                        scenery.geometry.robject2root[robj],
                        ray,
                        &isec);

                CHECK(isec.distance_of_ray != DBL_MAX);
        }

        mliScenery_free(&scenery);
}
