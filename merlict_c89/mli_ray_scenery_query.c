/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_ray_scenery_query.h"

void _mli_inner_object_traversal(
        void *_inner,
        const struct mliOcTree *object_octree,
        const uint32_t object_octree_leaf_idx)
{
        /* traverse faces in an object-wavefront */
        struct _mliInnerWork *inner = (struct _mliInnerWork *)_inner;

        uint32_t f;
        const uint32_t num_faces_in_object_leaf = mliOcTree_leaf_num_objects(
                object_octree, object_octree_leaf_idx);

        struct mliIntersectionMinimalQuery tmp_isecmin =
                mliIntersectionMinimalQuery_init();

        for (f = 0; f < num_faces_in_object_leaf; f++) {

                uint32_t face_idx = mliOcTree_leaf_object_link(
                        object_octree, object_octree_leaf_idx, f);

                struct mliFace fv = inner->object->faces_vertices[face_idx];

                int32_t hit = mliRay_intersects_triangle(
                        inner->ray_object,
                        inner->object->vertices[fv.a],
                        inner->object->vertices[fv.b],
                        inner->object->vertices[fv.c],
                        &tmp_isecmin.distance_of_ray);

                tmp_isecmin.geometry_id.face = face_idx;

                if (hit) {
                        tmp_isecmin.position_local = mliRay_at(
                                &inner->ray_object,
                                tmp_isecmin.distance_of_ray);

                        inner->has_intersection = 1;
                        if (tmp_isecmin.distance_of_ray <
                            inner->intersection->distance_of_ray) {
                                (*inner->intersection) = tmp_isecmin;
                        }
                }
        }
        return;
}

int _mli_query_object_reference(
        const struct mliObject *object,
        const struct mliOcTree *object_octree,
        const struct mliHomTraComp robject2root_comp,
        const struct mliRay ray_root,
        struct mliIntersectionMinimalQuery *isecmin)
{
        struct mliHomTra robject2root =
                mliHomTra_from_compact(robject2root_comp);

        struct _mliInnerWork inner;
        inner.has_intersection = 0;
        inner.intersection = isecmin;
        inner.ray_object = mliHomTra_ray_inverse(&robject2root, ray_root);
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
        struct _mliOuterWork *outer = (struct _mliOuterWork *)_outer;

        uint32_t ro;
        const uint32_t num_robjects_in_scenery_leaf =
                mliOcTree_leaf_num_objects(
                        scenery_octree, scenery_octree_leaf_idx);

        struct mliIntersectionMinimalQuery tmp_isecmin =
                mliIntersectionMinimalQuery_init();

        for (ro = 0; ro < num_robjects_in_scenery_leaf; ro++) {

                uint32_t robject_idx = mliOcTree_leaf_object_link(
                        scenery_octree, scenery_octree_leaf_idx, ro);
                uint32_t object_idx = outer->geometry->robjects[robject_idx];

                int32_t hit = _mli_query_object_reference(
                        &outer->geometry->objects[object_idx],
                        &outer->accelerator->object_octrees[object_idx],
                        outer->geometry->robject2root[robject_idx],
                        outer->ray_root,
                        &tmp_isecmin);

                tmp_isecmin.geometry_id.robj = robject_idx;

                if (hit) {
                        if (tmp_isecmin.distance_of_ray <
                            outer->intersection->distance_of_ray) {
                                (*outer->intersection) = tmp_isecmin;
                        }
                }
        }
        return;
}

void mli_query_intersection_minimal(
        const struct mliScenery *scenery,
        const struct mliRay ray_root,
        struct mliIntersectionMinimalQuery *isecmin)
{
        struct _mliOuterWork outer;

        (*isecmin) = mliIntersectionMinimalQuery_init();

        outer.intersection = isecmin;
        outer.geometry = &scenery->geometry;
        outer.accelerator = &scenery->accelerator;
        outer.ray_root = ray_root;

        mli_ray_octree_traversal(
                &scenery->accelerator.scenery_octree,
                ray_root,
                (void *)&outer,
                _mli_outer_scenery_traversal);
}

int mli_query_intersection_with_surface_normal(
        const struct mliScenery *scenery,
        const struct mliRay ray_root,
        struct mliIntersectionSurfaceNormal *isecsrf)
{
        struct mliIntersectionMinimalQuery isecmin =
                mliIntersectionMinimalQuery_init();

        mli_query_intersection_minimal(scenery, ray_root, &isecmin);
        if (isecmin.distance_of_ray == DBL_MAX) {
                return 0;
        } else {
                uint32_t robject_idx = isecmin.geometry_id.robj;
                uint32_t object_idx =
                        scenery->geometry.robjects[isecmin.geometry_id.robj];
                uint32_t face_idx = isecmin.geometry_id.face;

                struct mliHomTra robject2root = mliHomTra_from_compact(
                        scenery->geometry.robject2root[robject_idx]);
                struct mliRay ray_object =
                        mliHomTra_ray_inverse(&robject2root, ray_root);

                struct mliObject *obj = &scenery->geometry.objects[object_idx];

                struct mliFace fv = obj->faces_vertices[face_idx];
                struct mliFace fvn = obj->faces_vertex_normals[face_idx];

                (*isecsrf) = mliIntersectionSurfaceNormal_init();
                isecsrf->distance_of_ray = isecmin.distance_of_ray;
                isecsrf->geometry_id = isecmin.geometry_id;
                isecsrf->position =
                        mliRay_at(&ray_root, isecmin.distance_of_ray);
                isecsrf->position_local = isecmin.position_local;

                /* find surface-normal */
                isecsrf->surface_normal_local = mliTriangle_surface_normal(
                        obj->vertices[fv.a],
                        obj->vertices[fv.b],
                        obj->vertices[fv.c],
                        obj->vertex_normals[fvn.a],
                        obj->vertex_normals[fvn.b],
                        obj->vertex_normals[fvn.c],
                        isecsrf->position_local);

                isecsrf->surface_normal = mliHomTra_dir(
                        &robject2root, isecsrf->surface_normal_local);

                isecsrf->from_outside_to_inside =
                        mli_ray_runs_from_outside_to_inside(
                                ray_object.direction,
                                isecsrf->surface_normal_local);

                return 1;
        }
}
