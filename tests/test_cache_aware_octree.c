/* Copyright 2019-2020 Sebastian Achim Mueller                                */


CASE("init mliCaOctree") {
        struct mliCa2Octree caoctree = mliCa2Octree_init();
        CHECK(caoctree.cube.lower.x == 0.0);
}

CASE("sizeof mliCa2Node") {
        CHECK(sizeof(struct mliCa2Node) == 5*8);
}

CASE("init mliCaOctree") {
    struct mliScenery scenery = mliScenery_init();
    struct mliOcTree octree = mliOcTree_init();
    struct mliCa2Octree tree2 = mliCa2Octree_init();
    struct mliIntersection isec;
    size_t num_nodes, num_leafs, num_object_links;
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    CHECK(mliOcTree_malloc_from_scenery(&octree, &scenery));
    fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);

    mliNode_set_flat_index(&octree.root);
    mliNode_num_nodes_leafs_objects(
        &octree.root,
        &num_nodes,
        &num_leafs,
        &num_object_links);

    /*mliNode_print(&octree.root, 4u, 0u);*/


    CHECK(mliCa2Octree_malloc(&tree2, num_nodes, num_leafs, num_object_links));


    mliCa2Octree_set(&tree2, &octree);

    /*mliCa2Octree_print(&tree2);*/

    fprintf(
        stderr,
        "num nodes: %lu, leafs: %lu, objects: %lu\n",
        num_nodes,
        num_leafs,
        num_object_links);

    CHECK(mliCa2Octree_equal_payload(&tree2, &octree));

    mli_ray_octree_traversal(
        &scenery,
        &octree,
        mliRay_set(
            mliVec_set(0.1 ,2.5, 10.),
            mliVec_set(0. ,0., -1.)),
        &isec);

    mliOcTree_free(&octree);
    mliCa2Octree_free(&tree2);
    mliScenery_free(&scenery);
}
