/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/include/mli_testing.h"
#include "../src/mliOcTree.h"

CASE("init mliCaOctree")
{
        struct mliOcTree octree = mliOcTree_init();
        CHECK(mliVec_equal(octree.cube.lower, mliVec_init(0.0, 0.0, 0.0)));
        CHECK(octree.cube.edge_length == 0.0);
        CHECK(octree.num_nodes == 0);
        CHECK(octree.leafs.num_leafs == 0);
        CHECK(octree.root_type == MLI_OCTREE_TYPE_NONE);
        CHECK(octree.nodes == NULL);
        CHECK(octree.leafs.adresses == NULL);
        CHECK(octree.leafs.num_object_links == 0);
        CHECK(octree.leafs.object_links == NULL);
}

CASE("sizeof mliNode") { CHECK(sizeof(struct mliNode) == 5 * 8); }

CASE("ray parallel to axis")
{
        struct mliScenery scenery = mliScenery_init();
        uint32_t i;
        uint32_t robj = 0;
        uint32_t obj;
        struct mliIntersection isec;
        struct mliVec sups[6];
        struct mliVec dirs[6];
        const double N = 0.0;

        sups[0] = mliVec_init(0, 0, -10);
        dirs[0] = mliVec_init(N, N, 1);

        sups[1] = mliVec_init(0, 0, 10);
        dirs[1] = mliVec_init(N, N, -1);

        sups[2] = mliVec_init(-10, 0, 0);
        dirs[2] = mliVec_init(1, N, N);

        sups[2] = mliVec_init(10, 0, 0);
        dirs[2] = mliVec_init(-1, N, N);

        sups[3] = mliVec_init(-10, 0, 0);
        dirs[3] = mliVec_init(1, N, N);

        sups[4] = mliVec_init(0, -10, 0);
        dirs[4] = mliVec_init(N, 1, N);

        sups[5] = mliVec_init(0, 10, 0);
        dirs[5] = mliVec_init(N, -1, N);

        CHECK(mliScenery_malloc_from_path_tar(
                &scenery,
                "mli_core/"
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));

        obj = scenery.geometry.robjects[robj];
        CHECK(strcmp("teapot", scenery.geometry.object_names[obj].cstr) == 0);

        for (i = 0; i < 6; i++) {
                struct mliRay ray;

                ray = mliRay_set(sups[i], dirs[i]);

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
