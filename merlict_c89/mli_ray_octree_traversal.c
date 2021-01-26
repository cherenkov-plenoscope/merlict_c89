/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_ray_octree_traversal.h"

/*
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

int _mli_first_node(
        const struct mliVec t0,
        const struct mliVec tm)
{
        uint8_t child = 0;
        if (t0.x > t0.y) {
                if (t0.x > t0.z) {
                        /* Y-Z-plane */
                        if (tm.y < t0.x)
                                child |= 2;
                        if (tm.z < t0.x)
                                child |= 1;
                        return (int)child;
                }
        } else {
                if (t0.y > t0.z) {
                        /* X-Z-plane */
                        if (tm.x < t0.y)
                                child |= 4;
                        if (tm.z < t0.y)
                                child |= 1;
                        return (int)child;
                }
        }
        /* X-Y-plane */
        if (tm.x < t0.z)
                child |= 4;
        if (tm.y < t0.z)
                child |= 2;
        return (int)child;
}

int _mli_new_node(double txm, int x, double tym, int y, double tzm, int z)
{
        if (txm < tym) {
                if (txm < tzm) {
                        /* Y-Z-plane */
                        return x;
                }
        } else {
                if (tym < tzm) {
                        /* X-Z-plane */
                        return y;
                }
        }
        return z; /* X-Y-plane */
}

void mli_set_txm_tym_tzm(
        const struct mliVec t0,
        const struct mliVec t1,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliVec *tm)
{
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
        tm->x = 0.5 * (t0.x + t1.x);
        if (MLI_IS_NAN(tm->x)) {
                const struct mliVec cube_upper = mliCube_upper(cube);
                const double x0 = cube.lower.x;
                const double x1 = cube_upper.x;
                if (ray_octree_support.x < (x0 + x1) * .5)
                        tm->x = DBL_MAX;
                else
                        tm->x = -DBL_MAX;
        }

        tm->y = 0.5 * (t0.y + t1.y);
        if (MLI_IS_NAN(tm->y)) {
                const struct mliVec cube_upper = mliCube_upper(cube);
                const double y0 = cube.lower.y;
                const double y1 = cube_upper.y;
                if (ray_octree_support.y < (y0 + y1) * .5)
                        tm->y = DBL_MAX;
                else
                        tm->y = -DBL_MAX;
        }

        tm->z = 0.5 * (t0.z + t1.z);
        if (MLI_IS_NAN(tm->z)) {
                const struct mliVec cube_upper = mliCube_upper(cube);
                const double z0 = cube.lower.z;
                const double z1 = cube_upper.z;
                if (ray_octree_support.z < (z0 + z1) * .5)
                        tm->z = DBL_MAX;
                else
                        tm->z = -DBL_MAX;
        }
}

void _mli_proc_subtree(
        double tx0,
        double ty0,
        double tz0,
        double tx1,
        double ty1,
        double tz1,
        const struct mliOcTree *octree,
        const int32_t octree_node,
        const int32_t octree_type,
        uint8_t a,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliIntersection *isec,
        const struct mliScenery *scenery,
        const struct mliRay ray,
        uint64_t *num_hits)
{
        struct mliVec tm;
        int currNode;

        if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
                return;
        }

        if (octree_type == MLI_OCTREE_TYPE_NONE) {
                return;
        }

        if (octree_type == MLI_OCTREE_TYPE_LEAF) {
                struct mliIntersection tmp_isec;
                uint64_t o;
                for (o = 0; o < mliOcTree_leaf_num_objects(octree, octree_node);
                     o++) {
                        /*
                        uint32_t object_idx = mliOcTree_leaf_object_link(
                                octree, octree_node, o);
                        */

                        int object_has_intersection = 1;
/*
                        mliScenery_intersection(
                                scenery, ray, object_idx, &tmp_isec);
*/

                        tmp_isec.object_idx = 0u;
                        tmp_isec.distance_of_ray = 0.0;
                        if (object_has_intersection) {
                                (*num_hits) += 1u;
                                if (tmp_isec.distance_of_ray <
                                    isec->distance_of_ray) {
                                        (*isec) = tmp_isec;
                                }
                        }
                }
                return;
        }

        mli_set_txm_tym_tzm(
                mliVec_set(tx0, ty0, tz0),
                mliVec_set(tx1, ty1, tz1),
                cube,
                ray_octree_support,
                &tm);

        currNode = _mli_first_node(mliVec_set(tx0, ty0, tz0), tm);
        do {
                switch (currNode) {
                case 0: {
                        _mli_proc_subtree(
                                tx0,
                                ty0,
                                tz0,
                                tm.x,
                                tm.y,
                                tm.z,
                                octree,
                                octree->nodes[octree_node].children[a],
                                octree->nodes[octree_node].types[a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tm.x, 4, tm.y, 2, tm.z, 1);
                        break;
                }
                case 1: {
                        _mli_proc_subtree(
                                tx0,
                                ty0,
                                tm.z,
                                tm.x,
                                tm.y,
                                tz1,
                                octree,
                                octree->nodes[octree_node].children[1 ^ a],
                                octree->nodes[octree_node].types[1 ^ a],
                                a,
                                cube,
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tm.x, 5, tm.y, 3, tm.z, 8);
                        break;
                }
                case 2: {
                        _mli_proc_subtree(
                                tx0,
                                tm.y,
                                tz0,
                                tm.x,
                                ty1,
                                tm.z,
                                octree,
                                octree->nodes[octree_node].children[2 ^ a],
                                octree->nodes[octree_node].types[2 ^ a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tm.x, 6, ty1, 8, tm.z, 3);
                        break;
                }
                case 3: {
                        _mli_proc_subtree(
                                tx0,
                                tm.y,
                                tm.z,
                                tm.x,
                                ty1,
                                tz1,
                                octree,
                                octree->nodes[octree_node].children[3 ^ a],
                                octree->nodes[octree_node].types[3 ^ a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tm.x, 7, ty1, 8, tz1, 8);
                        break;
                }
                case 4: {
                        _mli_proc_subtree(
                                tm.x,
                                ty0,
                                tz0,
                                tx1,
                                tm.y,
                                tm.z,
                                octree,
                                octree->nodes[octree_node].children[4 ^ a],
                                octree->nodes[octree_node].types[4 ^ a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tx1, 8, tm.y, 6, tm.z, 5);
                        break;
                }
                case 5: {
                        _mli_proc_subtree(
                                tm.x,
                                ty0,
                                tm.z,
                                tx1,
                                tm.y,
                                tz1,
                                octree,
                                octree->nodes[octree_node].children[5 ^ a],
                                octree->nodes[octree_node].types[5 ^ a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tx1, 8, tm.y, 7, tz1, 8);
                        break;
                }
                case 6: {
                        _mli_proc_subtree(
                                tm.x,
                                tm.y,
                                tz0,
                                tx1,
                                ty1,
                                tm.z,
                                octree,
                                octree->nodes[octree_node].children[6 ^ a],
                                octree->nodes[octree_node].types[6 ^ a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = _mli_new_node(tx1, 8, ty1, 8, tm.z, 7);
                        break;
                }
                case 7: {
                        _mli_proc_subtree(
                                tm.x,
                                tm.y,
                                tm.z,
                                tx1,
                                ty1,
                                tz1,
                                octree,
                                octree->nodes[octree_node].children[7 ^ a],
                                octree->nodes[octree_node].types[7 ^ a],
                                a,
                                mliCube_octree_child_code(cube, a),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray,
                                num_hits);
                        currNode = 8;
                        break;
                }
                }
        } while (currNode < 8);
}

void mli_ray_octree_traversal(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mliRay ray,
        struct mliIntersection *isec)
{
        uint64_t num_hits;
        double divx, divy, divz;
        double tx0, tx1, ty0, ty1, tz0, tz1;
        struct mliRay ray_octree;
        struct mliVec cube_upper, cube_size;
        struct mliCube cube;
        int32_t octree_root_node, octree_root_type;
        uint8_t a = 0;
        isec->distance_of_ray = DBL_MAX;
        cube = octree->cube;
        cube_upper = mliCube_upper(cube);
        octree_root_node = 0u;
        octree_root_type = octree->root_type;
        ray_octree = ray;
        num_hits = 0u;
        cube_size = mliVec_add(cube.lower, cube_upper);

        if (ray_octree.direction.x < 0) {
                ray_octree.support.x = -ray_octree.support.x + cube_size.x;
                ray_octree.direction.x = -ray_octree.direction.x;
                a |= 4;
        }
        if (ray_octree.direction.y < 0) {
                ray_octree.support.y = -ray_octree.support.y + cube_size.y;
                ray_octree.direction.y = -ray_octree.direction.y;
                a |= 2;
        }
        if (ray_octree.direction.z < 0) {
                ray_octree.support.z = -ray_octree.support.z + cube_size.z;
                ray_octree.direction.z = -ray_octree.direction.z;
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
                _mli_proc_subtree(
                        tx0,
                        ty0,
                        tz0,
                        tx1,
                        ty1,
                        tz1,
                        octree,
                        octree_root_node,
                        octree_root_type,
                        a,
                        cube,
                        ray_octree.support,
                        isec,
                        scenery,
                        ray,
                        &num_hits);
        }
}
