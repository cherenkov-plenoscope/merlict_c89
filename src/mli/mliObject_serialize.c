/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_serialize.h"
#include "mliMagicId.h"
#include "mliObject_valid.h"
#include "../chk/chk.h"
#include "../string/string_serialize.h"

int mliObject_fwrite(const struct mliObject *obj, FILE *f)
{
        uint64_t i;
        struct mliMagicId magic;
        chk(mliMagicId_set(&magic, "mliObject"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_fwrite(&obj->num_vertices, sizeof(uint32_t), 1u, f);
        chk_fwrite(&obj->num_vertex_normals, sizeof(uint32_t), 1u, f);
        chk_fwrite(&obj->num_faces, sizeof(uint32_t), 1u, f);
        chk_fwrite(&obj->num_materials, sizeof(uint32_t), 1u, f);

        chk_fwrite(obj->vertices, sizeof(struct mliVec), obj->num_vertices, f);

        chk_fwrite(
                obj->vertex_normals,
                sizeof(struct mliVec),
                obj->num_vertex_normals,
                f);

        chk_fwrite(
                obj->faces_vertices, sizeof(struct mliFace), obj->num_faces, f);
        chk_fwrite(
                obj->faces_vertex_normals,
                sizeof(struct mliFace),
                obj->num_faces,
                f);
        chk_fwrite(obj->faces_materials, sizeof(uint16_t), obj->num_faces, f);

        for (i = 0; i < obj->num_materials; i++) {
                chk(mli_String_fwrite(&obj->material_names[i], f));
        }

        return 1;
chk_error:
        return 0;
}

int mliObject_malloc_fread(struct mliObject *obj, FILE *f)
{
        uint64_t i;
        uint32_t num_vertices;
        uint32_t num_vertex_normals;
        uint32_t num_faces;
        uint32_t num_materials;
        struct mliMagicId magic;
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliObject"));
        mliMagicId_warn_version(&magic);

        chk_fread(&num_vertices, sizeof(uint32_t), 1u, f);
        chk_fread(&num_vertex_normals, sizeof(uint32_t), 1u, f);
        chk_fread(&num_faces, sizeof(uint32_t), 1u, f);
        chk_fread(&num_materials, sizeof(uint32_t), 1u, f);

        chk(mliObject_malloc(
                obj,
                num_vertices,
                num_vertex_normals,
                num_faces,
                num_materials));

        chk_fread(obj->vertices, sizeof(struct mliVec), obj->num_vertices, f);
        chk_fread(
                obj->vertex_normals,
                sizeof(struct mliVec),
                obj->num_vertex_normals,
                f);

        chk_fread(
                obj->faces_vertices, sizeof(struct mliFace), obj->num_faces, f);
        chk_fread(
                obj->faces_vertex_normals,
                sizeof(struct mliFace),
                obj->num_faces,
                f);
        chk_fread(obj->faces_materials, sizeof(uint16_t), obj->num_faces, f);

        for (i = 0; i < obj->num_materials; i++) {
                chk(mli_String_malloc_fread(&obj->material_names[i], f));
        }

        chk_msg(mliObject_has_valid_faces(obj),
                "A face refers to a not existing vertex/vertex_normal.");
        return 1;
chk_error:
        mliObject_free(obj);
        return 0;
}
