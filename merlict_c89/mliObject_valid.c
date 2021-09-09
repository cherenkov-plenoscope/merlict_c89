/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_valid.h"
#include "mli_debug.h"

int mliObject_is_valid(const struct mliObject *obj)
{
        chk_msg(mliObject_has_valid_vertices(obj),
                "Expected vertices to be valid.");
        chk_msg(mliObject_has_valid_faces(obj), "Expected faces to be valid.");
        chk_msg(mliObject_has_valid_normals(obj, MLI_EPSILON),
                "Expected vertex-normals to be normalized.");
        chk_msg(mliObject_has_valid_materials(obj),
                "Expected materials to be valid.");
        return 1;
error:
        return 0;
}

int mliObject_has_valid_faces(const struct mliObject *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_faces; i++) {
                chk_msg(obj->faces_vertices[i].a <= obj->num_vertices,
                        "Expected face.a <= num_vertices");
                chk_msg(obj->faces_vertices[i].b <= obj->num_vertices,
                        "Expected face.b <= num_vertices");
                chk_msg(obj->faces_vertices[i].c <= obj->num_vertices,
                        "Expected face.c <= num_vertices");

                chk_msg(obj->faces_vertex_normals[i].a <=
                                obj->num_vertex_normals,
                        "Expected faces_vertex_normals.a <= "
                        "num_vertex_normals");
                chk_msg(obj->faces_vertex_normals[i].b <=
                                obj->num_vertex_normals,
                        "Expected faces_vertex_normals.b <= "
                        "num_vertex_normals");
                chk_msg(obj->faces_vertex_normals[i].c <=
                                obj->num_vertex_normals,
                        "Expected faces_vertex_normals.c <= "
                        "num_vertex_normals");
                chk_msg(obj->faces_materials[i] < obj->num_materials,
                        "Expected faces_materials < "
                        "num_materials");
        }
        return 1;
error:
        chk_eprintf("faces[%u] is invalid.", i);
        return 0;
}

int mliObject_has_valid_vertices(const struct mliObject *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_vertices; i++) {
                chk_msg(!MLI_IS_NAN(obj->vertices[i].x), "X is 'nan'.");
                chk_msg(!MLI_IS_NAN(obj->vertices[i].y), "Y is 'nan'.");
                chk_msg(!MLI_IS_NAN(obj->vertices[i].z), "Z is 'nan'.");
        }
        return 1;
error:
        chk_eprintf("vertices[%u] is invalid.", i);
        return 0;
}

int mliObject_has_valid_normals(
        const struct mliObject *obj,
        const double epsilon)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_vertex_normals; i++) {
                double norm;
                chk_msg(!MLI_IS_NAN(obj->vertex_normals[i].x), "X is 'nan'.");
                chk_msg(!MLI_IS_NAN(obj->vertex_normals[i].y), "Y is 'nan'.");
                chk_msg(!MLI_IS_NAN(obj->vertex_normals[i].z), "Z is 'nan'.");

                norm = mliVec_norm(obj->vertex_normals[i]);
                chk_msg(fabs(norm - 1.0) <= epsilon,
                        "Expected vertex_normals to be normalized.");
        }
        return 1;
error:
        chk_eprintf("vertex_normals[%u] is invalid.", i);
        return 0;
}

int mliObject_has_valid_materials(const struct mliObject *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_materials; i++) {
                chk_msg(mliName_valid(&obj->material_names[i]),
                        "Expected material_name to be '\\0' terminated.");
                chk_msg(strlen(obj->material_names[i].c_str) > 0,
                        "Expected strlen(material_name) > 0.");
        }
        return 1;
error:
        chk_eprintf("materials[%u] is invalid.", i);
        return 0;
}
