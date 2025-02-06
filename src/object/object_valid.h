/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_VALID_H_
#define MLI_OBJECT_VALID_H_

#include <stdint.h>
#include "../bool/bool.h"
#include "../chk/chk.h"
#include "object.h"

mli_bool mli_Object_is_valid(const struct mli_Object *obj);
mli_bool mli_Object_has_valid_vertices(const struct mli_Object *obj);
mli_bool mli_Object_has_valid_faces(const struct mli_Object *obj);
mli_bool mli_Object_has_valid_normals(
        const struct mli_Object *obj,
        const double epsilon);
mli_bool mli_Object_has_valid_materials(const struct mli_Object *obj);
chk_rc mli_Object_num_unused(
        const struct mli_Object *obj,
        uint32_t *num_unused_vertices,
        uint32_t *num_unused_vertex_normals,
        uint32_t *num_unused_materials);
#endif
