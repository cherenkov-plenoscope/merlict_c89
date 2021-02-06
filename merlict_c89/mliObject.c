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

        obj.num_groups = 0;
        obj.last_face_in_group = NULL;
        return obj;
}

void mliObject_free(struct mliObject *obj)
{
        free(obj->vertices);
        free(obj->vertex_normals);
        free(obj->faces_vertices);
        free(obj->faces_vertex_normals);
        free(obj->last_face_in_group);
        *obj = mliObject_init();
}

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces,
        const uint64_t num_groups)
{
        mli_check(num_vertices < UINT32_MAX, "Expected num_vertices < uint32");
        mli_check(num_vertex_normals < UINT32_MAX,
                "Expected num_vertex_normals < uint32");
        mli_check(num_faces < UINT32_MAX, "Expected num_faces < uint32");
        mli_check(num_groups < UINT32_MAX, "Expected num_groups < uint32");
        mli_check(num_groups <= num_faces, "Expected num_groups <= num_faces");

        mliObject_free(obj);
        obj->num_vertices = num_vertices;
        obj->num_vertex_normals = num_vertex_normals;
        obj->num_faces = num_faces;
        obj->num_groups = num_groups;
        mli_malloc(obj->vertices, struct mliVec, obj->num_vertices);
        mli_malloc(obj->vertex_normals, struct mliVec, obj->num_vertex_normals);
        mli_malloc(obj->faces_vertices, struct mliFace, obj->num_faces);
        mli_malloc(obj->faces_vertex_normals, struct mliFace, obj->num_faces);
        mli_malloc(obj->last_face_in_group, uint32_t, obj->num_groups);
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
        mli_log_err_vargs(("Face %ld (counting starts at 0)", i));
        return 0;
}

int mliObject_assert_normals(const struct mliObject *obj, const double epsilon)
{
        uint64_t i;
        for (i = 0; i < obj->num_vertex_normals; i++) {
                const double norm = mliVec_norm(obj->vertex_normals[i]);
                mli_check(
                        fabs(norm - 1.0) <= epsilon,
                        "Expected vertex_normals to be normalized.");
        }
        return 1;
error:
        mli_log_err_vargs(("Vertex-normal %ld (counting starts at 0)", i));
        return 0;
}

int mliObject_assert_valid_groups(const struct mliObject *obj)
{
        uint64_t i;
        for (i = 0; i < obj->num_groups; i++) {
                mli_check(
                        obj->last_face_in_group[i] < obj->num_faces,
                        "Expected last_face_in_group < num_faces.");
        }
        return 1;
error:
        mli_log_err_vargs(("Group %d's last face > num_faces.", i));
        return 0;
}

int mliObject_is_equal(const struct mliObject *a, const struct mliObject *b)
{
        uint64_t i;
        mli_c(a->num_vertices == b->num_vertices);
        mli_c(a->num_vertex_normals == b->num_vertex_normals);
        mli_c(a->num_faces == b->num_faces);
        mli_c(a->num_groups == b->num_groups);

        for (i = 0; i < a->num_vertices; i++) {
                mli_c(mliVec_is_equal(a->vertices[i], b->vertices[i]));
        }
        for (i = 0; i < a->num_vertex_normals; i++) {
                mli_c(mliVec_is_equal(
                        a->vertex_normals[i], b->vertex_normals[i]));
        }
        for (i = 0; i < a->num_faces; i++) {
                mli_c(mliFace_is_equal(
                        a->faces_vertices[i], b->faces_vertices[i]));
                mli_c(mliFace_is_equal(
                        a->faces_vertex_normals[i],
                        b->faces_vertex_normals[i]));
        }
        for (i = 0; i < a->num_groups; i++) {
                mli_c(a->last_face_in_group[i] == b->last_face_in_group[i]);
        }
        return 1;
error:
        return 0;
}

int mliObject_cpy(struct mliObject *destination, struct mliObject *source)
{
        uint64_t p;
        mli_check(
                destination->num_vertices == source->num_vertices,
                "Expected source and destination mliObject to have same "
                "num_vertices.");
        for (p = 0; p < destination->num_vertices; p++) {
                destination->vertices[p] = source->vertices[p];
        }

        mli_check(
                destination->num_vertex_normals == source->num_vertex_normals,
                "Expected source and destination mliObject to have same "
                "num_vertex_normals.");
        for (p = 0; p < destination->num_vertex_normals; p++) {
                destination->vertex_normals[p] = source->vertex_normals[p];
        }

        mli_check(
                destination->num_faces == source->num_faces,
                "Expected source and destination mliObject to have same "
                "num_faces.");
        for (p = 0; p < destination->num_faces; p++) {
                destination->faces_vertices[p] = source->faces_vertices[p];
                destination->faces_vertex_normals[p] =
                        source->faces_vertex_normals[p];
        }

        mli_check(
                destination->num_groups == source->num_groups,
                "Expected source and destination mliObject to have same "
                "num_groups.");
        for (p = 0; p < destination->num_groups; p++) {
                destination->last_face_in_group[p] =
                        source->last_face_in_group[p];

        }

        return 1;
error:
        return 0;
}
