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

/*
 *                    __ Z
 *        Y            /|
 *         /\         /
 *         |   -----------------
 *         |  /   3   /   7   /|
 *         | /---------------/ |
 *         |/   2   /   6   /|7|
 *         |---------------- | |
 *         |       |       |6|/|
 *         |   2   |   6   | / |
 *         |       |       |/|5|
 *         |-------|-------| | |
 *         |       |       |4|/
 *         |   0   |   4   | /
 *         |       |       |/
 *         L--------------------> X
 *
 *      Figure 1: Labeled Octree (the hidden one has label 1).
 *      cite{revelles2000efficient}
 */

/*  Current  |     Exit-plane
 *  sub-node |
 *  (state)  |  YZ      XZ      XY
 * ----------|----------------------
 *      0    |  4       2       1
 *      1    |  5       3       END
 *      2    |  6       END     3
 *      3    |  7       END     END
 *      4    |  END     6       5
 *      5    |  END     7       END
 *      6    |  END     END     7
 *      7    |  END     END     END
 *
 *      Table 3: State transition.
 *      cite{revelles2000efficient}
 */

#define mli_END 8

int _mli_first_octree_node(
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

int _mli_next_octree_node(const struct mliVec tm, int x, int y, int z)
{
        if (tm.x < tm.y) {
                if (tm.x < tm.z) {
                        /* Y-Z-plane */
                        return x;
                }
        } else {
                if (tm.y < tm.z) {
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
        struct mliVec t0,
        struct mliVec t1,
        const struct mliOcTree *octree,
        const int32_t octree_node,
        const int32_t octree_type,
        uint8_t permutation,
        const struct mliCube cube,
        const struct mliVec ray_octree_support,
        struct mliIntersection *isec,
        const struct mliScenery *scenery,
        const struct mliRay ray)
{
        struct mliVec tm;
        struct mliVec nt0, nt1;
        int currNode;

        if (t1.x < 0 || t1.y < 0 || t1.z < 0) {
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
                                if (tmp_isec.distance_of_ray <
                                    isec->distance_of_ray) {
                                        (*isec) = tmp_isec;
                                }
                        }
                }
                return;
        }

        mli_set_txm_tym_tzm(t0, t1, cube, ray_octree_support, &tm);

        currNode = _mli_first_octree_node(t0, tm);
        do {
                switch (currNode) {
                case 0: {
                        nt0.x = t0.x;
                        nt0.y = t0.y;
                        nt0.z = t0.z;

                        nt1.x = tm.x;
                        nt1.y = tm.y;
                        nt1.z = tm.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[permutation],
                                octree->nodes[octree_node].types[permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = _mli_next_octree_node(nt1, 4, 2, 1);
                        break;
                }
                case 1: {
                        nt0.x = t0.x;
                        nt0.y = t0.y;
                        nt0.z = tm.z;

                        nt1.x = tm.x;
                        nt1.y = tm.y;
                        nt1.z = t1.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[1 ^ permutation],
                                octree->nodes[octree_node].types[1 ^ permutation],
                                permutation,
                                cube,
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = _mli_next_octree_node(nt1, 5, 3, mli_END);
                        break;
                }
                case 2: {
                        nt0.x = t0.x;
                        nt0.y = tm.y;
                        nt0.z = t0.z;

                        nt1.x = tm.x;
                        nt1.y = t1.y;
                        nt1.z = tm.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[2 ^ permutation],
                                octree->nodes[octree_node].types[2 ^ permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = _mli_next_octree_node(nt1, 6, mli_END, 3);
                        break;
                }
                case 3: {
                        nt0.x = t0.x;
                        nt0.y = tm.y;
                        nt0.z = tm.z;

                        nt1.x = tm.x;
                        nt1.y = t1.y;
                        nt1.z = t1.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[3 ^ permutation],
                                octree->nodes[octree_node].types[3 ^ permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);

                        currNode = _mli_next_octree_node(nt1, 7, mli_END, mli_END);
                        break;
                }
                case 4: {
                        nt0.x = tm.x;
                        nt0.y = t0.y;
                        nt0.z = t0.z;

                        nt1.x = t1.x;
                        nt1.y = tm.y;
                        nt1.z = tm.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[4 ^ permutation],
                                octree->nodes[octree_node].types[4 ^ permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = _mli_next_octree_node(nt1, mli_END, 6, 5);
                        break;
                }
                case 5: {
                        nt0.x = tm.x;
                        nt0.y = t0.y;
                        nt0.z = tm.z;

                        nt1.x = t1.x;
                        nt1.y = tm.y;
                        nt1.z = t1.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[5 ^ permutation],
                                octree->nodes[octree_node].types[5 ^ permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = _mli_next_octree_node(nt1, mli_END, 7, mli_END);
                        break;
                }
                case 6: {
                        nt0.x = tm.x;
                        nt0.y = tm.y;
                        nt0.z = t0.z;

                        nt1.x = t1.x;
                        nt1.y = t1.y;
                        nt1.z = tm.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[6 ^ permutation],
                                octree->nodes[octree_node].types[6 ^ permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = _mli_next_octree_node(nt1, mli_END, mli_END, 7);
                        break;
                }
                case 7: {
                        nt0.x = tm.x;
                        nt0.y = tm.y;
                        nt0.z = t0.z;

                        nt1.x = t1.x;
                        nt1.y = t1.y;
                        nt1.z = t1.z;

                        _mli_proc_subtree(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[octree_node].children[7 ^ permutation],
                                octree->nodes[octree_node].types[7 ^ permutation],
                                permutation,
                                mliCube_octree_child_code(cube, permutation),
                                ray_octree_support,
                                isec,
                                scenery,
                                ray);
                        currNode = mli_END;
                        break;
                }
                }
        } while (currNode < mli_END);
}

void mli_ray_octree_traversal(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mliRay ray,
        struct mliIntersection *isec)
{
        struct mliVec div;
        struct mliVec t0, t1;
        struct mliRay ray_wrt_octree;
        struct mliVec cube_upper, cube_size;
        struct mliCube cube;
        int32_t octree_root_node, octree_root_type;
        uint8_t permutation = 0;
        isec->distance_of_ray = DBL_MAX;
        cube = octree->cube;
        cube_upper = mliCube_upper(cube);
        octree_root_node = 0u;
        octree_root_type = octree->root_type;
        ray_wrt_octree = ray;
        cube_size = mliVec_add(cube.lower, cube_upper);

        if (ray_wrt_octree.direction.x < 0) {
                ray_wrt_octree.support.x = -ray_wrt_octree.support.x + cube_size.x;
                ray_wrt_octree.direction.x = -ray_wrt_octree.direction.x;
                permutation |= 4;
        }
        if (ray_wrt_octree.direction.y < 0) {
                ray_wrt_octree.support.y = -ray_wrt_octree.support.y + cube_size.y;
                ray_wrt_octree.direction.y = -ray_wrt_octree.direction.y;
                permutation |= 2;
        }
        if (ray_wrt_octree.direction.z < 0) {
                ray_wrt_octree.support.z = -ray_wrt_octree.support.z + cube_size.z;
                ray_wrt_octree.direction.z = -ray_wrt_octree.direction.z;
                permutation |= 1;
        }

        div.x = 1.0 / ray_wrt_octree.direction.x;
        div.y = 1.0 / ray_wrt_octree.direction.y;
        div.z = 1.0 / ray_wrt_octree.direction.z;

        t0.x = (cube.lower.x - ray_wrt_octree.support.x) * div.x;
        t1.x = (cube_upper.x - ray_wrt_octree.support.x) * div.x;
        t0.y = (cube.lower.y - ray_wrt_octree.support.y) * div.y;
        t1.y = (cube_upper.y - ray_wrt_octree.support.y) * div.y;
        t0.z = (cube.lower.z - ray_wrt_octree.support.z) * div.z;
        t1.z = (cube_upper.z - ray_wrt_octree.support.z) * div.z;

        if (MLI_MAX3(t0.x, t0.y, t0.z) < MLI_MIN3(t1.x, t1.y, t1.z)) {
                _mli_proc_subtree(
                        t0,
                        t1,
                        octree,
                        octree_root_node,
                        octree_root_type,
                        permutation,
                        cube,
                        ray_wrt_octree.support,
                        isec,
                        scenery,
                        ray);
        }
}

void _mli_transform_ray_into_octree_frame(
        const struct mliRay ray_wrt_scenery,
        const struct mliVec octree_cube_size,
        struct mliRay *ray_wrt_octree,
        uint8_t *octree_permutation)
{
        uint8_t permutation = 0u;
        if (ray_wrt_scenery.direction.x < 0.0) {
                ray_wrt_octree->support.x = -ray_wrt_scenery.support.x + octree_cube_size.x;
                ray_wrt_octree->direction.x = -ray_wrt_scenery.direction.x;
                permutation |= 4;
        }
        if (ray_wrt_scenery.direction.y < 0.0) {
                ray_wrt_octree->support.y = -ray_wrt_scenery.support.y + octree_cube_size.y;
                ray_wrt_octree->direction.y = -ray_wrt_scenery.direction.y;
                permutation |= 2;
        }
        if (ray_wrt_scenery.direction.z < 0.0) {
                ray_wrt_octree->support.z = -ray_wrt_scenery.support.z + octree_cube_size.z;
                ray_wrt_octree->direction.z = -ray_wrt_scenery.direction.z;
                permutation |= 1;
        }
        (*octree_permutation) = permutation;
}
