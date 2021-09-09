/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree_valid.h"

int mliOcTree_valid(const struct mliOcTree *octree)
{
        uint32_t n, c;
        mli_check_message(!MLI_IS_NAN(octree->cube.lower.x), "cube.lower.x is 'nan'.");
        mli_check_message(!MLI_IS_NAN(octree->cube.lower.y), "cube.lower.y is 'nan'.");
        mli_check_message(!MLI_IS_NAN(octree->cube.lower.z), "cube.lower.z is 'nan'.");
        mli_check_message(
                !MLI_IS_NAN(octree->cube.edge_length),
                "cube.edge_length is 'nan'.");
        mli_check_message(
                octree->cube.edge_length >= 0.0,
                "Expected cube.edge_length >= 0.0.");

        for (n = 0u; n < octree->num_nodes; n++) {
                for (c = 0u; c < 8u; c++) {
                        if (octree->nodes[n].types[c] == MLI_OCTREE_TYPE_NONE) {
                                mli_check_message(
                                        octree->nodes[n].children[c] == 0u,
                                        "Expected the address of a 'NONE' "
                                        "child to be '0'.");
                        } else if (
                                octree->nodes[n].types[c] ==
                                MLI_OCTREE_TYPE_NODE) {
                                mli_check_message(
                                        octree->nodes[n].children[c] <
                                                octree->num_nodes,
                                        "Expected the address of a 'NODE' "
                                        "child to be < num_nodes.");
                        } else if (
                                octree->nodes[n].types[c] ==
                                MLI_OCTREE_TYPE_LEAF) {
                                mli_check_message(
                                        octree->nodes[n].children[c] <
                                                octree->leafs.num_leafs,
                                        "Expected the address of a 'LEAF' "
                                        "child to be < leafs.num_leafs.");
                        } else {
                                mli_sentinel(
                                        "Expected octree->nodes[n].type[c] "
                                        "to be either NONE, NODE, or LEAF.");
                        }
                }
        }
        return 1;
error:
        return 0;
}

int mliOcTree_valid_wrt_links(
        const struct mliOcTree *octree,
        const uint32_t num_links)
{
        uint32_t n;
        for (n = 0u; n < octree->leafs.num_object_links; n++) {
                mli_check_message(
                        octree->leafs.object_links[n] < num_links,
                        "Expected object_links[n] <  num_links.");
        }
        return 1;
error:
        return 0;
}
