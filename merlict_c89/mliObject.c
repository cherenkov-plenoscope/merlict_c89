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

        return obj;
}

void mliObject_free(struct mliObject *obj)
{
        free(obj->vertices);
        free(obj->vertex_normals);
        free(obj->faces_vertices);
        free(obj->faces_vertex_normals);
        *obj = mliObject_init();
}

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces)
{
        mli_check(num_vertices < UINT32_MAX, "Expected num_vertices < uint32");
        mli_check(
                num_vertex_normals < UINT32_MAX,
                "Expected num_vertices < uint32");
        mli_check(num_faces < UINT32_MAX, "Expected num_vertices < uint32");

        mliObject_free(obj);
        obj->num_vertices = num_vertices;
        obj->num_vertex_normals = num_vertex_normals;
        obj->num_faces = num_faces;
        mli_malloc(obj->vertices, struct mliVec, obj->num_vertices);
        mli_malloc(obj->vertex_normals, struct mliVec, obj->num_vertex_normals);
        mli_malloc(obj->faces_vertices, struct mliFace, obj->num_faces);
        mli_malloc(obj->faces_vertex_normals, struct mliFace, obj->num_faces);
        return 1;
error:
        return 0;
}

int mliObject_assert_valid_faces(const struct mliObject *obj)
{
        uint64_t i;
        for (i = 0; i < obj->num_faces; i++) {
                mli_check(
                        obj->faces_vertices[i].a <= obj->num_vertices,
                        "Expected face.a <= num_vertices");
                mli_check(
                        obj->faces_vertices[i].b <= obj->num_vertices,
                        "Expected face.b <= num_vertices");
                mli_check(
                        obj->faces_vertices[i].c <= obj->num_vertices,
                        "Expected face.c <= num_vertices");

                mli_check(
                        obj->faces_vertex_normals[i].a <=
                                obj->num_vertex_normals,
                        "Expected faces_vertex_normals.a <= "
                        "num_vertex_normals");
                mli_check(
                        obj->faces_vertex_normals[i].b <=
                                obj->num_vertex_normals,
                        "Expected faces_vertex_normals.b <= "
                        "num_vertex_normals");
                mli_check(
                        obj->faces_vertex_normals[i].c <=
                                obj->num_vertex_normals,
                        "Expected faces_vertex_normals.c <= "
                        "num_vertex_normals");
        }
        return 1;
error:
        return 0;
}
