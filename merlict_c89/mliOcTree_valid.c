/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree_valid.h"

int mliOcTree_valid(struct mliOcTree *octree)
{
        uint64_t i, c;
        mli_check(!MLI_IS_NAN(octree->cube.lower.x), "cube.lower.x is 'nan'.");
        mli_check(!MLI_IS_NAN(octree->cube.lower.y), "cube.lower.y is 'nan'.");
        mli_check(!MLI_IS_NAN(octree->cube.lower.z), "cube.lower.z is 'nan'.");
        mli_check(!MLI_IS_NAN(octree->cube.edge_length),
                "cube.edge_length is 'nan'.");
        mli_check(octree->cube.edge_length >= 0.0,
                "Expected cube.edge_length >= 0.0.");

        for (i = 0u; i < octree->num_nodes; i++) {
                for (c = 0u; c < 8u; c++) {
                        if (octree->nodes[i].types[c] == MLI_OCTREE_TYPE_NONE) {
                                mli_check(octree->nodes[i].children[c] == 0u,
                                        "Expected the address of a 'NONE' "
                                        "child to be '0'.");
                        } else if (octree->nodes[i].types[c] == MLI_OCTREE_TYPE_NODE) {
                                mli_check(
                                        octree->nodes[i].children[c] <
                                        octree->num_nodes,
                                        "Expected the address of a 'NODE' "
                                        "child to be < num_nodes.");
                        } else if (octree->nodes[i].types[c] == MLI_OCTREE_TYPE_LEAF) {
                                mli_check(
                                        octree->nodes[i].children[c] <
                                        octree->num_nodes,
                                        "Expected the address of a 'LEAF' "
                                        "child to be < leafs.num_leafs.");
                        } else {
                                mli_sentinel(
                                        "Expected octree->nodes[i].type[c] "
                                        "to be either NONE, NODE, or LEAF.");
                        }
                }
        }
        return 1;
error:
        return 0;
}

int mliOcTree_valid_links(
        struct mliOcTree *octree,
        const uint32_t num_links)
{
        uint64_t i;
        for (i = 0u; i < octree->leafs.num_object_links; i++) {
                mli_check(octree->leafs.object_links[i] < num_links,
                        "Expected object_links[i] <  num_links.");
        }
        return 1;
error:
        return 0;
}
