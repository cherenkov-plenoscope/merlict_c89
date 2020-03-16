/* Copyright 2019-2020 Sebastian Achim Mueller                                */


CASE("init mliCaOctree") {
        struct mliCaOctree caoctree = mliCaOctree_init();
        CHECK(caoctree.cube.lower.x == 0.0);
}

CASE("init mliCaOctree") {
    struct mliScenery scenery = mliScenery_init();
    struct mliOcTree octree = mliOcTree_init();
    struct mliCaOctree caoctree = mliCaOctree_init();
    struct mliIntersection isec;
    mliScenery_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
    CHECK(mliOcTree_malloc_from_scenery(&octree, &scenery));
    fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
    CHECK(mliCaOctree_malloc(&caoctree));

    mliNode_set_flat_index(&octree.root);

    CHECK(mliCaOctree_set_with_dynamic_octree(
        &caoctree,
        &octree));

    fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
    mliNode_print(&octree.root, 4, 0u);
    fprintf(
        stderr,
        "num_nodes: %u\n",
        mliNode_capacity_nodes(&octree.root));
    fprintf(
        stderr,
        "capacity_objects: %u\n",
        mliNode_capacity_objects(&octree.root));

    mliCaOctree_print(&caoctree);

    mli_ray_octree_traversal(
        &scenery,
        &octree,
        mliRay_set(
            mliVec_set(0.1 ,2.5, 10.),
            mliVec_set(0. ,0., -1.)),
        &isec);

    mliOcTree_free(&octree);
    mliCaOctree_free(&caoctree);
    mliScenery_free(&scenery);
}
