/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_ray_scenery_query.h"

struct _mliInnerObjectWork {
        struct mliIntersection *intersection;
        const struct mliObject *object;
        struct mliRay ray_wrt_object;
        int has_intersection;
};

struct _mliOuterSceneryWork {
        struct mliIntersection *intersection;
        const struct mliScenery *scenery;
        const struct mliAccelerator *accelerator;
        struct mliRay ray_wrt_root;
};

void _mli_inner_object_traversal(
        void *_inner,
        const struct mliOcTree *object_octree,
        const uint32_t object_octree_leaf_idx)
{
        /* traverse faces in an object-wavefront */
        struct _mliInnerObjectWork *inner =
                (struct _mliInnerObjectWork *)_inner;

        uint32_t f;
        const uint32_t num_faces_in_object_leaf = mliOcTree_leaf_num_objects(
                object_octree, object_octree_leaf_idx);

        struct mliIntersection tmp_intersection = mliIntersection_init();

        for (f = 0; f < num_faces_in_object_leaf; f++) {

                uint32_t face_idx = mliOcTree_leaf_object_link(
                        object_octree, object_octree_leaf_idx, f);

                struct mliFace fv = inner->object->faces_vertices[face_idx];
                struct mliFace fvn =
                        inner->object->faces_vertex_normals[face_idx];

                int32_t face_has_intersection = mliTriangle_intersection(
                        inner->ray_wrt_object,

                        inner->object->vertices[fv.a],
                        inner->object->vertices[fv.b],
                        inner->object->vertices[fv.c],

                        inner->object->vertex_normals[fvn.a],
                        inner->object->vertex_normals[fvn.b],
                        inner->object->vertex_normals[fvn.c],

                        &tmp_intersection.distance_of_ray,
                        &tmp_intersection.position_local,
                        &tmp_intersection.surface_normal_local);

                tmp_intersection.geometry_id.face = face_idx;
                tmp_intersection.from_outside_to_inside =
                        mli_ray_runs_from_outside_to_inside(
                                inner->ray_wrt_object.direction,
                                tmp_intersection.surface_normal_local);

                if (face_has_intersection) {
                        inner->has_intersection = 1;
                        if (tmp_intersection.distance_of_ray <
                            inner->intersection->distance_of_ray) {
                                (*inner->intersection) = tmp_intersection;
                        }
                }
        }
        return;
}

int mliRobject_intersection(
        const struct mliObject *object,
        const struct mliOcTree *object_octree,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray_wrt_root,
        struct mliIntersection *intersection)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);

        struct _mliInnerObjectWork inner;
        inner.has_intersection = 0;
        inner.intersection = intersection;
        inner.ray_wrt_object = mliHomTra_ray_inverse(&local2root, ray_wrt_root);
        inner.object = object;

        mli_ray_octree_traversal(
                object_octree,
                inner.ray_wrt_object,
                (void *)&inner,
                _mli_inner_object_traversal);

        inner.intersection->position =
                mliHomTra_pos(&local2root, inner.intersection->position_local);
        inner.intersection->surface_normal = mliHomTra_dir(
                &local2root, inner.intersection->surface_normal_local);

        return inner.has_intersection;
}

void _mli_outer_scenery_traversal(
        void *_outer,
        const struct mliOcTree *scenery_octree,
        const uint32_t scenery_octree_leaf_idx)
{
        /* traverse object-wavefronts in a scenery */
        struct _mliOuterSceneryWork *outer =
                (struct _mliOuterSceneryWork *)_outer;

        uint32_t ro;
        const uint32_t num_robjects_in_scenery_leaf =
                mliOcTree_leaf_num_objects(
                        scenery_octree, scenery_octree_leaf_idx);

        struct mliIntersection tmp_intersection = mliIntersection_init();

        for (ro = 0; ro < num_robjects_in_scenery_leaf; ro++) {

                uint32_t robject_idx = mliOcTree_leaf_object_link(
                        scenery_octree, scenery_octree_leaf_idx, ro);
                uint32_t object_idx = outer->scenery->robjects[robject_idx];

                int32_t robject_has_intersection = mliRobject_intersection(
                        &outer->scenery->resources.objects[object_idx],
                        &outer->accelerator->object_octrees[object_idx],
                        outer->scenery->robject2root[robject_idx],
                        outer->ray_wrt_root,
                        &tmp_intersection);

                tmp_intersection.geometry_id.robj = robject_idx;

                if (robject_has_intersection) {
                        if (tmp_intersection.distance_of_ray <
                            outer->intersection->distance_of_ray) {
                                (*outer->intersection) = tmp_intersection;
                        }
                }
        }
        return;
}

void mli_ray_scenery_query(
        const struct mliCombine *combine,
        const struct mliRay ray_wrt_root,
        struct mliIntersection *intersection)
{
        struct _mliOuterSceneryWork outer;

        (*intersection) = mliIntersection_init();

        outer.intersection = intersection;
        outer.scenery = combine->scenery;
        outer.accelerator = combine->accelerator;
        outer.ray_wrt_root = ray_wrt_root;

        mli_ray_octree_traversal(
                &combine->accelerator->scenery_octree,
                ray_wrt_root,
                (void *)&outer,
                _mli_outer_scenery_traversal);
}
