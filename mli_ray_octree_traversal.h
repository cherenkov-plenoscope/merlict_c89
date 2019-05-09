/* Copyright 2019 Sebastian Achim Mueller
 *
 *  Based on
 *
 *  @article{revelles2000efficient,
 *      title={An efficient parametric algorithm for octree traversal},
 *      author={Revelles, Jorge and Urena, Carlos and Lastra, Miguel},
 *      year={2000},
 *      publisher={V{\'a}clav Skala-UNION Agency}
 *  }
 *
 *  Profiting from additional comments by Jeroen Baert.
*/
#ifndef MERLICT_MLIRAYOCTREE_H_
#define MERLICT_MLIRAYOCTREE_H_
#include <stdint.h>
#include <float.h>
#include "mliOcTree.h"
#include "mliCube.h"

typedef struct {
    const mliScenery* scenery;
    mliRay ray;
    mliIntersection *intersection;
    uint64_t object_idx_coming_from;
} mliBag;

int __mli_first_node(
    double tx0,
    double ty0,
    double tz0,
    double txm,
    double tym,
    double tzm) {
    uint8_t child = 0;
    if (tx0 > ty0){
        if (tx0 > tz0) {
            /* Y-Z-plane */
            if(tym < tx0) child|=2;
            if(tzm < tx0) child|=1;
            return (int)child;
        }
    }
    else {
        if (ty0 > tz0) {
            /* X-Z-plane */
            if(txm < ty0) child|=4;
            if(tzm < ty0) child|=1;
            return (int)child;
        }
    }
    /* X-Y-plane */
    if (txm < tz0) child|=4;
    if (tym < tz0) child|=2;
    return (int)child;
}

int __mli_new_node(
    double txm,
    int x,
    double tym,
    int y,
    double tzm,
    int z) {
    if(txm < tym){
        if(txm < tzm) {
            /* Y-Z-plane */
            return x;
        }
    }
    else{
        if(tym < tzm) {
            /* X-Z-plane */
            return y;
        }
    }
    return z; /* X-Y-plane */
}

void __mli_proc_subtree (
    double tx0,
    double ty0,
    double tz0,
    double tx1,
    double ty1,
    double tz1,
    const mliNode* node,
    uint8_t a,
    mliIntersection* isec,
    const mliScenery* scenery,
    const mliRay ray,
    const uint64_t object_idx_coming_from) {
    float txm, tym, tzm;
    int currNode;

    if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
        return;
    }

    if (mliNode_num_children(node) == 0u) {
        mliIntersection tmp_isec;
        uint64_t c;
        uint64_t hit = 0;
        /*fprintf(stderr, "Leaf %u\n", node->i);*/
        for (c = 0; c < node->num_objects; c++) {
            uint64_t object_idx = node->objects[c];
            if (object_idx_coming_from == object_idx)
                continue;

            if (mliScenery_intersection(
                    scenery,
                    ray,
                    object_idx,
                    &tmp_isec)
            ) {
                hit += 1u;
                if (tmp_isec.distance_of_ray < isec->distance_of_ray) {
                    (*isec) = tmp_isec;
                }
            }
        }
        if (hit) {
            /*fprintf(stderr, "hits %lu\n", hit);*/
        }
        return;
    } else {
        /*fprintf(stderr, "Node %u\n", node->i);*/
    }

    txm = 0.5*(tx0 + tx1);
    tym = 0.5*(ty0 + ty1);
    tzm = 0.5*(tz0 + tz1);

    currNode = __mli_first_node(tx0,ty0,tz0,txm,tym,tzm);
    do{
        switch (currNode)
        {
        case 0: {
            __mli_proc_subtree(
                tx0,ty0,tz0,txm,tym,tzm,
                node->children[a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(txm,4,tym,2,tzm,1);
            break;}
        case 1: {
            __mli_proc_subtree(
                tx0,ty0,tzm,txm,tym,tz1,
                node->children[1^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(txm,5,tym,3,tz1,8);
            break;}
        case 2: {
            __mli_proc_subtree(
                tx0,tym,tz0,txm,ty1,tzm,
                node->children[2^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(txm,6,ty1,8,tzm,3);
            break;}
        case 3: {
            __mli_proc_subtree(
                tx0,tym,tzm,txm,ty1,tz1,
                node->children[3^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(txm,7,ty1,8,tz1,8);
            break;}
        case 4: {
            __mli_proc_subtree(
                txm,ty0,tz0,tx1,tym,tzm,
                node->children[4^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(tx1,8,tym,6,tzm,5);
            break;}
        case 5: {
            __mli_proc_subtree(
                txm,ty0,tzm,tx1,tym,tz1,
                node->children[5^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(tx1,8,tym,7,tz1,8);
            break;}
        case 6: {
            __mli_proc_subtree(
                txm,tym,tz0,tx1,ty1,tzm,
                node->children[6^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = __mli_new_node(tx1,8,ty1,8,tzm,7);
            break;}
        case 7: {
            __mli_proc_subtree(
                txm,tym,tzm,tx1,ty1,tz1,
                node->children[7^a], a,
                isec, scenery, ray, object_idx_coming_from);
            currNode = 8;
            break;}
        }
    } while (currNode<8);
}

void mli_ray_octree_traversal(
    const mliScenery* scenery,
    const mliOcTree* octree,
    const mliRay ray,
    mliIntersection *isec,
    const uint64_t object_idx_coming_from) {
    double divx;
    double divy;
    double divz;

    double tx0;
    double tx1;
    double ty0;
    double ty1;
    double tz0;
    double tz1;
    mliRay ray_octree;
    mliVec cube_upper;
    mliCube cube;
    const mliNode *root;
    uint8_t a = 0;
    isec->distance_of_ray = FLT_MAX;
    cube = octree->cube;
    cube_upper = mliCube_upper(cube);
    root = &octree->root;
    ray_octree = ray;

    if (ray_octree.direction.x < 0) {
        ray_octree.support.x = - ray_octree.support.x;
        ray_octree.direction.x = - ray_octree.direction.x;
        a |= 4;
    }
    if (ray_octree.direction.y < 0) {
        ray_octree.support.y = - ray_octree.support.y;
        ray_octree.direction.y = - ray_octree.direction.y;
        a |= 2;
    }
    if (ray_octree.direction.z < 0) {
        ray_octree.support.z = - ray_octree.support.z;
        ray_octree.direction.z = - ray_octree.direction.z;
        a |= 1;
    }

    /*fprintf(stderr, "a: %d\n", a);*/
    divx = 1 / ray_octree.direction.x;
    divy = 1 / ray_octree.direction.y;
    divz = 1 / ray_octree.direction.z;

    tx0 = (cube.lower.x - ray_octree.support.x) * divx;
    tx1 = (cube_upper.x - ray_octree.support.x) * divx;
    ty0 = (cube.lower.y - ray_octree.support.y) * divy;
    ty1 = (cube_upper.y - ray_octree.support.y) * divy;
    tz0 = (cube.lower.z - ray_octree.support.z) * divz;
    tz1 = (cube_upper.z - ray_octree.support.z) * divz;

    if (MLI_MAX3(tx0, ty0, tz0) < MLI_MIN3(tx1, ty1, tz1)) {
        __mli_proc_subtree(
            tx0, ty0, tz0, tx1, ty1, tz1,
            root, a,
            isec, scenery, ray, object_idx_coming_from);
    }
}

#endif
