/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOcTree_equal.h"

int mliOcTree_equal(const struct mliOcTree *a, const struct mliOcTree *b)
{
        uint64_t i, j;
        chk_msg(a->num_nodes == b->num_nodes, "num_nodes not equal.");
        chk_msg(
                a->leafs.num_leafs == b->leafs.num_leafs,
                "leafs.num_leafs not equal.");
        chk_msg(
                a->leafs.num_object_links == b->leafs.num_object_links,
                "num_object_links not equal.");

        chk_msg(a->cube.lower.x == b->cube.lower.x, "cube.lower.x not eaual");
        chk_msg(a->cube.lower.y == b->cube.lower.y, "cube.lower.y not eaual");
        chk_msg(a->cube.lower.z == b->cube.lower.z, "cube.lower.z not eaual");
        chk_msg(
                a->cube.edge_length == b->cube.edge_length,
                "cube.edge_length not eaual");

        chk_msg(a->root_type == b->root_type, "cube.root_type not eaual");

        for (i = 0; i < a->num_nodes; i++) {
                for (j = 0; j < 8; j++) {
                        chk_msg(
                                a->nodes[i].children[j] ==
                                        b->nodes[i].children[j],
                                "octree.nodes[i].children[j] not equal");
                        chk_msg(
                                a->nodes[i].types[j] == b->nodes[i].types[j],
                                "octree.nodes[i].types[j] not equal");
                }
        }

        for (i = 0; i < a->leafs.num_leafs; i++) {
                chk_msg(
                        a->leafs.adresses[i].first_object_link ==
                                b->leafs.adresses[i].first_object_link,
                        "octree.leafs.adresses[i].first_object_link not equal");
                chk_msg(
                        a->leafs.adresses[i].num_object_links ==
                                b->leafs.adresses[i].num_object_links,
                        "octree.leafs.adresses[i].num_object_links not equal");
        }
        for (i = 0; i < a->leafs.num_object_links; i++) {
                chk_msg(
                        a->leafs.object_links[i] == b->leafs.object_links[i],
                        "octree.leafs.object_links[i] not equal");
        }

        return 1;
error:
        return 0;
}
