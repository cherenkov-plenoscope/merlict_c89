/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject.h"
#include "mli_debug.h"

struct mliObject mliObject_init(void)
{
        struct mliObject obj;
        obj.num_vertices = 0;
        obj.vertices = NULL;

        obj.num_vertex_normals = 0;
        obj.vertex_normals = NULL;

        obj.num_faces = 0;
        obj.faces_vertices = NULL;
        obj.faces_vertex_normals = NULL;

        obj.num_materials = 0;
        obj.first_face_in_next_material = NULL;
        obj.material_names = NULL;
        return obj;
}

void mliObject_free(struct mliObject *obj)
{
        free(obj->vertices);
        free(obj->vertex_normals);
        free(obj->faces_vertices);
        free(obj->faces_vertex_normals);
        free(obj->first_face_in_next_material);
        free(obj->material_names);
        *obj = mliObject_init();
}

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces,
        const uint64_t num_materials)
{
        uint32_t i;
        mli_check(num_vertices < UINT32_MAX, "Expected num_vertices < uint32");
        mli_check(
                num_vertex_normals < UINT32_MAX,
                "Expected num_vertex_normals < uint32");
        mli_check(num_faces < UINT32_MAX, "Expected num_faces < uint32");
        mli_check(
                num_materials < UINT32_MAX, "Expected num_materials < uint32");
        mli_check(num_materials > 0, "Expected num_materials > 0");
        mli_check(
                num_materials <= num_faces,
                "Expected num_materials <= num_faces");

        mliObject_free(obj);
        obj->num_vertices = num_vertices;
        obj->num_vertex_normals = num_vertex_normals;
        obj->num_faces = num_faces;
        obj->num_materials = num_materials;
        mli_malloc(obj->vertices, struct mliVec, obj->num_vertices);
        mli_malloc(obj->vertex_normals, struct mliVec, obj->num_vertex_normals);
        mli_malloc(obj->faces_vertices, struct mliFace, obj->num_faces);
        mli_malloc(obj->faces_vertex_normals, struct mliFace, obj->num_faces);
        mli_malloc(
                obj->first_face_in_next_material, uint32_t, obj->num_materials);
        mli_malloc(obj->material_names, struct mliName, obj->num_materials);
        for (i = 0; i < obj->num_materials; i++) {
                obj->material_names[i] = mliName_init();
        }
        return 1;
error:
        return 0;
}

int mliObject_equal(const struct mliObject *a, const struct mliObject *b)
{
        uint64_t i;
        mli_c(a->num_vertices == b->num_vertices);
        mli_c(a->num_vertex_normals == b->num_vertex_normals);
        mli_c(a->num_faces == b->num_faces);
        mli_c(a->num_materials == b->num_materials);

        for (i = 0; i < a->num_vertices; i++) {
                mli_c(mliVec_equal(a->vertices[i], b->vertices[i]));
        }
        for (i = 0; i < a->num_vertex_normals; i++) {
                mli_c(mliVec_equal(a->vertex_normals[i], b->vertex_normals[i]));
        }
        for (i = 0; i < a->num_faces; i++) {
                mli_c(mliFace_equal(
                        a->faces_vertices[i], b->faces_vertices[i]));
                mli_c(mliFace_equal(
                        a->faces_vertex_normals[i],
                        b->faces_vertex_normals[i]));
        }
        for (i = 0; i < a->num_materials; i++) {
                mli_c(a->first_face_in_next_material[i] ==
                      b->first_face_in_next_material[i]);
                mli_c(mliName_equal(
                        &a->material_names[i], &b->material_names[i]));
        }
        return 1;
error:
        return 0;
}

uint32_t mliObject_resolve_material_idx(
        const struct mliObject *obj,
        const uint32_t face_idx)
{
        uint32_t material_idx = 0;
        assert(face_idx < obj->num_faces);
        MLI_UPPER_COMPARE(
                obj->first_face_in_next_material,
                obj->num_materials,
                face_idx,
                material_idx);
        return material_idx;
}
