/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBJECT_H_
#define MERLICT_C89_MLIOBJECT_H_

#include <stdint.h>
#include "mliVec.h"
#include "mliFace.h"
#include "mliName.h"

struct mliObject {
        uint32_t num_vertices;
        struct mliVec *vertices;

        uint32_t num_vertex_normals;
        struct mliVec *vertex_normals;

        uint32_t num_faces;
        struct mliFace *faces_vertices;
        struct mliFace *faces_vertex_normals;

        uint32_t num_materials;
        uint32_t *first_face_in_next_material;
        struct mliName *material_names;
};

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces,
        const uint64_t num_materials);
void mliObject_free(struct mliObject *obj);
struct mliObject mliObject_init(void);
int mliObject_assert_valid_faces(const struct mliObject *obj);
int mliObject_assert_normals(const struct mliObject *obj, const double epsilon);
int mliObject_assert_valid_materials(const struct mliObject *obj);
int mliObject_is_equal(const struct mliObject *a, const struct mliObject *b);
int mliObject_resolve_material(
        const struct mliObject *obj,
        const uint32_t face_idx,
        uint32_t *material_idx);
#endif
