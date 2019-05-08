/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTTREE_H_
#define MERLICT_MLIOCTTREE_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliOBB.h"
#include "mliCube.h"
#include "mliMath.h"


typedef struct mliNode {
    struct mliNode* mother;
    struct mliNode* children[8];
    uint32_t i;
    uint32_t num_objects;
    uint32_t *objects;
} mliNode;


void mliNode_init(mliNode *n) {
    int c;
    n->mother = NULL;
    for (c = 0; c < 8u; c++) {
        n->children[c] = NULL;}
    n->num_objects = 0u;
    n->i = 0u;
    n->objects = NULL;}


int mliNode_is_blank(const mliNode *n) {
    int c;
    if (n->mother != NULL)
        return 0;
    for (c = 0; c < 8u; c++)
        if (n->children[c] != NULL)
            return 0;
    if (n->num_objects != 0u)
        return 0;
    return 1;}


void mliNode_free(mliNode *n) {
    uint32_t c;
    for (c = 0; c < 8u; c++)
        if (n->children[c] != NULL)
            mliNode_free(n->children[c]);
    if (n->num_objects)
        free(n->objects);}


int mliNode_num_children(const mliNode *node) {
    uint32_t c;
    uint32_t num_leafs = 0u;
    for (c = 0u; c < 8u; c++)
        if (node->children[c] != NULL)
            num_leafs++;
    return num_leafs;}


uint32_t mliNode_signs_to_child(
    const uint32_t sx,
    const uint32_t sy,
    const uint32_t sz) {
    return 4*sx + 2*sy + 1*sz;}

void mliNode_add_children(
    mliNode *node,
    const mliScenery *scenery,
    const mliCube cube_bound) {
    int make_children;
    uint32_t c;
    uint32_t sx, sy, sz, obj;
    uint32_t child;
    mliCube child_cube_bounds[8];
    uint32_t num_ovrerlaps[8];
    uint32_t **overlapping_objects;

    if (node->num_objects <= 32u) {
        return;
    }

    overlapping_objects = malloc(8u*sizeof(uint32_t*));
    for (c = 0; c < 8u; c++) {
        overlapping_objects[c] = malloc(node->num_objects*sizeof(uint32_t));}

    /* sense possible children */
    for (sx = 0u; sx < 2u; sx++) {
        for (sy = 0u; sy < 2u; sy++) {
            for (sz = 0u; sz < 2u; sz++) {
                child = mliNode_signs_to_child(sx, sy, sz);
                child_cube_bounds[child] = mliCube_octree_child(cube_bound, sx, sy, sz);
                num_ovrerlaps[child] = 0u;
                for (obj = 0u; obj < node->num_objects; obj++) {
                    uint32_t obj_idx = node->objects[obj];
                    if (mliScenery_overlap_obb(
                        scenery,
                        obj_idx,
                        mliCube_to_obb(child_cube_bounds[child]))
                    ) {
                        overlapping_objects[child][num_ovrerlaps[child]] = obj_idx;
                        num_ovrerlaps[child]++;
                    }
                }
            }
        }
    }

    make_children = 0;
    for (c = 0; c < 8u; c++) {
        if (num_ovrerlaps[c] < node->num_objects/4) {
            make_children += 1;}}

    if (make_children) {
        for (c = 0; c < 8u; c++) {
            node->children[c] = (mliNode*)malloc(sizeof(mliNode));
            mliNode_init(node->children[c]);
            node->children[c]->mother = node;
            node->children[c]->num_objects = num_ovrerlaps[c];
            node->children[c]->i = c;
            node->children[c]->objects =
                (uint32_t*)malloc(num_ovrerlaps[c]*sizeof(uint32_t));
            mli_uint32_ncpy(
                overlapping_objects[c],
                node->children[c]->objects,
                num_ovrerlaps[c]);
        }
    }

    for (c = 0; c < 8u; c++) {
        free(overlapping_objects[c]);}
    free(overlapping_objects);

    if (make_children) {
        for (c = 0; c < 8u; c++) {
            mliNode_add_children(
                node->children[c],
                scenery,
                child_cube_bounds[c]);
        }
    }
}

mliNode mliNode_from_scenery(const mliScenery *scenery, const mliCube scenery_cube) {
    mliNode tree;
    uint32_t idx;
    mliNode_init(&tree);
    tree.mother = NULL;
    tree.num_objects = mliScenery_num_objects(scenery);
    tree.objects = (uint32_t*)malloc(tree.num_objects*sizeof(uint32_t));
    for (idx = 0; idx < tree.num_objects; idx++) {
        tree.objects[idx] = idx;}
    mliNode_add_children(&tree, scenery, scenery_cube);
    return tree;
}


void __mliNode_num_nodes_recursive(const mliNode *node, uint32_t *num_nodes) {
    uint32_t c;
    (*num_nodes)++;
    for (c = 0u; c < 8u; c++) {
        if (node->children[c] != NULL) {
            __mliNode_num_nodes_recursive(node->children[c], num_nodes);}}}


uint32_t mliNode_num_nodes(const mliNode *node) {
    uint32_t num_nodes = 0u;
    __mliNode_num_nodes_recursive(node, &num_nodes);
    return num_nodes;}


void __mliNode_capacity_objects_recursive(
    const mliNode *node,
    uint32_t *capacity_objects) {
    if (mliNode_num_children(node) == 0u) {
        (*capacity_objects) += node->num_objects;
    } else {
        uint32_t c;
        for (c = 0u; c < 8u; c++) {
            if (node->children[c] != NULL) {
                __mliNode_capacity_objects_recursive(
                    node->children[c],
                    capacity_objects);}}}}


uint32_t mliNode_capacity_objects(const mliNode *node) {
    uint32_t capacity_objects = 0u;
    __mliNode_capacity_objects_recursive(node, &capacity_objects);
    return capacity_objects;}


void mliNode_print(const mliNode *node, const uint32_t indent) {
    uint32_t i;
    uint32_t num_c = mliNode_num_children(node);
    for (i = 0u; i < indent; i++) printf(" ");
    if (num_c == 0)
        printf("|-Leaf %u: overlaps: %u", node->i, node->num_objects);
    else
        printf("Node %u: num_children %u", node->i, num_c);
    printf("\n");
    for (i = 0u; i < 8u; i++) {
        if (node->children[i] != NULL) {
            mliNode_print(node->children[i], indent + 2);
        }
    }
}

typedef struct {
    mliCube cube;
    mliNode root;
} mliOcTree;

void mliOcTree_free(mliOcTree *octree) {
    mliNode_free(&octree->root);}

mliOcTree mliOcTree_from_scenery(const mliScenery *scenery) {
    mliOcTree octree;
    octree.cube = mliCube_outermost_cube(
            mliScenery_outermost_obb(scenery));
    octree.root = mliNode_from_scenery(
        scenery,
        octree.cube);
    return octree;}

#endif
