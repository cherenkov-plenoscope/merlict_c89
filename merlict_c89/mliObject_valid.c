/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_valid.h"
#include "mli_debug.h"

int mliObject_is_valid(const struct mliObject *obj)
{
        mli_check(
                mliObject_has_valid_vertices(obj),
                "Expected vertices to be valid.");
        mli_check(
                mliObject_has_valid_faces(obj), "Expected faces to be valid.");
        mli_check(
                mliObject_has_valid_normals(obj, MLI_EPSILON),
                "Expected vertex-normals to be normalized.");
        mli_check(
                mliObject_has_valid_materials(obj),
                "Expected materials to be valid.");
        return 1;
error:
        return 0;
}

int mliObject_has_valid_faces(const struct mliObject *obj)
{
        uint32_t i = 0;
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
        mli_eprintf("faces[%u] is invalid.", i);
        return 0;
}

int mliObject_has_valid_vertices(const struct mliObject *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_vertices; i++) {
                mli_check(!MLI_IS_NAN(obj->vertices[i].x), "X is 'nan'.");
                mli_check(!MLI_IS_NAN(obj->vertices[i].y), "Y is 'nan'.");
                mli_check(!MLI_IS_NAN(obj->vertices[i].z), "Z is 'nan'.");
        }
        return 1;
error:
        mli_eprintf("vertices[%u] is invalid.", i);
        return 0;
}

int mliObject_has_valid_normals(
        const struct mliObject *obj,
        const double epsilon)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_vertex_normals; i++) {
                double norm;
                mli_check(!MLI_IS_NAN(obj->vertex_normals[i].x), "X is 'nan'.");
                mli_check(!MLI_IS_NAN(obj->vertex_normals[i].y), "Y is 'nan'.");
                mli_check(!MLI_IS_NAN(obj->vertex_normals[i].z), "Z is 'nan'.");

                norm = mliVec_norm(obj->vertex_normals[i]);
                mli_check(
                        fabs(norm - 1.0) <= epsilon,
                        "Expected vertex_normals to be normalized.");
        }
        return 1;
error:
        mli_eprintf("vertex_normals[%u] is invalid.", i);
        return 0;
}

int mliObject_has_valid_materials(const struct mliObject *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_materials; i++) {
                mli_check(
                        obj->first_face_in_next_material[i] <= obj->num_faces,
                        "Expected first_face_in_next_material < num_faces.");
                if (i > 0) {
                        mli_check(
                                obj->first_face_in_next_material[i] >
                                        obj->first_face_in_next_material[i - 1],
                                "Expected first_face_in_next_material to be "
                                "strictly "
                                "ascending, but it is not.");
                }
                mli_check(
                        mliName_valid(&obj->material_names[i]),
                        "Expected material_name to be '\\0' terminated.");
                mli_check(
                        strlen(obj->material_names[i].c_str) > 0,
                        "Expected strlen(material_name) > 0.");
        }
        return 1;
error:
        mli_eprintf("materials[%u] is invalid.", i);
        return 0;
}
