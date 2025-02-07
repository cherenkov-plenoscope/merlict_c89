/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OCTREE_H_
#define MLI_OCTREE_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "octree_tmp.h"

enum mli_octree_type {
        MLI_OCTREE_TYPE_NONE = 0,
        MLI_OCTREE_TYPE_NODE = 1,
        MLI_OCTREE_TYPE_LEAF = 2
};

struct mli_octree_LeafAddress {
        uint32_t first_object_link;
        uint32_t num_object_links;
};

struct mli_octree_LeafArray {
        uint64_t num_leafs;
        struct mli_octree_LeafAddress *adresses;
        uint64_t num_object_links;
        uint32_t *object_links;
};

struct mli_octree_Node {
        uint32_t children[8];
        uint8_t types[8];
};

struct mli_OcTree {
        struct mli_Cube cube;
        uint64_t num_nodes;
        struct mli_octree_Node *nodes;
        struct mli_octree_LeafArray leafs;
        uint8_t root_type;
};

void mli_OcTree_print(const struct mli_OcTree *tree);
void mli_OcTree_print_walk(
        const struct mli_OcTree *tree,
        const int32_t node_idx,
        const uint8_t node_type,
        const uint32_t indent,
        const uint32_t child);
mli_bool mli_OcTree_equal_payload(
        const struct mli_OcTree *tree,
        const struct mli_octree_TmpOcTree *tmp_octree);
mli_bool mli_OcTree_equal_payload_walk(
        const struct mli_OcTree *tree,
        const int32_t node_idx,
        const int32_t node_type,
        const struct mli_octree_TmpNode *tmp_node);
uint32_t mli_OcTree_leaf_object_link(
        const struct mli_OcTree *tree,
        const uint64_t leaf,
        const uint64_t object_link);
uint64_t mli_OcTree_leaf_num_objects(
        const struct mli_OcTree *tree,
        const uint64_t leaf);
uint64_t mli_OcTree_node_num_children(
        const struct mli_OcTree *tree,
        const uint64_t node_idx);
void mli_OcTree_set(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpOcTree *dyntree);
void mli_OcTree_set_walk(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpNode *dynnode,
        uint64_t *object_link_size);
void mli_OcTree_set_leaf(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpNode *dynnode,
        uint64_t *object_link_size);
void mli_OcTree_set_node(
        struct mli_OcTree *tree,
        const struct mli_octree_TmpNode *dynnode);
chk_rc mli_OcTree_malloc(
        struct mli_OcTree *tree,
        const uint64_t num_nodes,
        const uint64_t num_leafs,
        const uint64_t num_object_links);
void mli_OcTree_free(struct mli_OcTree *tree);
struct mli_OcTree mli_OcTree_init(void);
struct mli_octree_Node mli_octree_Node_init(void);
chk_rc mli_octree_LeafArray_malloc(
        struct mli_octree_LeafArray *leafs,
        const uint64_t num_leafs,
        const uint64_t num_object_links);
void mli_octree_LeafArray_free(struct mli_octree_LeafArray *leafs);
struct mli_octree_LeafArray mli_octree_LeafArray_init(void);
struct mli_octree_LeafAddress mli_octree_LeafAddress_init(void);

chk_rc mli_OcTree_malloc_from_object_wavefront(
        struct mli_OcTree *octree,
        const struct mli_Object *object);

struct mli_GeometryAndAccelerator;
chk_rc mli_OcTree_malloc_from_Geometry(
        struct mli_OcTree *octree,
        const struct mli_GeometryAndAccelerator *accgeo,
        const struct mli_AABB outermost_aabb);

#endif
