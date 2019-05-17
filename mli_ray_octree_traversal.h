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
#include "mliMath.h"


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

void mli_set_txm_tym_tzm(
    double tx0,
    double ty0,
    double tz0,
    double tx1,
    double ty1,
    double tz1,
    const mliCube cube,
    const mliVec ray_octree_support,
    double *txm,
    double *tym,
    double *tzm) {
    /*
     * This implements Section 3.3
     * "Generalising for Rays Parallel to One Main Axis"
     * in
     *  @article{revelles2000efficient,
     *      title={An efficient parametric algorithm for octree traversal},
     *      author={Revelles, Jorge and Urena, Carlos and Lastra, Miguel},
     *      year={2000},
     *      publisher={V{\'a}clav Skala-UNION Agency}
     *  }
     */
    (*txm) = 0.5*(tx0 + tx1);
    if (MLI_IS_NAN(*txm)) {
        const mliVec cube_upper = mliCube_upper(cube);
        const double x0 = cube.lower.x;
        const double x1 = cube_upper.x;
        if ( ray_octree_support.x < (x0 + x1)*.5 )
            (*txm) = DBL_MAX;
        else
            (*txm) = -DBL_MAX;
    }

    (*tym) = 0.5*(ty0 + ty1);
    if (MLI_IS_NAN(*tym)) {
        const mliVec cube_upper = mliCube_upper(cube);
        const double y0 = cube.lower.y;
        const double y1 = cube_upper.y;
        if ( ray_octree_support.y < (y0 + y1)*.5 )
            (*tym) = DBL_MAX;
        else
            (*tym) = -DBL_MAX;
    }

    (*tzm) = 0.5*(tz0 + tz1);
    if (MLI_IS_NAN(*tzm)) {
        const mliVec cube_upper = mliCube_upper(cube);
        const double z0 = cube.lower.z;
        const double z1 = cube_upper.z;
        if ( ray_octree_support.z < (z0 + z1)*.5 )
            (*tzm) = DBL_MAX;
        else
            (*tzm) = -DBL_MAX;
    }
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
    const mliCube cube,
    const mliVec ray_octree_support,
    mliIntersection* isec,
    const mliScenery* scenery,
    const mliRay ray,
    uint64_t *num_hits) {
    double txm, tym, tzm;
    int currNode;

    if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
        return;
    }

    if (mliNode_num_children(node) == 0u) {
        mliIntersection tmp_isec;
        uint64_t c;
        for (c = 0; c < node->num_objects; c++) {
            uint64_t object_idx = node->objects[c];
            int object_has_intersection = mliScenery_intersection(
                scenery,
                ray,
                object_idx,
                &tmp_isec);

            if (object_has_intersection) {
                (*num_hits) += 1u;
                if (tmp_isec.distance_of_ray < isec->distance_of_ray) {
                    (*isec) = tmp_isec;
                }
            }
        }
        return;
    }

    mli_set_txm_tym_tzm(
        tx0, ty0, tz0, tx1, ty1, tz1,
        cube, ray_octree_support,
        &txm, &tym, &tzm);

    currNode = __mli_first_node(tx0,ty0,tz0,txm,tym,tzm);
    do{
        switch (currNode)
        {
        case 0: {
            __mli_proc_subtree(
                tx0,ty0,tz0,txm,tym,tzm,
                node->children[a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(txm,4,tym,2,tzm,1);
            break;}
        case 1: {
            __mli_proc_subtree(
                tx0,ty0,tzm,txm,tym,tz1,
                node->children[1^a], a, cube, ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(txm,5,tym,3,tz1,8);
            break;}
        case 2: {
            __mli_proc_subtree(
                tx0,tym,tz0,txm,ty1,tzm,
                node->children[2^a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(txm,6,ty1,8,tzm,3);
            break;}
        case 3: {
            __mli_proc_subtree(
                tx0,tym,tzm,txm,ty1,tz1,
                node->children[3^a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(txm,7,ty1,8,tz1,8);
            break;}
        case 4: {
            __mli_proc_subtree(
                txm,ty0,tz0,tx1,tym,tzm,
                node->children[4^a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(tx1,8,tym,6,tzm,5);
            break;}
        case 5: {
            __mli_proc_subtree(
                txm,ty0,tzm,tx1,tym,tz1,
                node->children[5^a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(tx1,8,tym,7,tz1,8);
            break;}
        case 6: {
            __mli_proc_subtree(
                txm,tym,tz0,tx1,ty1,tzm,
                node->children[6^a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = __mli_new_node(tx1,8,ty1,8,tzm,7);
            break;}
        case 7: {
            __mli_proc_subtree(
                txm,tym,tzm,tx1,ty1,tz1,
                node->children[7^a], a,
                mliCube_octree_child_code(cube, a), ray_octree_support,
                isec, scenery, ray, num_hits);
            currNode = 8;
            break;}
        }
    } while (currNode<8);
}

void mli_ray_octree_traversal(
    const mliScenery* scenery,
    const mliOcTree* octree,
    const mliRay ray,
    mliIntersection *isec) {
    uint64_t num_hits;
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
    mliVec cube_size;
    mliCube cube;
    const mliNode *root;
    uint8_t a = 0;
    isec->distance_of_ray = FLT_MAX;
    cube = octree->cube;
    cube_upper = mliCube_upper(cube);
    root = &octree->root;
    ray_octree = ray;
    num_hits = 0u;
    cube_size = mliVec_add(cube.lower, cube_upper);

    if (ray_octree.direction.x < 0) {
        ray_octree.support.x = - ray_octree.support.x + cube_size.x;
        ray_octree.direction.x = - ray_octree.direction.x;
        a |= 4;
    }
    if (ray_octree.direction.y < 0) {
        ray_octree.support.y = - ray_octree.support.y + cube_size.y;
        ray_octree.direction.y = - ray_octree.direction.y;
        a |= 2;
    }
    if (ray_octree.direction.z < 0) {
        ray_octree.support.z = - ray_octree.support.z + cube_size.z;
        ray_octree.direction.z = - ray_octree.direction.z;
        a |= 1;
    }

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
            root, a, cube, ray_octree.support,
            isec, scenery, ray, &num_hits);
    }
}

#endif
