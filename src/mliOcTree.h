/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOCTREE_H_
#define MLIOCTREE_H_

#include <stdint.h>
#include "mliGeometryAndAccelerator.h"
#include "mliTmpOcTree.h"

#define MLI_OCTREE_TYPE_NONE 0
#define MLI_OCTREE_TYPE_NODE 1
#define MLI_OCTREE_TYPE_LEAF 2

struct mliLeafAddress {
        uint32_t first_object_link;
        uint32_t num_object_links;
};

struct mliLeafArray {
        uint64_t num_leafs;
        struct mliLeafAddress *adresses;
        uint64_t num_object_links;
        uint32_t *object_links;
};

struct mliNode {
        uint32_t children[8];
        uint8_t types[8];
};

struct mliOcTree {
        struct mliCube cube;
        uint64_t num_nodes;
        struct mliNode *nodes;
        struct mliLeafArray leafs;
        uint8_t root_type;
};

void mliOcTree_print(const struct mliOcTree *tree);
void mliOcTree_print_walk(
        const struct mliOcTree *tree,
        const int32_t node_idx,
        const uint8_t node_type,
        const uint32_t indent,
        const uint32_t child);
int mliOcTree_equal_payload(
        const struct mliOcTree *tree,
        const struct mliTmpOcTree *tmp_octree);
int mliOcTree_equal_payload_walk(
        const struct mliOcTree *tree,
        const int32_t node_idx,
        const int32_t node_type,
        const struct mliTmpNode *tmp_node);
uint32_t mliOcTree_leaf_object_link(
        const struct mliOcTree *tree,
        const uint64_t leaf,
        const uint64_t object_link);
uint64_t mliOcTree_leaf_num_objects(
        const struct mliOcTree *tree,
        const uint64_t leaf);
uint64_t mliOcTree_node_num_children(
        const struct mliOcTree *tree,
        const uint64_t node_idx);
void mliOcTree_set(struct mliOcTree *tree, const struct mliTmpOcTree *dyntree);
void mliOcTree_set_walk(
        struct mliOcTree *tree,
        const struct mliTmpNode *dynnode,
        uint64_t *object_link_size);
void mliOcTree_set_leaf(
        struct mliOcTree *tree,
        const struct mliTmpNode *dynnode,
        uint64_t *object_link_size);
void mliOcTree_set_node(
        struct mliOcTree *tree,
        const struct mliTmpNode *dynnode);
int mliOcTree_malloc(
        struct mliOcTree *tree,
        const uint64_t num_nodes,
        const uint64_t num_leafs,
        const uint64_t num_object_links);
void mliOcTree_free(struct mliOcTree *tree);
struct mliOcTree mliOcTree_init(void);
struct mliNode mliNode_init(void);
int mliLeafArray_malloc(
        struct mliLeafArray *leafs,
        const uint64_t num_leafs,
        const uint64_t num_object_links);
void mliLeafArray_free(struct mliLeafArray *leafs);
struct mliLeafArray mliLeafArray_init(void);
struct mliLeafAddress mliLeafAddress_init(void);

int mliOcTree_malloc_from_object_wavefront(
        struct mliOcTree *octree,
        const struct mliObject *object);

int mliOcTree_malloc_from_Geometry(
        struct mliOcTree *octree,
        const struct mliGeometryAndAccelerator *accgeo,
        const struct mliAABB outermost_aabb);

#endif
