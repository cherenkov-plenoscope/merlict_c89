/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_H_
#define MLI_GEOMETRY_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../homtra/homtra.h"
#include "../materials/materials.h"
#include "../string/string.h"
#include "../object/object.h"

struct mli_Geometry {
        uint32_t num_objects;
        struct mli_Object *objects;
        struct mli_String *object_names;

        uint32_t num_robjects;
        uint32_t *robjects;
        uint32_t *robject_ids;
        struct mli_HomTraComp *robject2root;
};

chk_rc mli_Geometry_malloc(
        struct mli_Geometry *self,
        const uint32_t num_objects,
        const uint32_t num_robjects);
chk_rc mli_Geometry_malloc_references(
        struct mli_Geometry *self,
        const uint32_t num_robjects);
chk_rc mli_Geometry_malloc_objects(
        struct mli_Geometry *self,
        const uint32_t num_objects);

void mli_Geometry_free(struct mli_Geometry *self);
void mli_Geometry_free_objects(struct mli_Geometry *self);
void mli_Geometry_free_references(struct mli_Geometry *self);

struct mli_Geometry mli_Geometry_init(void);
void mli_Geometry_init_objects(struct mli_Geometry *self);
void mli_Geometry_init_references(struct mli_Geometry *self);

void mli_Geometry_info_fprint(FILE *f, const struct mli_Geometry *self);
struct mli_BoundaryLayer mli_Geometry_object_surfaces(
        const struct mli_Geometry *self,
        const uint32_t object_idx);
chk_rc mli_Geometry_warn_objects(const struct mli_Geometry *self);
#endif
