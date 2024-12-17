/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_H_
#define MLI_OBJECT_H_

#include <stdint.h>
#include "../vec/vec.h"
#include "object_face.h"
#include "../string/string.h"

struct mli_Object {
        uint32_t num_vertices;
        struct mli_Vec *vertices;

        uint32_t num_vertex_normals;
        struct mli_Vec *vertex_normals;

        uint32_t num_faces;
        struct mli_object_Face *faces_vertices;
        struct mli_object_Face *faces_vertex_normals;
        uint16_t *faces_materials;

        uint32_t num_materials;
        struct mli_String *material_names;
};

int mli_Object_malloc(
        struct mli_Object *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces,
        const uint64_t num_materials);
void mli_Object_free(struct mli_Object *obj);
struct mli_Object mli_Object_init(void);
int mli_Object_equal(const struct mli_Object *a, const struct mli_Object *b);
uint32_t mli_Object_resolve_material_idx(
        const struct mli_Object *obj,
        const uint32_t face_idx);
#endif
