/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object_valid.h"
#include "../math/math.h"
#include "../chk/chk.h"

mli_bool mli_Object_is_valid(const struct mli_Object *obj)
{
        chk_msg(mli_Object_has_valid_vertices(obj), "Bad vertex.");
        chk_msg(mli_Object_has_valid_faces(obj), "Expected faces to be valid.");
        chk_msg(mli_Object_has_valid_normals(obj, MLI_MATH_EPSILON),
                "Bad vertex-normal.");
        chk_msg(mli_Object_has_valid_materials(obj), "Bad material.");
        return MLI_TRUE;
chk_error:
        return MLI_FALSE;
}

mli_bool mli_Object_has_valid_faces(const struct mli_Object *obj)
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
        return MLI_TRUE;
chk_error:
        chk_eprintf("In obj.faces[%u]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Object_has_valid_vertices(const struct mli_Object *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_vertices; i++) {
                chk_msg(!MLI_MATH_IS_NAN(obj->vertices[i].x), "X is 'nan'.");
                chk_msg(!MLI_MATH_IS_NAN(obj->vertices[i].y), "Y is 'nan'.");
                chk_msg(!MLI_MATH_IS_NAN(obj->vertices[i].z), "Z is 'nan'.");
        }
        return MLI_TRUE;
chk_error:
        chk_eprintf("In obj.vertices[%u]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Object_has_valid_normals(
        const struct mli_Object *obj,
        const double epsilon)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_vertex_normals; i++) {
                double norm;
                chk_msg(!MLI_MATH_IS_NAN(obj->vertex_normals[i].x),
                        "X is 'nan'.");
                chk_msg(!MLI_MATH_IS_NAN(obj->vertex_normals[i].y),
                        "Y is 'nan'.");
                chk_msg(!MLI_MATH_IS_NAN(obj->vertex_normals[i].z),
                        "Z is 'nan'.");

                norm = mli_Vec_norm(obj->vertex_normals[i]);
                chk_msg(fabs(norm - 1.0) <= epsilon,
                        "Expected vertex_normals to be normalized.");
        }
        return MLI_TRUE;
chk_error:
        chk_eprintf("In obj.vertex_normals[%u]\n", i);
        return MLI_FALSE;
}

mli_bool mli_Object_has_valid_materials(const struct mli_Object *obj)
{
        uint32_t i = 0;
        for (i = 0; i < obj->num_materials; i++) {
                chk(mli_String_valid(&obj->material_names[i], 1));
        }
        return MLI_TRUE;
chk_error:
        chk_eprintf("In obj.material_names[%u]\n", i);
        return MLI_FALSE;
}

chk_rc mli_Object_num_unused(
        const struct mli_Object *obj,
        uint32_t *num_unused_vertices,
        uint32_t *num_unused_vertex_normals,
        uint32_t *num_unused_materials)
{
        uint64_t *v = NULL;
        uint64_t *vn = NULL;
        uint64_t *mtl = NULL;
        uint64_t i, f;

        (*num_unused_vertices) = 0;
        (*num_unused_vertex_normals) = 0;
        (*num_unused_materials) = 0;

        chk_malloc(v, uint64_t, obj->num_vertices);
        MLI_MATH_ARRAY_SET(v, 0, obj->num_vertices);

        chk_malloc(vn, uint64_t, obj->num_vertex_normals);
        MLI_MATH_ARRAY_SET(vn, 0, obj->num_vertex_normals);

        chk_malloc(mtl, uint64_t, obj->num_materials);
        MLI_MATH_ARRAY_SET(mtl, 0, obj->num_materials);

        for (f = 0; f < obj->num_faces; f++) {
                v[obj->faces_vertices[f].a] += 1;
                v[obj->faces_vertices[f].b] += 1;
                v[obj->faces_vertices[f].c] += 1;

                vn[obj->faces_vertex_normals[f].a] += 1;
                vn[obj->faces_vertex_normals[f].b] += 1;
                vn[obj->faces_vertex_normals[f].c] += 1;

                mtl[obj->faces_materials[f]] += 1;
        }

        for (i = 0; i < obj->num_vertices; i++) {
                if (v[i] == 0) {
                        (*num_unused_vertices) += 1;
                }
        }

        for (i = 0; i < obj->num_vertex_normals; i++) {
                if (vn[i] == 0) {
                        (*num_unused_vertex_normals) += 1;
                }
        }

        for (i = 0; i < obj->num_materials; i++) {
                if (mtl[i] == 0) {
                        (*num_unused_materials) += 1;
                }
        }

        free(v);
        free(vn);
        free(mtl);
        return CHK_SUCCESS;
chk_error:
        free(v);
        free(vn);
        free(mtl);
        return CHK_FAIL;
}
