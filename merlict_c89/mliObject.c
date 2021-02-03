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
        mli_eprintf(("Face %ld (counting starts at 0)", i));
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
        mli_eprintf(("Vertex-normal %ld (counting starts at 0)", i));
        return 0;
}

int mliObject_fwrite(const struct mliObject *obj, FILE *f)
{
        mli_fwrite(&obj->num_vertices, sizeof(uint32_t), 1u, f);
        mli_fwrite(&obj->num_vertex_normals, sizeof(uint32_t), 1u, f);
        mli_fwrite(&obj->num_faces, sizeof(uint32_t), 1u, f);

        mli_fwrite(obj->vertices, sizeof(struct mliVec), obj->num_vertices, f);

        mli_fwrite(
                obj->vertex_normals,
                sizeof(struct mliVec),
                obj->num_vertex_normals,
                f);

        mli_fwrite(
                obj->faces_vertices, sizeof(struct mliFace), obj->num_faces, f);
        mli_fwrite(
                obj->faces_vertex_normals,
                sizeof(struct mliFace),
                obj->num_faces,
                f);
        return 1;
error:
        return 0;
}

int mliObject_malloc_from_file(struct mliObject *obj, FILE *f)
{
        uint32_t num_vertices;
        uint32_t num_vertex_normals;
        uint32_t num_faces;

        mli_fread(&num_vertices, sizeof(uint32_t), 1u, f);
        mli_fread(&num_vertex_normals, sizeof(uint32_t), 1u, f);
        mli_fread(&num_faces, sizeof(uint32_t), 1u, f);

        mli_c(mliObject_malloc(
                obj, num_vertices, num_vertex_normals, num_faces));

        mli_fread(obj->vertices, sizeof(struct mliVec), obj->num_vertices, f);
        mli_fread(
                obj->vertex_normals,
                sizeof(struct mliVec),
                obj->num_vertex_normals,
                f);

        mli_fread(
                obj->faces_vertices, sizeof(struct mliFace), obj->num_faces, f);
        mli_fread(
                obj->faces_vertex_normals,
                sizeof(struct mliFace),
                obj->num_faces,
                f);

        mli_check(
                mliObject_assert_valid_faces(obj),
                "A face refers to a not existing vertex/vertex_normal.");
        return 1;
error:
        mliObject_free(obj);
        return 0;
}

int mliObject_is_equal(const struct mliObject *a, const struct mliObject *b)
{
        uint64_t i;
        mli_c(a->num_vertices == b->num_vertices);
        mli_c(a->num_vertex_normals == b->num_vertex_normals);
        mli_c(a->num_faces == b->num_faces);

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

        return 1;
error:
        return 0;
}
