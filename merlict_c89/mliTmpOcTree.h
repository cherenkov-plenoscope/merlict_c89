/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITMPOCTREE_H_
#define MERLICT_C89_MLITMPOCTREE_H_

#include <stdint.h>

#include "mliScenery.h"
#include "mliOBB.h"
#include "mliCube.h"
#include "mli_math.h"
#include "mliOctOverlaps.h"

#define MLI_TMPNODE_FLAT_INDEX_NONE -1

struct mliTmpNode {
        struct mliTmpNode *children[8];
        uint32_t num_objects;
        uint32_t *objects;

        int32_t flat_index;
        int32_t node_index;
        int32_t leaf_index;
};

struct mliTmpOcTree {
        struct mliCube cube;
        struct mliTmpNode root;
};

int mliTmpOcTree_malloc_from_scenery(
        struct mliTmpOcTree *octree,
        const struct mliScenery *scenery);
void mliTmpOcTree_free(struct mliTmpOcTree *octree);
struct mliTmpOcTree mliTmpOcTree_init(void);
void mliTmpNode_num_nodes_leafs_objects(
        const struct mliTmpNode *root_node,
        uint64_t *num_nodes,
        uint64_t *num_leafs,
        uint64_t *num_object_links);
void _mliTmpNode_num_nodes_leafs_objects(
        const struct mliTmpNode *node,
        uint64_t *num_nodes,
        uint64_t *num_leafs,
        uint64_t *num_object_links);
void mliTmpNode_set_flat_index(struct mliTmpNode *root_node);
void _mliTmpNode_set_flat_index(
        struct mliTmpNode *node,
        int32_t *flat_index,
        int32_t *node_index,
        int32_t *leaf_index);
int _mliTmpNode_exists_and_objects(const struct mliTmpNode *node);
void mliTmpNode_print(
        const struct mliTmpNode *node,
        const uint32_t indent,
        const uint32_t child);
int mliTmpNode_num_children(const struct mliTmpNode *node);
int mliTmpNode_malloc_tree_from_scenery(
        struct mliTmpNode *root_node,
        const struct mliScenery *scenery,
        const struct mliCube scenery_cube);
uint64_t mli_guess_octree_depth_based_on_num_objects(
        const uint64_t num_objects);
int mliTmpNode_add_children(
        struct mliTmpNode *node,
        const struct mliScenery *scenery,
        const struct mliCube cube,
        const uint64_t depth,
        const uint64_t max_depth);
uint32_t mliTmpNode_signs_to_child(
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz);
int mliTmpNode_malloc(struct mliTmpNode *n, const uint32_t num_objects);
void mliTmpNode_free(struct mliTmpNode *n);
struct mliTmpNode mliTmpNode_init(void);
#endif
