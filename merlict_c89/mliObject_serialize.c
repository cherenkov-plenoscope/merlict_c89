/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_serialize.h"
#include "mli_debug.h"
#include "mliMagicId.h"

int mliObject_fwrite(const struct mliObject *obj, FILE *f)
{
        struct mliMagicId magic;
        mli_c(mliMagicId_set(&magic, "mliObject"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        mli_fwrite(&obj->num_vertices, sizeof(uint32_t), 1u, f);
        mli_fwrite(&obj->num_vertex_normals, sizeof(uint32_t), 1u, f);
        mli_fwrite(&obj->num_faces, sizeof(uint32_t), 1u, f);
        mli_fwrite(&obj->num_materials, sizeof(uint32_t), 1u, f);

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

        mli_fwrite(
                obj->first_face_in_next_material,
                sizeof(uint32_t),
                obj->num_materials,
                f);
        mli_fwrite(
                obj->material_names,
                sizeof(struct mliName),
                obj->num_materials,
                f);

        return 1;
error:
        return 0;
}

int mliObject_malloc_fread(struct mliObject *obj, FILE *f)
{
        uint32_t num_vertices;
        uint32_t num_vertex_normals;
        uint32_t num_faces;
        uint32_t num_materials;
        struct mliMagicId magic;
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliObject"));
        mliMagicId_warn_version(&magic);

        mli_fread(&num_vertices, sizeof(uint32_t), 1u, f);
        mli_fread(&num_vertex_normals, sizeof(uint32_t), 1u, f);
        mli_fread(&num_faces, sizeof(uint32_t), 1u, f);
        mli_fread(&num_materials, sizeof(uint32_t), 1u, f);

        mli_c(mliObject_malloc(
                obj,
                num_vertices,
                num_vertex_normals,
                num_faces,
                num_materials));

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

        mli_fread(
                obj->first_face_in_next_material,
                sizeof(uint32_t),
                obj->num_materials,
                f);
        mli_fread(
                obj->material_names,
                sizeof(struct mliName),
                obj->num_materials,
                f);

        mli_check(
                mliObject_has_valid_faces(obj),
                "A face refers to a not existing vertex/vertex_normal.");
        return 1;
error:
        mliObject_free(obj);
        return 0;
}
