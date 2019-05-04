/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTTREE_H_
#define MERLICT_MLIOCTTREE_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliIndex.h"
#include "mliOBB.h"
#include "mliMath.h"

#define MLI_NUM_CHILDREN_OCTTREE 8u

typedef struct {
    uint32_t num_nodes;
    int32_t *node_string;

    uint32_t num_leafs;
    uint32_t *leafs;
    uint32_t *leaf_string;
} mliOctTree;

void mliOctTree_malloc(mliOctTree* tree) {
    tree->node_string = (int32_t*)malloc(
        tree->num_nodes*MLI_NUM_CHILDREN_OCTTREE*sizeof(int32_t));}

void mliOctTree_free(mliOctTree* tree) {
    free(tree->node_string);
    tree->node_string = NULL;
    tree->num_nodes = 0u;}

uint32_t __mliOctTree_node_idx(
    const mliOctTree* tree,
    const uint32_t node,
    const uint32_t child) {
    return node*MLI_NUM_CHILDREN_OCTTREE + child;}

int32_t mliOctTree_at_node_child(
    const mliOctTree* tree,
    const uint32_t node,
    const uint32_t child) {
    uint32_t idx = __mliOctTree_node_idx(tree, node, child);
    return tree->node_string[idx];}

void mliOctTree_set_node_child(
    mliOctTree* tree,
    const uint32_t node,
    const uint32_t child,
    const int32_t child_node) {
    uint32_t idx = __mliOctTree_node_idx(tree, node, child);
    tree->node_string[idx] = child_node;}

#endif
