/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTTREE_H_
#define MERLICT_MLIOCTTREE_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliOBB.h"
#include "mliCube.h"
#include "mli_math.h"
#include "mliOctOverlaps.h"


struct mliNode {
    struct mliNode* mother;
    struct mliNode* children[8];
    uint32_t num_objects;
    uint32_t *objects;
};


void mliNode_init(struct mliNode *n) {
    uint64_t c;
    n->mother = NULL;
    for (c = 0; c < 8u; c++) {
        n->children[c] = NULL;}
    n->num_objects = 0u;
    n->objects = NULL;}


int mliNode_is_blank(const struct mliNode *n) {
    uint64_t c;
    if (n->mother != NULL)
        return 0;
    for (c = 0; c < 8u; c++)
        if (n->children[c] != NULL)
            return 0;
    if (n->num_objects != 0u)
        return 0;
    return 1;}


void mliNode_free(struct mliNode *n) {
    uint32_t c;
    for (c = 0; c < 8u; c++)
        if (n->children[c] != NULL)
            mliNode_free(n->children[c]);
    if (n->num_objects)
        free(n->objects);}


int mliNode_num_children(const struct mliNode *node) {
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

int mliNode_add_children(
    struct mliNode *node,
    const struct mliScenery *scenery,
    const struct mliCube cube,
    const uint64_t depth,
    const uint64_t max_depth) {
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
                const uint32_t child = mliNode_signs_to_child(sx, sy, sz);
                child_cubes[child] = mliCube_octree_child(cube, sx, sy, sz);
                for (obj = 0u; obj < node->num_objects; obj++) {
                    const uint32_t object_idx = node->objects[obj];
                    if (mliScenery_overlap_obb(
                        scenery,
                        object_idx,
                        mliCube_to_obb(child_cubes[child]))
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
        mli_malloc(node->children[c], struct mliNode, 1u);
        mliNode_init(node->children[c]);
        node->children[c]->mother = node;
        node->children[c]->num_objects = overlap[c].dyn.size;
        mli_malloc(node->children[c]->objects, uint32_t, overlap[c].dyn.size);
        mli_uint32_ncpy(
            overlap[c].arr,
            node->children[c]->objects,
            overlap[c].dyn.size);
    }

    for (c = 0u; c < 8u; c++) {
        mliOctOverlap_free(&overlap[c]);
    }

    for (c = 0; c < 8u; c++) {
        mliNode_add_children(
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

struct mliNode mliNode_from_scenery(
    const struct mliScenery *scenery,
    const struct mliCube scenery_cube) {
    struct mliNode root;
    uint32_t idx;
    uint64_t depth, max_depth;
    depth = 0u;
    mliNode_init(&root);
    root.mother = NULL;
    root.num_objects = mliScenery_num_objects(scenery);
    max_depth = 1u + (uint64_t)ceil(log((double)root.num_objects)/log(8.0));
    root.objects = (uint32_t*)malloc(root.num_objects*sizeof(uint32_t));
    for (idx = 0; idx < root.num_objects; idx++) {
        root.objects[idx] = idx;}
    mliNode_add_children(&root, scenery, scenery_cube, depth, max_depth);
    return root;
}


void __mliNode_num_nodes_recursive(const struct mliNode *node, uint32_t *num_nodes) {
    uint32_t c;
    *num_nodes += 1;
    for (c = 0u; c < 8u; c++) {
        if (node->children[c] != NULL) {
            __mliNode_num_nodes_recursive(node->children[c], num_nodes);
        }
    }
}


uint32_t mliNode_num_nodes(const struct mliNode *node) {
    uint32_t num_nodes = 0u;
    __mliNode_num_nodes_recursive(node, &num_nodes);
    return num_nodes;}


void _mliNode_capacity_nodes(const struct mliNode *node, uint32_t *num_nodes) {
    uint32_t c;
    for (c = 0u; c < 8u; c++) {
        if (node->children[c] != NULL) {
            *num_nodes += 1;
            _mliNode_capacity_nodes(node->children[c], num_nodes);
        }
    }
}


uint32_t mliNode_capacity_nodes(const struct mliNode *node) {
    uint32_t num_nodes = 1u;
    _mliNode_capacity_nodes(node, &num_nodes);
    return num_nodes;}


void __mliNode_capacity_objects_recursive(
    const struct mliNode *node,
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


uint32_t mliNode_capacity_objects(const struct mliNode *node) {
    uint32_t capacity_objects = 0u;
    __mliNode_capacity_objects_recursive(node, &capacity_objects);
    return capacity_objects;}


void mliNode_print(const struct mliNode *node, const uint32_t indent, const uint32_t ch) {
    uint32_t i;
    uint32_t c;
    uint32_t num_c = mliNode_num_children(node);
    for (i = 0u; i < indent; i++) printf(" ");
    if (num_c == 0) {
        uint32_t j;
        printf("|-Leaf %u: %u [", ch, node->num_objects);
        for(j = 0; j < node->num_objects; j++) {
            printf("%u, ", node->objects[j]);
        }
        printf("]");
    } else
        printf("Node: %u", ch);
    printf("\n");
    for (c = 0u; c < 8u; c++) {
        if (node->children[c] != NULL) {
            mliNode_print(node->children[c], indent + 2, c);
        }
    }
}

struct mliOcTree {
    struct mliCube cube;
    struct mliNode root;
};

void mliOcTree_free(struct mliOcTree *octree) {
    mliNode_free(&octree->root);}

struct mliOcTree mliOcTree_from_scenery(const struct mliScenery *scenery) {
    struct mliOcTree octree;
    octree.cube = mliCube_outermost_cube(
            mliScenery_outermost_obb(scenery));
    octree.root = mliNode_from_scenery(
        scenery,
        octree.cube);
    return octree;}

#endif
