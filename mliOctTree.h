/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTTREE_H_
#define MERLICT_MLIOCTTREE_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliIndex.h"
#include "mliOBB.h"
#include "mliMath.h"

#define MLI_NUM_CHILDREN_OCTTREE 8u

typedef struct mliNode {
    struct mliNode* mother;
    struct mliNode* children[8];
    uint32_t num_objects;
    uint32_t *objects;
} mliNode;

void mliNode_init(mliNode *n) {
    int c;
    n->mother = NULL;
    for (c = 0; c < 8u; c++) {
        n->children[c] = NULL;}
    n->num_objects = 0u;
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

int mliNode_is_leaf(const mliNode *n) {
    return n->num_objects > 0u;}

int mliNode_is_node(const mliNode *n) {
    return !mliNode_is_leaf(n);}

void mliNode_free(mliNode *n) {
    if (mliNode_is_node(n)) {
        uint32_t c;
        for (c = 0; c < 8u; c++) {
            if (n->children[c] != NULL)  {
                mliNode_free(n->children[c]);
            }
        }
    } else {
        free(n->objects);
    }
}

uint32_t mliNode_signs_to_child(
    const uint32_t sx,
    const uint32_t sy,
    const uint32_t sz) {
    return 4*sx + 2*sy + 1*sz;}

void mliNode_child_to_signs(
    const uint32_t child,
    uint32_t *sx,
    uint32_t *sy,
    uint32_t *sz) {
    uint32_t rest;
    (*sx) = child / 4;
    rest = child - (*sx);
    (*sy) = rest / 2;
    rest = rest - (*sy);
    (*sz) = rest;}

void mliNode_add_children(
    mliNode *node,
    const mliScenery *scenery,
    const mliOBB obb) {
    uint32_t sx, sy, sz, obj, ovrerlaps_child;
    uint32_t child;
    uint32_t *overlapping_objects;
    overlapping_objects =
        (uint32_t*)malloc(node->num_objects*sizeof(uint32_t));
    for (sx = 0u; sx < 2u; sx++) {
        for (sy = 0u; sy < 2u; sy++) {
            for (sz = 0u; sz < 2u; sz++) {
                mliOBB child_obb;
                child = mliNode_signs_to_child(sx, sy, sz);
                child_obb = mliOBB_octtree_child(obb, sx, sy, sz);
                ovrerlaps_child = 0u;
                for (obj = 0u; obj < node->num_objects; obj++) {
                    uint32_t obj_idx = node->objects[obj];
                    if (mliScenery_overlap_obb(scenery, obj_idx, child_obb)) {
                        overlapping_objects[ovrerlaps_child++] = obj_idx;
                    }
                }

                /*fprintf(stderr, "ovrerlaps_child %u\n", ovrerlaps_child);*/
                if (ovrerlaps_child == node->num_objects || ovrerlaps_child  <= 16u) {
                    /* No point in creating this child node */
                } else {
                    node->children[child] = (mliNode*)malloc(sizeof(mliNode));
                    mliNode_init(node->children[child]);
                    node->children[child]->mother = node;
                    node->children[child]->num_objects = ovrerlaps_child;
                    node->children[child]->objects =
                        (uint32_t*)malloc(ovrerlaps_child*sizeof(uint32_t));
                    mli_uint32_ncpy(
                        overlapping_objects,
                        node->children[child]->objects,
                        ovrerlaps_child);
                    mliNode_add_children(
                        node->children[child],
                        scenery,
                        child_obb);
                }
            }
        }
    }
    free(overlapping_objects);
}

void mliNode_for_scenery(mliNode *tree, const mliScenery *scenery) {
    mliOBB cube_obb;
    mliOBB scenery_obb;
    uint32_t idx;
    assert(mliNode_is_blank(tree));
    scenery_obb = mliScenery_outermost_obb(scenery);
    cube_obb = mliOBB_outer_cube(scenery_obb);
    tree->mother = NULL;
    tree->num_objects = mliScenery_num_entities(scenery);
    tree->objects = (uint32_t*)malloc(tree->num_objects*sizeof(uint32_t));
    for (idx = 0; idx < tree->num_objects; idx++) {
        tree->objects[idx] = idx;}
    mliNode_add_children(tree, scenery, cube_obb);

}

void mliNode_print(const mliNode *node, const uint32_t indent) {
    uint32_t i;
    uint32_t num_c = 0u;
    for (i = 0u; i < indent; i++) printf(" ");
    for (i = 0u; i < 8u; i++) if (node->children[i] != NULL) num_c++;
    if (num_c == 0)
        printf("|-Leaf: overlaps: %u", node->num_objects);
    else
        printf("Node: num_children %u", num_c);
    printf("\n");
    for (i = 0u; i < 8u; i++) {
        if (node->children[i] != NULL) {
            mliNode_print(node->children[i], indent + 2);
        }
    }
}

#endif
