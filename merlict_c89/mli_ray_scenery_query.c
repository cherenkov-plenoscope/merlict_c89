/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_ray_scenery_query.h"

struct _mliInnerObjectWork {
        struct mliPresection *presection;
        const struct mliObject *object;
        struct mliRay ray_wrt_object;
        int has_intersection;
};

struct _mliOuterSceneryWork {
        struct mliPresection *presection;
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

        struct mliPresection tmp_presection = mliPresection_init();

        for (f = 0; f < num_faces_in_object_leaf; f++) {

                uint32_t face_idx = mliOcTree_leaf_object_link(
                        object_octree, object_octree_leaf_idx, f);

                struct mliFace fv = inner->object->faces_vertices[face_idx];

                int32_t face_has_intersection = mliRay_intersects_triangle(
                        inner->ray_wrt_object,
                        inner->object->vertices[fv.a],
                        inner->object->vertices[fv.b],
                        inner->object->vertices[fv.c],
                        &tmp_presection.distance_of_ray);

                tmp_presection.geometry_id.face = face_idx;

                if (face_has_intersection) {

                        tmp_presection.position_local = mliRay_at(
                                &inner->ray_wrt_object,
                                tmp_presection.distance_of_ray);

                        inner->has_intersection = 1;
                        if (tmp_presection.distance_of_ray <
                            inner->presection->distance_of_ray) {
                                (*inner->presection) = tmp_presection;
                        }
                }
        }
        return;
}

int _mliRobject_first_causual_intersection(
        const struct mliObject *object,
        const struct mliOcTree *object_octree,
        const struct mliHomTraComp local2root_comp,
        const struct mliRay ray_wrt_root,
        struct mliPresection *presection)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);

        struct _mliInnerObjectWork inner;
        inner.has_intersection = 0;
        inner.presection = presection;
        inner.ray_wrt_object = mliHomTra_ray_inverse(&local2root, ray_wrt_root);
        inner.object = object;

        mli_ray_octree_traversal(
                object_octree,
                inner.ray_wrt_object,
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

        struct mliPresection tmp_presection = mliPresection_init();

        for (ro = 0; ro < num_robjects_in_scenery_leaf; ro++) {

                uint32_t robject_idx = mliOcTree_leaf_object_link(
                        scenery_octree, scenery_octree_leaf_idx, ro);
                uint32_t object_idx = outer->scenery->robjects[robject_idx];

                int32_t robject_has_intersection = _mliRobject_first_causual_intersection(
                        &outer->scenery->resources.objects[object_idx],
                        &outer->accelerator->object_octrees[object_idx],
                        outer->scenery->robject2root[robject_idx],
                        outer->ray_wrt_root,
                        &tmp_presection);

                tmp_presection.geometry_id.robj = robject_idx;

                if (robject_has_intersection) {
                        if (tmp_presection.distance_of_ray <
                            outer->presection->distance_of_ray) {
                                (*outer->presection) = tmp_presection;
                        }
                }
        }
        return;
}

void mli_ray_scenery_query(
        const struct mliCombine *combine,
        const struct mliRay ray_wrt_root,
        struct mliPresection *presection)
{
        struct _mliOuterSceneryWork outer;

        (*presection) = mliPresection_init();

        outer.presection = presection;
        outer.scenery = combine->scenery;
        outer.accelerator = combine->accelerator;
        outer.ray_wrt_root = ray_wrt_root;

        mli_ray_octree_traversal(
                &combine->accelerator->scenery_octree,
                ray_wrt_root,
                (void *)&outer,
                _mli_outer_scenery_traversal);
}

int mli_first_casual_intersection(
        const struct mliCombine *combine,
        const struct mliRay ray,
        struct mliIntersection *intersection)
{
        struct mliPresection presec = mliPresection_init();

        mli_ray_scenery_query(combine, ray, &presec);
        if (presec.distance_of_ray == DBL_MAX) {
                return 0;
        } else {
                uint32_t robject_idx = presec.geometry_id.robj;
                uint32_t object_idx = combine->scenery->robjects[
                        presec.geometry_id.robj];
                uint32_t face_idx = presec.geometry_id.face;

                struct mliHomTra local2root = mliHomTra_from_compact(
                        combine->scenery->robject2root[robject_idx]);
                struct mliRay ray_wrt_object = mliHomTra_ray_inverse(
                        &local2root,
                        ray);

                struct mliObject *obj = &combine->scenery->resources.objects[
                        object_idx];

                struct mliFace fv = obj->faces_vertices[face_idx];
                struct mliFace fvn = obj->faces_vertex_normals[face_idx];

                (*intersection) = mliIntersection_init();
                intersection->distance_of_ray = presec.distance_of_ray;
                intersection->geometry_id = presec.geometry_id;
                intersection->position = mliRay_at(
                        &ray, presec.distance_of_ray);
                intersection->position_local = presec.position_local;

                /* find surface-normal */
                intersection->surface_normal_local = mliTriangle_surface_normal(
                        obj->vertices[fv.a],
                        obj->vertices[fv.b],
                        obj->vertices[fv.c],
                        obj->vertex_normals[fvn.a],
                        obj->vertex_normals[fvn.b],
                        obj->vertex_normals[fvn.c],
                        intersection->position_local);

                intersection->surface_normal = mliHomTra_dir(
                        &local2root, intersection->surface_normal_local);

                intersection->from_outside_to_inside =
                        mli_ray_runs_from_outside_to_inside(
                                ray_wrt_object.direction,
                                intersection->surface_normal_local);

                return 1;
        }
}
