/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "ray_scenery_query.h"
#include <float.h>
#include "from_outside_to_inside.h"
#include "../triangle/triangle_intersection.h"
#include "../object/object_face.h"

void mli_tracer_inner_object_traversal(
        void *_inner,
        const struct mli_OcTree *object_octree,
        const uint32_t object_octree_leaf_idx)
{
        /* traverse faces in an object-wavefront */
        struct mli_trace_QueryInnerWork *inner =
                (struct mli_trace_QueryInnerWork *)_inner;

        uint32_t f;
        const uint32_t num_faces_in_object_leaf = mli_OcTree_leaf_num_objects(
                object_octree, object_octree_leaf_idx);

        struct mli_Intersection tmp_isec = mli_Intersection_init();

        for (f = 0; f < num_faces_in_object_leaf; f++) {

                uint32_t face_idx = mli_OcTree_leaf_object_link(
                        object_octree, object_octree_leaf_idx, f);

                struct mli_object_Face fv =
                        inner->object->faces_vertices[face_idx];

                int32_t hit = mli_Ray_intersects_triangle(
                        inner->ray_object,
                        inner->object->vertices[fv.a],
                        inner->object->vertices[fv.b],
                        inner->object->vertices[fv.c],
                        &tmp_isec.distance_of_ray);

                tmp_isec.geometry_id.face = face_idx;

                if (hit) {
                        tmp_isec.position_local = mli_Ray_at(
                                &inner->ray_object, tmp_isec.distance_of_ray);

                        inner->has_intersection = 1;
                        if (tmp_isec.distance_of_ray <
                            inner->intersection->distance_of_ray) {
                                (*inner->intersection) = tmp_isec;
                        }
                }
        }
        return;
}

int mli_query_object_reference(
        const struct mli_Object *object,
        const struct mli_OcTree *object_octree,
        const struct mli_HomTraComp robject2root_comp,
        const struct mli_Ray ray_root,
        struct mli_Intersection *isec)
{
        struct mli_HomTra robject2root =
                mli_HomTraComp_from_compact(robject2root_comp);

        struct mli_trace_QueryInnerWork inner;
        inner.has_intersection = 0;
        inner.intersection = isec;
        inner.ray_object = mli_HomTraComp_ray_inverse(&robject2root, ray_root);
        inner.object = object;

        mli_trace_ray_octree_traversal(
                object_octree,
                inner.ray_object,
                (void *)&inner,
                mli_tracer_inner_object_traversal);

        return inner.has_intersection;
}

void mli_tracer_outer_scenery_traversal(
        void *_outer,
        const struct mli_OcTree *scenery_octree,
        const uint32_t scenery_octree_leaf_idx)
{
        /* traverse object-wavefronts in a scenery */
        struct mli_tracer_QueryOuterWork *outer =
                (struct mli_tracer_QueryOuterWork *)_outer;

        uint32_t ro;
        const uint32_t num_robjects_in_scenery_leaf =
                mli_OcTree_leaf_num_objects(
                        scenery_octree, scenery_octree_leaf_idx);

        struct mli_Intersection tmp_isec = mli_Intersection_init();

        for (ro = 0; ro < num_robjects_in_scenery_leaf; ro++) {

                uint32_t robject_idx = mli_OcTree_leaf_object_link(
                        scenery_octree, scenery_octree_leaf_idx, ro);
                uint32_t object_idx = outer->geometry->robjects[robject_idx];

                int32_t hit = mli_query_object_reference(
                        &outer->geometry->objects[object_idx],
                        &outer->accelerator->object_octrees[object_idx],
                        outer->geometry->robject2root[robject_idx],
                        outer->ray_root,
                        &tmp_isec);

                tmp_isec.geometry_id.robj = robject_idx;

                if (hit) {
                        if (tmp_isec.distance_of_ray <
                            outer->intersection->distance_of_ray) {
                                (*outer->intersection) = tmp_isec;
                        }
                }
        }
        return;
}

int mli_trace_query_intersection(
        const struct mli_Scenery *scenery,
        const struct mli_Ray ray_root,
        struct mli_Intersection *isec)
{
        struct mli_tracer_QueryOuterWork outer;

        (*isec) = mli_Intersection_init();

        outer.intersection = isec;
        outer.geometry = &scenery->geometry;
        outer.accelerator = &scenery->accelerator;
        outer.ray_root = ray_root;

        mli_trace_ray_octree_traversal(
                &scenery->accelerator.scenery_octree,
                ray_root,
                (void *)&outer,
                mli_tracer_outer_scenery_traversal);

        if (isec->distance_of_ray == DBL_MAX) {
                return 0;
        } else {
                return 1;
        }
}

int mli_trace_query_intersection_with_surface_normal(
        const struct mli_Scenery *scenery,
        const struct mli_Ray ray_root,
        struct mli_IntersectionSurfaceNormal *isecsrf)
{
        struct mli_Intersection isec = mli_Intersection_init();

        const int has_intersection =
                mli_trace_query_intersection(scenery, ray_root, &isec);

        if (has_intersection) {
                uint32_t robject_idx = isec.geometry_id.robj;
                uint32_t object_idx =
                        scenery->geometry.robjects[isec.geometry_id.robj];
                uint32_t face_idx = isec.geometry_id.face;

                struct mli_HomTra robject2root = mli_HomTraComp_from_compact(
                        scenery->geometry.robject2root[robject_idx]);
                struct mli_Ray ray_object =
                        mli_HomTraComp_ray_inverse(&robject2root, ray_root);

                struct mli_Object *obj = &scenery->geometry.objects[object_idx];

                struct mli_object_Face fv = obj->faces_vertices[face_idx];
                struct mli_object_Face fvn =
                        obj->faces_vertex_normals[face_idx];

                (*isecsrf) = mli_IntersectionSurfaceNormal_init();
                isecsrf->distance_of_ray = isec.distance_of_ray;
                isecsrf->geometry_id = isec.geometry_id;
                isecsrf->position = mli_Ray_at(&ray_root, isec.distance_of_ray);
                isecsrf->position_local = isec.position_local;

                /* find surface-normal */
                isecsrf->surface_normal_local = mli_Triangle_surface_normal(
                        obj->vertices[fv.a],
                        obj->vertices[fv.b],
                        obj->vertices[fv.c],
                        obj->vertex_normals[fvn.a],
                        obj->vertex_normals[fvn.b],
                        obj->vertex_normals[fvn.c],
                        isecsrf->position_local);

                isecsrf->surface_normal = mli_HomTraComp_dir(
                        &robject2root, isecsrf->surface_normal_local);

                isecsrf->from_outside_to_inside =
                        mli_trace_ray_runs_from_outside_to_inside(
                                ray_object.direction,
                                isecsrf->surface_normal_local);

                return 1;
        } else {
                return 0;
        }
}
