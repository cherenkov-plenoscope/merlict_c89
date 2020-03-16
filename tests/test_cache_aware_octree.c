/* Copyright 2019-2020 Sebastian Achim Mueller                                */


CASE("init mliCaOctree") {
        struct mliCa2Octree caoctree = mliCa2Octree_init();
        CHECK(caoctree.cube.lower.x == 0.0);
}

CASE("sizeof mliNode") {
        CHECK(sizeof(struct mliNode) == 5*8);
}

CASE("init mliCaOctree") {
    struct mliScenery scenery = mliScenery_init();
    struct mliTmpOcTree tmp_octree = mliTmpOcTree_init();
    struct mliCa2Octree octree = mliCa2Octree_init();
    struct mliIntersection isec;
    size_t num_nodes, num_leafs, num_object_links;
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    CHECK(mliTmpOcTree_malloc_from_scenery(&tmp_octree, &scenery));
    fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);

    mliTmpNode_set_flat_index(&tmp_octree.root);
    mliTmpNode_num_nodes_leafs_objects(
        &tmp_octree.root,
        &num_nodes,
        &num_leafs,
        &num_object_links);

    /*mliTmpNode_print(&tmp_octree.root, 4u, 0u);*/


    CHECK(mliCa2Octree_malloc(&octree, num_nodes, num_leafs, num_object_links));


    mliCa2Octree_set(&octree, &tmp_octree);

    /*mliCa2Octree_print(&octree);*/

    fprintf(
        stderr,
        "num nodes: %lu, leafs: %lu, objects: %lu\n",
        num_nodes,
        num_leafs,
        num_object_links);

    CHECK(mliCa2Octree_equal_payload(&octree, &tmp_octree));

    mli_ray_octree_traversal(
        &scenery,
        &octree,
        mliRay_set(
            mliVec_set(0.1 ,2.5, 10.),
            mliVec_set(0. ,0., -1.)),
        &isec);

    mliTmpOcTree_free(&tmp_octree);
    mliCa2Octree_free(&octree);
    mliScenery_free(&scenery);
}
