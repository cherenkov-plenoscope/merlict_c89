/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "octree_tmp.h"
#include "../math/math.h"
#include "../object/object_AABB.h"
#include "../vector/uint32_vector.h"
#include "../chk/chk.h"

uint64_t mli_octree_guess_depth_based_on_num_objects(const uint64_t num_objects)
{
        return 3u + (uint64_t)ceil(log((double)num_objects) / log(8.0));
}

/*
 * The dynamic node
 * ================
 */

struct mli_octree_TmpNode mli_octree_TmpNode_init(void)
{
        struct mli_octree_TmpNode n;
        uint64_t c;
        for (c = 0; c < 8u; c++) {
                n.children[c] = NULL;
        }
        n.num_objects = 0u;
        n.objects = NULL;
        n.flat_index = MLI_OCTREE_TMPNODE_FLAT_INDEX_NONE;
        n.node_index = MLI_OCTREE_TMPNODE_FLAT_INDEX_NONE;
        n.leaf_index = MLI_OCTREE_TMPNODE_FLAT_INDEX_NONE;
        return n;
}

void mli_octree_TmpNode_free(struct mli_octree_TmpNode *n)
{
        uint32_t c;
        for (c = 0; c < 8u; c++)
                if (n->children[c] != NULL)
                        mli_octree_TmpNode_free(n->children[c]);
        free(n->objects);
}

chk_rc mli_octree_TmpNode_malloc(
        struct mli_octree_TmpNode *n,
        const uint32_t num_objects)
{
        mli_octree_TmpNode_free(n);
        n->num_objects = num_objects;
        chk_malloc(n->objects, uint32_t, n->num_objects);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint32_t mli_octree_TmpNode_signs_to_child(
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz)
{
        return 4 * sx + 2 * sy + 1 * sz;
}

chk_rc mli_octree_TmpNode_add_children(
        struct mli_octree_TmpNode *node,
        const void *bundle,
        mli_bool (*item_in_bundle_has_overlap_aabb)(
                const void *,
                const uint32_t,
                const struct mli_AABB),
        const struct mli_Cube cube,
        const uint64_t depth,
        const uint64_t max_depth)
{
        uint32_t c;
        uint32_t sx, sy, sz, obj;
        struct mli_Cube child_cubes[8];
        struct mli_octree_OverlapVector overlap[8];

        if (node->num_objects <= 32u) {
                return CHK_SUCCESS;
        }

        if (depth == max_depth) {
                return CHK_SUCCESS;
        }

        for (c = 0u; c < 8u; c++) {
                overlap[c] = mli_octree_OverlapVector_init();
                chk(mli_octree_OverlapVector_malloc(
                        &overlap[c], node->num_objects));
        }

        /* sense possible children */
        for (sx = 0u; sx < 2u; sx++) {
                for (sy = 0u; sy < 2u; sy++) {
                        for (sz = 0u; sz < 2u; sz++) {
                                const uint32_t child =
                                        mli_octree_TmpNode_signs_to_child(
                                                sx, sy, sz);
                                child_cubes[child] =
                                        mli_Cube_octree_child(cube, sx, sy, sz);
                                for (obj = 0u; obj < node->num_objects; obj++) {
                                        const uint32_t object_idx =
                                                node->objects[obj];
                                        if (item_in_bundle_has_overlap_aabb(
                                                    bundle,
                                                    object_idx,
                                                    mli_Cube_to_aabb(
                                                            child_cubes
                                                                    [child]))) {
                                                chk(mli_octree_OverlapVector_push_back(
                                                        &overlap[child],
                                                        object_idx));
                                        }
                                }
                        }
                }
        }

        for (c = 0; c < 8u; c++) {
                chk_malloc(node->children[c], struct mli_octree_TmpNode, 1u);
                (*node->children[c]) = mli_octree_TmpNode_init();
                chk(mli_octree_TmpNode_malloc(
                        node->children[c], overlap[c].size));
                MLI_MATH_NCPY(
                        overlap[c].array,
                        node->children[c]->objects,
                        overlap[c].size);
        }

        for (c = 0u; c < 8u; c++) {
                mli_octree_OverlapVector_free(&overlap[c]);
        }

        for (c = 0; c < 8u; c++) {
                mli_octree_TmpNode_add_children(
                        node->children[c],
                        bundle,
                        item_in_bundle_has_overlap_aabb,
                        child_cubes[c],
                        depth + 1u,
                        max_depth);
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_octree_TmpNode_malloc_tree_from_bundle(
        struct mli_octree_TmpNode *root_node,
        const void *bundle,
        const uint32_t num_items_in_bundle,
        mli_bool (*item_in_bundle_has_overlap_aabb)(
                const void *,
                const uint32_t,
                const struct mli_AABB),
        const struct mli_Cube bundle_cube)
{
        uint32_t idx, start_depth, max_depth;
        start_depth = 0u;
        max_depth = mli_octree_guess_depth_based_on_num_objects(
                num_items_in_bundle);

        chk_msg(mli_octree_TmpNode_malloc(root_node, num_items_in_bundle),
                "Failed to allocate root-node in dynamic octree.");

        for (idx = 0; idx < root_node->num_objects; idx++) {
                root_node->objects[idx] = idx;
        }
        mli_octree_TmpNode_add_children(
                root_node,
                bundle,
                item_in_bundle_has_overlap_aabb,
                bundle_cube,
                start_depth,
                max_depth);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint32_t mli_octree_TmpNode_num_children(const struct mli_octree_TmpNode *node)
{
        uint32_t c, num = 0;
        for (c = 0u; c < 8u; c++)
                if (node->children[c] != NULL)
                        num++;
        return num;
}

void mli_octree_TmpNode_print(
        const struct mli_octree_TmpNode *node,
        const uint32_t indent,
        const uint32_t child)
{
        uint32_t i;
        uint32_t c;
        uint32_t num_children = mli_octree_TmpNode_num_children(node);
        for (i = 0u; i < indent; i++)
                printf(" ");
        if (num_children == 0) {
                uint32_t j;
                printf("|-Leaf[%d, %d] %u: %u [",
                       node->node_index,
                       node->leaf_index,
                       child,
                       node->num_objects);
                for (j = 0; j < node->num_objects; j++) {
                        printf("%u, ", node->objects[j]);
                }
                printf("]");
        } else {
                printf("Node[%d, %d]: %u",
                       node->node_index,
                       node->leaf_index,
                       child);
        }
        printf("\n");
        for (c = 0u; c < 8u; c++) {
                if (node->children[c] != NULL) {
                        mli_octree_TmpNode_print(
                                node->children[c], indent + 2, c);
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

mli_bool mli_octree_TmpNode_exists_and_has_objects(
        const struct mli_octree_TmpNode *node)
{
        if (node != NULL) {
                if (node->num_objects > 0u) {
                        return MLI_TRUE;
                }
        }
        return MLI_FALSE;
}

void mli_octree_TmpNode_set_flat_index_walk(
        struct mli_octree_TmpNode *node,
        int32_t *flat_index,
        int32_t *node_index,
        int32_t *leaf_index)
{
        uint64_t c;
        for (c = 0u; c < 8u; c++) {
                if (mli_octree_TmpNode_exists_and_has_objects(
                            node->children[c])) {
                        (*flat_index)++;
                        node->children[c]->flat_index = *flat_index;

                        if (mli_octree_TmpNode_num_children(
                                    node->children[c]) == 0) {
                                node->children[c]->leaf_index = *leaf_index;
                                (*leaf_index)++;
                        } else {
                                (*node_index)++;
                                node->children[c]->node_index = *node_index;
                        }
                }
        }
        for (c = 0u; c < 8u; c++) {
                if (mli_octree_TmpNode_exists_and_has_objects(
                            node->children[c])) {
                        mli_octree_TmpNode_set_flat_index_walk(
                                node->children[c],
                                flat_index,
                                node_index,
                                leaf_index);
                }
        }
}

void mli_octree_TmpNode_set_flat_index(struct mli_octree_TmpNode *root_node)
{
        int32_t flat_index = 0;
        int32_t node_index = 0;
        int32_t leaf_index = 0;
        root_node->flat_index = flat_index;

        if (mli_octree_TmpNode_num_children(root_node) == 0) {
                root_node->leaf_index = leaf_index;
        } else {
                root_node->node_index = node_index;
        }

        mli_octree_TmpNode_set_flat_index_walk(
                root_node, &flat_index, &node_index, &leaf_index);
}

/*
 * Find the number of valid nodes in dynamic tree
 */

void mli_octree_TmpNode_num_nodes_leafs_objects_walk(
        const struct mli_octree_TmpNode *node,
        uint64_t *num_nodes,
        uint64_t *num_leafs,
        uint64_t *num_object_links)
{
        uint64_t c;
        if (node->node_index != MLI_OCTREE_TMPNODE_FLAT_INDEX_NONE) {
                (*num_nodes)++;
        }
        if (node->leaf_index != MLI_OCTREE_TMPNODE_FLAT_INDEX_NONE) {
                (*num_leafs)++;
                (*num_object_links) += node->num_objects;
        }
        for (c = 0; c < 8u; c++) {
                if (node->children[c] != NULL) {
                        mli_octree_TmpNode_num_nodes_leafs_objects_walk(
                                node->children[c],
                                num_nodes,
                                num_leafs,
                                num_object_links);
                }
        }
}

void mli_octree_TmpNode_num_nodes_leafs_objects(
        const struct mli_octree_TmpNode *root_node,
        uint64_t *num_nodes,
        uint64_t *num_leafs,
        uint64_t *num_object_links)
{
        *num_nodes = 0;
        *num_leafs = 0;
        *num_object_links = 0;
        mli_octree_TmpNode_num_nodes_leafs_objects_walk(
                root_node, num_nodes, num_leafs, num_object_links);
}

/*
 * The dynamic octree
 * ==================
 */

struct mli_octree_TmpOcTree mli_octree_TmpOcTree_init(void)
{
        struct mli_octree_TmpOcTree octree;
        octree.cube.lower = mli_Vec_set(0., 0., 0.);
        octree.cube.edge_length = 0.;
        octree.root = mli_octree_TmpNode_init();
        return octree;
}

void mli_octree_TmpOcTree_free(struct mli_octree_TmpOcTree *octree)
{
        mli_octree_TmpNode_free(&octree->root);
}

chk_rc mli_octree_TmpOcTree_malloc_from_bundle(
        struct mli_octree_TmpOcTree *octree,
        const void *bundle,
        const uint32_t num_items_in_bundle,
        mli_bool (*item_in_bundle_has_overlap_aabb)(
                const void *,
                const uint32_t,
                const struct mli_AABB),
        struct mli_AABB bundle_aabb)
{
        mli_octree_TmpOcTree_free(octree);
        octree->cube = mli_Cube_outermost_cube(bundle_aabb);
        chk_msg(mli_octree_TmpNode_malloc_tree_from_bundle(
                        &octree->root,
                        bundle,
                        num_items_in_bundle,
                        item_in_bundle_has_overlap_aabb,
                        octree->cube),
                "Failed to allocate dynamic octree from bundle.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_octree_TmpOcTree_print(const struct mli_octree_TmpOcTree *octree)
{
        mli_octree_TmpNode_print(&octree->root, 0u, 0u);
}
