/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_H_
#define MLI_SURFACE_H_

#include <stdint.h>
#include "../func/func.h"
#include "../color/color.h"
#include "../string/string.h"

#define MLI_SURFACE_PHONG 100u
#define MLI_SURFACE_TRANSPARENT 102u

struct mli_Surface {
        uint32_t material;

        struct mli_Func specular_reflection;
        struct mli_Func diffuse_reflection;
};

int mli_Surface_malloc(
        struct mli_Surface *surface,
        const uint32_t num_points_specular_reflection,
        const uint32_t num_points_diffuse_reflection);
void mli_Surface_free(struct mli_Surface *surface);
struct mli_Surface mli_Surface_init(void);
int mli_Surface_equal(const struct mli_Surface *a, const struct mli_Surface *b);

int mli_material_type_to_string(const uint32_t type, struct mli_String *s);
int mli_material_type_from_string(const struct mli_String *s, uint32_t *id);

#endif
