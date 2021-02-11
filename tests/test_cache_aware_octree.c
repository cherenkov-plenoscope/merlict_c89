/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("init mliCaOctree")
{
        struct mliOcTree octree = mliOcTree_init();
        CHECK(mliVec_equal(octree.cube.lower, mliVec_set(0.0, 0.0, 0.0)));
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

/*
CASE("init mliCaOctree")
{
        struct mliGeometry scenery = mliGeometry_init();
        struct mliTmpOcTree tmp_octree = mliTmpOcTree_init();
        struct mliOcTree octree = mliOcTree_init();
        struct mliIntersectionSurfaceNormal isec;
        uint64_t num_nodes, num_leafs, num_object_links;
        mliGeometry_read_from_path(&scenery, "tests/resources/scn1.mli.tmp");
        CHECK(mliTmpOcTree_malloc_from_scenery(&tmp_octree, &scenery));

        mliTmpNode_set_flat_index(&tmp_octree.root);
        mliTmpNode_num_nodes_leafs_objects(
                &tmp_octree.root, &num_nodes, &num_leafs, &num_object_links);

        CHECK(mliOcTree_malloc(
                &octree, num_nodes, num_leafs, num_object_links));
        mliOcTree_set(&octree, &tmp_octree);

        CHECK(num_nodes == 65);
        CHECK(num_leafs == 361);
        CHECK(num_object_links == 6068);

        CHECK(mliOcTree_equal_payload(&octree, &tmp_octree));

        mli_ray_octree_traversal(
                &scenery,
                &octree,
                mliRay_set(mliVec_set(0.1, 2.5, 10.), mliVec_set(0., 0., -1.)),
                &isec);

        mliTmpOcTree_free(&tmp_octree);
        mliOcTree_free(&octree);
        mliGeometry_free(&scenery);
}

CASE("init mliOctree write and read")
{
        struct mliGeometry scenery = mliGeometry_init();
        struct mliOcTree octree = mliOcTree_init();
        struct mliOcTree octree_b = mliOcTree_init();

        CHECK(mliGeometry_read_from_path(
                &scenery, "tests/resources/scn1.mli.tmp"));

        CHECK(mliOcTree_malloc_from_scenery(&octree, &scenery));

        CHECK(mliOcTree_write_to_path(
                &octree, "tests/resources/scn1.mli.octree.tmp"));

        CHECK(mliOcTree_malloc_from_path(
                &octree_b, "tests/resources/scn1.mli.octree.tmp"));

        CHECK(mliOcTree_equal(&octree, &octree_b));

        mliOcTree_free(&octree_b);
        mliOcTree_free(&octree);
        mliGeometry_free(&scenery);
}
*/
