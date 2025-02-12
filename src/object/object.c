/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object.h"
#include <assert.h>
#include "../chk/chk.h"
#include <stdlib.h>

struct mli_Object mli_Object_init(void)
{
        struct mli_Object obj;
        obj.num_vertices = 0;
        obj.vertices = NULL;

        obj.num_vertex_normals = 0;
        obj.vertex_normals = NULL;

        obj.num_faces = 0;
        obj.faces_vertices = NULL;
        obj.faces_vertex_normals = NULL;
        obj.faces_materials = NULL;

        obj.num_materials = 0;
        obj.material_names = NULL;
        return obj;
}

void mli_Object_free(struct mli_Object *obj)
{
        size_t i;
        free(obj->vertices);
        free(obj->vertex_normals);
        free(obj->faces_vertices);
        free(obj->faces_vertex_normals);
        free(obj->faces_materials);

        for (i = 0; i < obj->num_materials; i++) {
                mli_String_free(&obj->material_names[i]);
        }
        free(obj->material_names);
        *obj = mli_Object_init();
}

chk_rc mli_Object_malloc(
        struct mli_Object *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces,
        const uint64_t num_materials)
{
        uint32_t i;
        chk_msg(num_vertices < UINT32_MAX, "Expected num_vertices < uint32");
        chk_msg(num_vertex_normals < UINT32_MAX,
                "Expected num_vertex_normals < uint32");
        chk_msg(num_faces < UINT32_MAX, "Expected num_faces < uint32");
        chk_msg(num_materials < UINT32_MAX, "Expected num_materials < uint32");
        chk_msg(num_materials > 0, "Expected num_materials > 0");
        chk_msg(num_materials <= num_faces,
                "Expected num_materials <= num_faces");

        mli_Object_free(obj);
        obj->num_vertices = num_vertices;
        obj->num_vertex_normals = num_vertex_normals;
        obj->num_faces = num_faces;
        obj->num_materials = num_materials;
        chk_malloc(obj->vertices, struct mli_Vec, obj->num_vertices);
        chk_malloc(
                obj->vertex_normals, struct mli_Vec, obj->num_vertex_normals);
        chk_malloc(obj->faces_vertices, struct mli_object_Face, obj->num_faces);
        chk_malloc(
                obj->faces_vertex_normals,
                struct mli_object_Face,
                obj->num_faces);
        chk_malloc(obj->faces_materials, uint16_t, obj->num_faces);
        chk_malloc(obj->material_names, struct mli_String, obj->num_materials);
        for (i = 0; i < obj->num_materials; i++) {
                obj->material_names[i] = mli_String_init();
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_Object_equal(
        const struct mli_Object *a,
        const struct mli_Object *b)
{
        uint64_t i;
        chk(a->num_vertices == b->num_vertices);
        chk(a->num_vertex_normals == b->num_vertex_normals);
        chk(a->num_faces == b->num_faces);
        chk(a->num_materials == b->num_materials);

        for (i = 0; i < a->num_vertices; i++) {
                chk(mli_Vec_equal(a->vertices[i], b->vertices[i]));
        }
        for (i = 0; i < a->num_vertex_normals; i++) {
                chk(mli_Vec_equal(a->vertex_normals[i], b->vertex_normals[i]));
        }
        for (i = 0; i < a->num_faces; i++) {
                chk(mli_object_Face_equal(
                        a->faces_vertices[i], b->faces_vertices[i]));
                chk(mli_object_Face_equal(
                        a->faces_vertex_normals[i],
                        b->faces_vertex_normals[i]));
                chk(a->faces_materials[i] == b->faces_materials[i]);
        }
        for (i = 0; i < a->num_materials; i++) {
                chk(mli_String_equal(
                        &a->material_names[i], &b->material_names[i]));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

uint32_t mli_Object_resolve_material_idx(
        const struct mli_Object *obj,
        const uint32_t face_idx)
{
        assert(face_idx < obj->num_faces);
        return obj->faces_materials[face_idx];
}
