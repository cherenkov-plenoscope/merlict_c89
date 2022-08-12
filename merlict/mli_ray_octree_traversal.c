/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_ray_octree_traversal.h"
#include <float.h>

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

int mli_ray_octree_traversal_first_octree_node(
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

int mli_ray_octree_traversal_next_octree_node(
        const struct mliVec tm,
        int x,
        int y,
        int z)
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

void mli_ray_octree_traversal_sub(
        struct mliVec t0,
        struct mliVec t1,
        const struct mliOcTree *octree,
        const int32_t node_idx,
        const int32_t node_type,
        uint8_t permutation,
        void *work,
        void (*work_on_leaf_node)(
                void *,
                const struct mliOcTree *,
                const uint32_t))
{
        int32_t proc_node;
        struct mliVec tm, nt0, nt1;

        if (t1.x < 0 || t1.y < 0 || t1.z < 0) {
                return;
        }

        if (node_type == MLI_OCTREE_TYPE_NONE) {
                return;
        }

        if (node_type == MLI_OCTREE_TYPE_LEAF) {
                /* callback */
                work_on_leaf_node(work, octree, node_idx);
                return;
        }

        tm.x = 0.5 * (t0.x + t1.x);
        tm.y = 0.5 * (t0.y + t1.y);
        tm.z = 0.5 * (t0.z + t1.z);

        proc_node = mli_ray_octree_traversal_first_octree_node(t0, tm);

        do {
                switch (proc_node) {
                case 0: {
                        nt0 = mliVec_init(t0.x, t0.y, t0.z);
                        nt1 = mliVec_init(tm.x, tm.y, tm.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx].children[permutation],
                                octree->nodes[node_idx].types[permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 4, 2, 1);
                        break;
                }
                case 1: {
                        nt0 = mliVec_init(t0.x, t0.y, tm.z);
                        nt1 = mliVec_init(tm.x, tm.y, t1.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[1 ^ permutation],
                                octree->nodes[node_idx].types[1 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 5, 3, 8);
                        break;
                }
                case 2: {
                        nt0 = mliVec_init(t0.x, tm.y, t0.z);
                        nt1 = mliVec_init(tm.x, t1.y, tm.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[2 ^ permutation],
                                octree->nodes[node_idx].types[2 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 6, 8, 3);
                        break;
                }
                case 3: {
                        nt0 = mliVec_init(t0.x, tm.y, tm.z);
                        nt1 = mliVec_init(tm.x, t1.y, t1.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[3 ^ permutation],
                                octree->nodes[node_idx].types[3 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 7, 8, 8);
                        break;
                }
                case 4: {
                        nt0 = mliVec_init(tm.x, t0.y, t0.z);
                        nt1 = mliVec_init(t1.x, tm.y, tm.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[4 ^ permutation],
                                octree->nodes[node_idx].types[4 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 8, 6, 5);
                        break;
                }
                case 5: {
                        nt0 = mliVec_init(tm.x, t0.y, tm.z);
                        nt1 = mliVec_init(t1.x, tm.y, t1.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[5 ^ permutation],
                                octree->nodes[node_idx].types[5 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 8, 7, 8);
                        break;
                }
                case 6: {
                        nt0 = mliVec_init(tm.x, tm.y, t0.z);
                        nt1 = mliVec_init(t1.x, t1.y, tm.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[6 ^ permutation],
                                octree->nodes[node_idx].types[6 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = mli_ray_octree_traversal_next_octree_node(
                                nt1, 8, 8, 7);
                        break;
                }
                case 7: {
                        nt0 = mliVec_init(tm.x, tm.y, tm.z);
                        nt1 = mliVec_init(t1.x, t1.y, t1.z);
                        mli_ray_octree_traversal_sub(
                                nt0,
                                nt1,
                                octree,
                                octree->nodes[node_idx]
                                        .children[7 ^ permutation],
                                octree->nodes[node_idx].types[7 ^ permutation],
                                permutation,
                                work,
                                work_on_leaf_node);
                        proc_node = 8;
                        break;
                }
                } /* end switch */
        } while (proc_node < 8);
}

void mli_ray_octree_traversal(
        const struct mliOcTree *octree,
        const struct mliRay ray,
        void *work,
        void (*work_on_leaf_node)(
                void *,
                const struct mliOcTree *,
                const uint32_t))
{
        struct mliVec t0, t1;
        struct mliVec div;
        struct mliRay ray_wrt_octree;
        struct mliVec cube_upper, cube_size;
        struct mliCube cube;
        int32_t octree_root_node, octree_root_type;
        uint8_t permutation = 0;
        cube = octree->cube;
        cube_upper = mliCube_upper(cube);
        octree_root_node = 0u;
        octree_root_type = octree->root_type;
        cube_size = mliVec_add(cube.lower, cube_upper);

        ray_wrt_octree = ray;

        if (ray_wrt_octree.direction.x < 0) {
                ray_wrt_octree.support.x =
                        -ray_wrt_octree.support.x + cube_size.x;
                ray_wrt_octree.direction.x = -ray_wrt_octree.direction.x;
                permutation |= 4;
        } else if (ray_wrt_octree.direction.x == 0.0) {
                ray_wrt_octree.direction.x = MLI_RAY_OCTREE_TRAVERSAL_EPSILON;
        }

        if (ray_wrt_octree.direction.y < 0) {
                ray_wrt_octree.support.y =
                        -ray_wrt_octree.support.y + cube_size.y;
                ray_wrt_octree.direction.y = -ray_wrt_octree.direction.y;
                permutation |= 2;
        } else if (ray_wrt_octree.direction.y == 0.0) {
                ray_wrt_octree.direction.y = MLI_RAY_OCTREE_TRAVERSAL_EPSILON;
        }

        if (ray_wrt_octree.direction.z < 0) {
                ray_wrt_octree.support.z =
                        -ray_wrt_octree.support.z + cube_size.z;
                ray_wrt_octree.direction.z = -ray_wrt_octree.direction.z;
                permutation |= 1;
        } else if (ray_wrt_octree.direction.z == 0.0) {
                ray_wrt_octree.direction.z = MLI_RAY_OCTREE_TRAVERSAL_EPSILON;
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
                mli_ray_octree_traversal_sub(
                        t0,
                        t1,
                        octree,
                        octree_root_node,
                        octree_root_type,
                        permutation,
                        work,
                        work_on_leaf_node);
        }
}
