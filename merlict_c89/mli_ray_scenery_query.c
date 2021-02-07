/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_ray_scenery_query.h"

struct _mliInnerObjectWork {
        struct mliIntersectionMinimalQuery *intersection;
        const struct mliObject *object;
        struct mliRay ray_object;
        int has_intersection;
};

struct _mliOuterSceneryWork {
        struct mliIntersectionMinimalQuery *intersection;
        const struct mliScenery *scenery;
        const struct mliAccelerator *accelerator;
        struct mliRay ray_root;
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

        struct mliIntersectionMinimalQuery tmp_isec_min = mliIntersectionMinimalQuery_init();

        for (f = 0; f < num_faces_in_object_leaf; f++) {

                uint32_t face_idx = mliOcTree_leaf_object_link(
                        object_octree, object_octree_leaf_idx, f);

                struct mliFace fv = inner->object->faces_vertices[face_idx];

                int32_t hit = mliRay_intersects_triangle(
                        inner->ray_object,
                        inner->object->vertices[fv.a],
                        inner->object->vertices[fv.b],
                        inner->object->vertices[fv.c],
                        &tmp_isec_min.distance_of_ray);

                tmp_isec_min.geometry_id.face = face_idx;

                if (hit) {
                        tmp_isec_min.position_local = mliRay_at(
                                &inner->ray_object,
                                tmp_isec_min.distance_of_ray);

                        inner->has_intersection = 1;
                        if (tmp_isec_min.distance_of_ray <
                            inner->intersection->distance_of_ray) {
                                (*inner->intersection) = tmp_isec_min;
                        }
                }
        }
        return;
}

int _mli_query_object_reference(
        const struct mliObject *object,
        const struct mliOcTree *object_octree,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray_root,
        struct mliIntersectionMinimalQuery *isec_min)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);

        struct _mliInnerObjectWork inner;
        inner.has_intersection = 0;
        inner.intersection = isec_min;
        inner.ray_object = mliHomTra_ray_inverse(&local2root, ray_root);
        inner.object = object;

        mli_ray_octree_traversal(
                object_octree,
                inner.ray_object,
                (void *)&inner,
                _mli_inner_object_traversal);

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

        struct mliIntersectionMinimalQuery tmp_isec_min = mliIntersectionMinimalQuery_init();

        for (ro = 0; ro < num_robjects_in_scenery_leaf; ro++) {

                uint32_t robject_idx = mliOcTree_leaf_object_link(
                        scenery_octree, scenery_octree_leaf_idx, ro);
                uint32_t object_idx = outer->scenery->robjects[robject_idx];

                int32_t hit = _mli_query_object_reference(
                        &outer->scenery->resources.objects[object_idx],
                        &outer->accelerator->object_octrees[object_idx],
                        outer->scenery->robject2root[robject_idx],
                        outer->ray_root,
                        &tmp_isec_min);

                tmp_isec_min.geometry_id.robj = robject_idx;

                if (hit) {
                        if (tmp_isec_min.distance_of_ray <
                            outer->intersection->distance_of_ray) {
                                (*outer->intersection) = tmp_isec_min;
                        }
                }
        }
        return;
}

void mli_query_intersection_minimal(
        const struct mliCombine *combine,
        const struct mliRay ray_root,
        struct mliIntersectionMinimalQuery *isec_min)
{
        struct _mliOuterSceneryWork outer;

        (*isec_min) = mliIntersectionMinimalQuery_init();

        outer.intersection = isec_min;
        outer.scenery = combine->scenery;
        outer.accelerator = combine->accelerator;
        outer.ray_root = ray_root;

        mli_ray_octree_traversal(
                &combine->accelerator->scenery_octree,
                ray_root,
                (void *)&outer,
                _mli_outer_scenery_traversal);
}

int mli_query_intersection_with_surface_normal(
        const struct mliCombine *combine,
        const struct mliRay ray_root,
        struct mliIntersectionSurfaceNormal *isec_srf)
{
        struct mliIntersectionMinimalQuery isec_min = mliIntersectionMinimalQuery_init();

        mli_query_intersection_minimal(combine, ray_root, &isec_min);
        if (isec_min.distance_of_ray == DBL_MAX) {
                return 0;
        } else {
                uint32_t robject_idx = isec_min.geometry_id.robj;
                uint32_t object_idx = combine->scenery->robjects[
                        isec_min.geometry_id.robj];
                uint32_t face_idx = isec_min.geometry_id.face;

                struct mliHomTra local2root = mliHomTra_from_compact(
                        combine->scenery->robject2root[robject_idx]);
                struct mliRay ray_object = mliHomTra_ray_inverse(
                        &local2root,
                        ray_root);

                struct mliObject *obj = &combine->scenery->resources.objects[
                        object_idx];

                struct mliFace fv = obj->faces_vertices[face_idx];
                struct mliFace fvn = obj->faces_vertex_normals[face_idx];

                (*isec_srf) = mliIntersectionSurfaceNormal_init();
                isec_srf->distance_of_ray = isec_min.distance_of_ray;
                isec_srf->geometry_id = isec_min.geometry_id;
                isec_srf->position = mliRay_at(
                        &ray_root, isec_min.distance_of_ray);
                isec_srf->position_local = isec_min.position_local;

                /* find surface-normal */
                isec_srf->surface_normal_local = mliTriangle_surface_normal(
                        obj->vertices[fv.a],
                        obj->vertices[fv.b],
                        obj->vertices[fv.c],
                        obj->vertex_normals[fvn.a],
                        obj->vertex_normals[fvn.b],
                        obj->vertex_normals[fvn.c],
                        isec_srf->position_local);

                isec_srf->surface_normal = mliHomTra_dir(
                        &local2root, isec_srf->surface_normal_local);

                isec_srf->from_outside_to_inside =
                        mli_ray_runs_from_outside_to_inside(
                                ray_object.direction,
                                isec_srf->surface_normal_local);

                return 1;
        }
}
