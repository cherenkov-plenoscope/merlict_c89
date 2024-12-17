/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_WAVEFRONT_H_
#define MLI_OBJECT_WAVEFRONT_H_

#include <stdint.h>
#include "object.h"
#include "object_face.h"
#include "../io/io.h"
#include "../string/string.h"
#include "../vec/vec.h"

int mli_Object_malloc_from_wavefront(struct mli_Object *obj, struct mli_IO *io);
int mli_Object_fprint_to_wavefront(
        struct mli_IO *f,
        const struct mli_Object *obj);
int mli_Object_parse_face_line(
        const struct mli_String *line,
        struct mli_object_Face *faces_vertices,
        struct mli_object_Face *faces_texture_points,
        struct mli_object_Face *faces_vertex_normals,
        int *line_mode);
int mli_Object_parse_three_float_line(
        const struct mli_String *line,
        struct mli_Vec *v);
#endif
