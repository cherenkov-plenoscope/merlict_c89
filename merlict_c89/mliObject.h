/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBJECT_H_
#define MERLICT_C89_MLIOBJECT_H_

#include <stdint.h>
#include "mliVec.h"
#include "mliFace.h"

struct mliObject {
        uint32_t num_vertices;
        struct mliVec *vertices;

        uint32_t num_vertex_normals;
        struct mliVec *vertex_normals;

        uint32_t num_faces;
        struct mliFace *faces_vertices;
        struct mliFace *faces_vertex_normals;
};

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces);
void mliObject_free(struct mliObject *obj);
struct mliObject mliObject_init(void);
int mliObject_assert_valid_faces(const struct mliObject *obj);
int mliObject_assert_normals(const struct mliObject *obj, const double epsilon);

int mliObject_fwrite(const struct mliObject *obj, FILE *f);
int mliObject_malloc_fread(struct mliObject *obj, FILE *f);
int mliObject_is_equal(const struct mliObject *a, const struct mliObject *b);
int mliObject_cpy(struct mliObject *destination, struct mliObject *source);
#endif
