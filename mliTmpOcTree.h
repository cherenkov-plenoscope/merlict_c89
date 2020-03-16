/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTTREE_H_
#define MERLICT_MLIOCTTREE_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliOBB.h"
#include "mliCube.h"
#include "mli_math.h"
#include "mliOctOverlaps.h"

#define MLI_TMPNODE_FLAT_INDEX_NONE -1

struct mliTmpNode {
        struct mliTmpNode* children[8];
        uint32_t num_objects;
        uint32_t *objects;

        int32_t flat_index;
        int32_t node_index;
        int32_t leaf_index;
};

struct mliTmpNode mliTmpNode_init()
{
        struct mliTmpNode n;
        uint64_t c;
        for (c = 0; c < 8u; c++) {
                n.children[c] = NULL;
        }
        n.num_objects = 0u;
        n.objects = NULL;
        n.flat_index = MLI_TMPNODE_FLAT_INDEX_NONE;
        n.node_index = MLI_TMPNODE_FLAT_INDEX_NONE;
        n.leaf_index = MLI_TMPNODE_FLAT_INDEX_NONE;
        return n;
}

void mliTmpNode_free(struct mliTmpNode *n)
{
        uint32_t c;
        for (c = 0; c < 8u; c++)
                if (n->children[c] != NULL)
                        mliTmpNode_free(n->children[c]);
        free(n->objects);
}

int mliTmpNode_malloc(struct mliTmpNode *n, const uint32_t num_objects)
{
        mliTmpNode_free(n);
        n->num_objects = num_objects;
        mli_malloc(n->objects, uint32_t, n->num_objects);
        return 1;
error:
        return 0;
}

uint32_t mliTmpNode_signs_to_child(
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz)
{
        return 4*sx + 2*sy + 1*sz;
}

int mliTmpNode_add_children(
        struct mliTmpNode *node,
        const struct mliScenery *scenery,
        const struct mliCube cube,
        const uint64_t depth,
        const uint64_t max_depth)
{
        uint32_t c;
        uint32_t sx, sy, sz, obj;
        struct mliCube child_cubes[8];
        struct mliOctOverlap overlap[8];

        if (node->num_objects <= 32u) {
                return 1;
        }

        if (depth == max_depth) {
                return 1;
        }

        for (c = 0u; c < 8u; c++) {
                overlap[c] = mliOctOverlap_init();
                mli_c(mliOctOverlap_malloc(&overlap[c], node->num_objects));
        }

        /* sense possible children */
        for (sx = 0u; sx < 2u; sx++) {
                for (sy = 0u; sy < 2u; sy++) {
                        for (sz = 0u; sz < 2u; sz++) {
                                const uint32_t child =
                                        mliTmpNode_signs_to_child(
                                                sx,
                                                sy,
                                                sz);
                                child_cubes[child] = mliCube_octree_child(
                                        cube,
                                        sx,
                                        sy,
                                        sz);
                                for (obj = 0u; obj < node->num_objects; obj++) {
                                        const uint32_t object_idx =
                                                node->objects[obj];
                                        if (mliScenery_overlap_obb(
                                                scenery,
                                                object_idx,
                                                mliCube_to_obb(
                                                        child_cubes[child]))
                                        ) {
                                                mli_c(mliOctOverlap_push_back(
                                                        &overlap[child],
                                                        object_idx));
                                        }
                                }
                        }
                }
        }

        for (c = 0; c < 8u; c++) {
                mli_malloc(node->children[c], struct mliTmpNode, 1u);
                (*node->children[c]) = mliTmpNode_init();
                mli_c(mliTmpNode_malloc(
                        node->children[c],
                        overlap[c].dyn.size));
                mli_uint32_ncpy(
                        overlap[c].arr,
                        node->children[c]->objects,
                        overlap[c].dyn.size);
        }

        for (c = 0u; c < 8u; c++) {
                mliOctOverlap_free(&overlap[c]);
        }

        for (c = 0; c < 8u; c++) {
                mliTmpNode_add_children(
                        node->children[c],
                        scenery,
                        child_cubes[c],
                        depth + 1u,
                        max_depth);
        }

        return 1;
error:
        return 0;
}

size_t mli_guess_octree_depth_based_on_num_objects(const size_t num_objects)
{
        return 1u + (size_t)ceil(log((double)num_objects)/log(8.0));
}

int mliTmpNode_malloc_tree_from_scenery(
        struct mliTmpNode* root_node,
        const struct mliScenery *scenery,
        const struct mliCube scenery_cube)
{
        uint32_t idx, start_depth, max_depth, num_objects;
        start_depth = 0u;
        num_objects = mliScenery_num_objects(scenery);
        max_depth = mli_guess_octree_depth_based_on_num_objects(num_objects);

        mli_check(mliTmpNode_malloc(root_node, num_objects),
                "Failed to allocate root-node in dynamic octree.")

        for (idx = 0; idx < root_node->num_objects; idx++) {
                root_node->objects[idx] = idx;
        }
        mliTmpNode_add_children(
                root_node,
                scenery,
                scenery_cube,
                start_depth,
                max_depth);
        return 1;
error:
        return 0;
}

int mliTmpNode_num_children(const struct mliTmpNode *node)
{
        uint32_t c, num = 0;
        for (c = 0u; c < 8u; c++)
                if (node->children[c] != NULL)
                        num++;
        return num;
}

void mliTmpNode_print(
        const struct mliTmpNode *node,
        const uint32_t indent,
        const uint32_t child)
{
        uint32_t i;
        uint32_t c;
        uint32_t num_children = mliTmpNode_num_children(node);
        for (i = 0u; i < indent; i++) printf(" ");
        if (num_children == 0) {
                uint32_t j;
                printf(
                        "|-Leaf[%d, %d] %u: %u [",
                        node->node_index,
                        node->leaf_index,
                        child, node->num_objects);
                for(j = 0; j < node->num_objects; j++) {
                        printf("%u, ", node->objects[j]);
                }
                printf("]");
        } else {
                printf(
                        "Node[%d, %d]: %u",
                        node->node_index,
                        node->leaf_index,
                        child);
        }
        printf("\n");
        for (c = 0u; c < 8u; c++) {
                if (node->children[c] != NULL) {
                        mliTmpNode_print(node->children[c], indent + 2, c);
                }
        }
}

/*
 * Assign a flat_index to every node that carries objects.
 * The ordering is:
 *      in tree:
 *                          3
 *                      1--{
 *                     /    4
 *                 0--{
 *                    \    5
 *                     2--{
 *                         6
 *
 *      in flat list:
 *      0, 1, 2, 3, 4, 5, 6, ...
 */

int _mliTmpNode_exists_and_objects(const struct mliTmpNode *node)
{
        if (node != NULL) {
                if (node->num_objects > 0u) {
                        return 1;
                }
        }
        return 0;
}

void _mliTmpNode_set_flat_index(
        struct mliTmpNode *node,
        int32_t *flat_index,
        int32_t *node_index,
        int32_t *leaf_index)
{
        size_t c;
        for (c = 0u; c < 8u; c++) {
                if(_mliTmpNode_exists_and_objects(node->children[c])) {
                        (*flat_index)++;
                        node->children[c]->flat_index = *flat_index;

                        if (mliTmpNode_num_children(node->children[c]) == 0) {
                                node->children[c]->leaf_index = *leaf_index;
                                (*leaf_index)++;
                        } else {
                                (*node_index)++;
                                node->children[c]->node_index = *node_index;
                        }
                }
        }
        for (c = 0u; c < 8u; c++) {
                if(_mliTmpNode_exists_and_objects(node->children[c])) {
                        _mliTmpNode_set_flat_index(
                                node->children[c],
                                flat_index,
                                node_index,
                                leaf_index);
                }
        }
}

void mliTmpNode_set_flat_index(struct mliTmpNode *root_node)
{
        int32_t flat_index = 0;
        int32_t node_index = 0;
        int32_t leaf_index = 0;
        root_node->flat_index = flat_index;

        if (mliTmpNode_num_children(root_node) == 0) {
                root_node->leaf_index = leaf_index;
        } else {
                root_node->node_index = node_index;
        }

        _mliTmpNode_set_flat_index(
                root_node,
                &flat_index,
                &node_index,
                &leaf_index);
}

/*
 * Find the number of valid nodes in dynamic tree
 */

void _mliTmpNode_num_nodes_leafs_objects(
        const struct mliTmpNode *node,
        size_t *num_nodes,
        size_t *num_leafs,
        size_t *num_object_links)
{
        size_t c;
        if (node->node_index != MLI_TMPNODE_FLAT_INDEX_NONE) {
                (*num_nodes)++;
        }
        if (node->leaf_index != MLI_TMPNODE_FLAT_INDEX_NONE) {
                (*num_leafs)++;
                (*num_object_links) += node->num_objects;
        }
        for (c = 0; c < 8u; c++) {
                if (node->children[c] != NULL) {
                        _mliTmpNode_num_nodes_leafs_objects(
                                node->children[c],
                                num_nodes,
                                num_leafs,
                                num_object_links);
                }
        }
}

void mliTmpNode_num_nodes_leafs_objects(
        const struct mliTmpNode *root_node,
        size_t *num_nodes,
        size_t *num_leafs,
        size_t *num_object_links)
{
        *num_nodes = 0;
        *num_leafs = 0;
        *num_object_links = 0;
        _mliTmpNode_num_nodes_leafs_objects(
                root_node,
                num_nodes,
                num_leafs,
                num_object_links);
}

/*
 * The dynamic octree
 */

struct mliTmpOcTree {
        struct mliCube cube;
        struct mliTmpNode root;
};

struct mliTmpOcTree mliTmpOcTree_init()
{
        struct mliTmpOcTree octree;
        octree.cube.lower = mliVec_set(0., 0., 0.);
        octree.cube.edge_length = 0.;
        octree.root = mliTmpNode_init();
        return octree;
}

void mliTmpOcTree_free(struct mliTmpOcTree *octree)
{
        mliTmpNode_free(&octree->root);
}

int mliTmpOcTree_malloc_from_scenery(
        struct mliTmpOcTree *octree,
        const struct mliScenery *scenery)
{
        mliTmpOcTree_free(octree);
        octree->cube = mliCube_outermost_cube(
                mliScenery_outermost_obb(scenery));
        mli_check(mliTmpNode_malloc_tree_from_scenery(
                &octree->root,
                scenery,
                octree->cube),
                "Failed to allocate dynamic octree from scenery.");
        return 1;
error:
        return 0;
}

#endif
