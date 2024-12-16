/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_H_
#define MLIOBJECT_H_

#include <stdint.h>
#include "../vec/vec.h"
#include "mliFace.h"
#include "../string/string.h"

struct mliObject {
        uint32_t num_vertices;
        struct mli_Vec *vertices;

        uint32_t num_vertex_normals;
        struct mli_Vec *vertex_normals;

        uint32_t num_faces;
        struct mliFace *faces_vertices;
        struct mliFace *faces_vertex_normals;
        uint16_t *faces_materials;

        uint32_t num_materials;
        struct mli_String *material_names;
};

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces,
        const uint64_t num_materials);
void mliObject_free(struct mliObject *obj);
struct mliObject mliObject_init(void);
int mliObject_equal(const struct mliObject *a, const struct mliObject *b);
uint32_t mliObject_resolve_material_idx(
        const struct mliObject *obj,
        const uint32_t face_idx);
#endif
