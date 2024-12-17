/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_TMP_H_
#define MLI_OCTREE_TMP_H_

#include <stdint.h>
#include "../mli/mliGeometry.h"
#include "../aabb/aabb.h"
#include "../mli/mliCube.h"
#include "octree_overlaps.h"

#define MLI_OCTREE_TMPNODE_FLAT_INDEX_NONE -1

uint64_t mli_octree_guess_depth_based_on_num_objects(
        const uint64_t num_objects);

/*
 * The dynamic node
 * ================
 */

struct mli_octree_TmpNode {
        struct mli_octree_TmpNode *children[8];
        uint32_t num_objects;
        uint32_t *objects;

        int32_t flat_index;
        int32_t node_index;
        int32_t leaf_index;
};

int mli_octree_TmpNode_malloc(
        struct mli_octree_TmpNode *n,
        const uint32_t num_objects);
void mli_octree_TmpNode_free(struct mli_octree_TmpNode *n);
struct mli_octree_TmpNode mli_octree_TmpNode_init(void);
void mli_octree_TmpNode_num_nodes_leafs_objects(
        const struct mli_octree_TmpNode *root_node,
        uint64_t *num_nodes,
        uint64_t *num_leafs,
        uint64_t *num_object_links);
void mli_octree_TmpNode_num_nodes_leafs_objects_walk(
        const struct mli_octree_TmpNode *node,
        uint64_t *num_nodes,
        uint64_t *num_leafs,
        uint64_t *num_object_links);
void mli_octree_TmpNode_set_flat_index(struct mli_octree_TmpNode *root_node);
void mli_octree_TmpNode_set_flat_index_walk(
        struct mli_octree_TmpNode *node,
        int32_t *flat_index,
        int32_t *node_index,
        int32_t *leaf_index);
int mli_octree_TmpNode_exists_and_has_objects(
        const struct mli_octree_TmpNode *node);
void mli_octree_TmpNode_print(
        const struct mli_octree_TmpNode *node,
        const uint32_t indent,
        const uint32_t child);
int mli_octree_TmpNode_num_children(const struct mli_octree_TmpNode *node);
int mli_octree_TmpNode_malloc_tree_from_bundle(
        struct mli_octree_TmpNode *root_node,
        const void *bundle,
        const uint32_t num_items_in_bundle,
        int (*item_in_bundle_has_overlap_aabb)(
                const void *,
                const uint32_t,
                const struct mli_AABB),
        const struct mliCube bundle_cube);
int mli_octree_TmpNode_add_children(
        struct mli_octree_TmpNode *node,
        const void *bundle,
        int (*item_in_bundle_has_overlap_aabb)(
                const void *,
                const uint32_t,
                const struct mli_AABB),
        const struct mliCube cube,
        const uint64_t depth,
        const uint64_t max_depth);
uint32_t mli_octree_TmpNode_signs_to_child(
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz);

/*
 * The dynamic octree
 * ==================
 */

struct mli_octree_TmpOcTree {
        struct mliCube cube;
        struct mli_octree_TmpNode root;
};

int mli_octree_TmpOcTree_malloc_from_bundle(
        struct mli_octree_TmpOcTree *octree,
        const void *bundle,
        const uint32_t num_items_in_bundle,
        int (*item_in_bundle_has_overlap_aabb)(
                const void *,
                const uint32_t,
                const struct mli_AABB),
        struct mli_AABB bundle_aabb);

void mli_octree_TmpOcTree_free(struct mli_octree_TmpOcTree *octree);
struct mli_octree_TmpOcTree mli_octree_TmpOcTree_init(void);
void mli_octree_TmpOcTree_print(const struct mli_octree_TmpOcTree *octree);

#endif
