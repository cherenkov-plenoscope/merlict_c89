/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object_serialize.h"
#include "object_valid.h"
#include "../magicid/magicid.h"
#include "../string/string_serialize.h"

chk_rc mli_Object_to_io(const struct mli_Object *obj, struct mli_IO *f)
{
        uint64_t i;
        struct mli_MagicId magic;
        chk(mli_MagicId_set(&magic, "mli_Object"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&obj->num_vertices, sizeof(uint32_t), 1u, f);
        chk_IO_write(&obj->num_vertex_normals, sizeof(uint32_t), 1u, f);
        chk_IO_write(&obj->num_faces, sizeof(uint32_t), 1u, f);
        chk_IO_write(&obj->num_materials, sizeof(uint32_t), 1u, f);

        chk_IO_write(
                obj->vertices, sizeof(struct mli_Vec), obj->num_vertices, f);

        chk_IO_write(
                obj->vertex_normals,
                sizeof(struct mli_Vec),
                obj->num_vertex_normals,
                f);

        chk_IO_write(
                obj->faces_vertices,
                sizeof(struct mli_object_Face),
                obj->num_faces,
                f);
        chk_IO_write(
                obj->faces_vertex_normals,
                sizeof(struct mli_object_Face),
                obj->num_faces,
                f);
        chk_IO_write(obj->faces_materials, sizeof(uint16_t), obj->num_faces, f);

        for (i = 0; i < obj->num_materials; i++) {
                chk(mli_String_to_io(&obj->material_names[i], f));
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Object_from_io(struct mli_Object *obj, struct mli_IO *f)
{
        uint64_t i;
        uint32_t num_vertices;
        uint32_t num_vertex_normals;
        uint32_t num_faces;
        uint32_t num_materials;
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Object"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&num_vertices, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_vertex_normals, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_faces, sizeof(uint32_t), 1u, f);
        chk_IO_read(&num_materials, sizeof(uint32_t), 1u, f);

        chk(mli_Object_malloc(
                obj,
                num_vertices,
                num_vertex_normals,
                num_faces,
                num_materials));

        chk_IO_read(
                obj->vertices, sizeof(struct mli_Vec), obj->num_vertices, f);
        chk_IO_read(
                obj->vertex_normals,
                sizeof(struct mli_Vec),
                obj->num_vertex_normals,
                f);

        chk_IO_read(
                obj->faces_vertices,
                sizeof(struct mli_object_Face),
                obj->num_faces,
                f);
        chk_IO_read(
                obj->faces_vertex_normals,
                sizeof(struct mli_object_Face),
                obj->num_faces,
                f);
        chk_IO_read(obj->faces_materials, sizeof(uint16_t), obj->num_faces, f);

        for (i = 0; i < obj->num_materials; i++) {
                chk(mli_String_from_io(&obj->material_names[i], f));
        }

        chk_msg(mli_Object_has_valid_faces(obj),
                "A face refers to a not existing vertex/vertex_normal.");
        return CHK_SUCCESS;
chk_error:
        mli_Object_free(obj);
        return CHK_FAIL;
}
