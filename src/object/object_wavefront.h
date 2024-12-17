/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_WAVEFRONT_H_
#define MLI_OBJECT_WAVEFRONT_H_

#include <stdint.h>
#include "object.h"
#include "object_face.h"
#include "../io/io.h"
#include "../string/string.h"
#include "../vec/vec.h"

int mliObject_malloc_from_wavefront(struct mliObject *obj, struct mli_IO *io);
int mliObject_fprint_to_wavefront(
        struct mli_IO *f,
        const struct mliObject *obj);
int mliObject_parse_face_line(
        const struct mli_String *line,
        struct mli_object_Face *faces_vertices,
        struct mli_object_Face *faces_texture_points,
        struct mli_object_Face *faces_vertex_normals,
        int *line_mode);
int mliObject_parse_three_float_line(
        const struct mli_String *line,
        struct mli_Vec *v);
#endif
