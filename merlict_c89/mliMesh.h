/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMESH_H_
#define MERLICT_C89_MLIMESH_H_

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mli_math.h"
#include "mliFace.h"
#include "mliVec.h"

struct mliMesh {
        uint32_t num_vertices;
        struct mliVec *vertices;
        uint32_t num_faces;
        struct mliFace *faces;

        struct mliBoundaryLayer *boundary_layers;
        uint32_t* user_ids;
};

int mliMesh_malloc_from_object_file(const char *path, struct mliMesh *m);
int mli_parse_three_doubles(const char *line, double *a, double *b, double *c);
int mli_parse_three_ints(const char *line, int *a, int *b, int *c);
int mliMesh_malloc(
        struct mliMesh *m,
        const uint32_t num_vertices,
        const uint32_t num_faces);
void mliMesh_free(struct mliMesh *m);
struct mliMesh mliMesh_init(void);
#endif
