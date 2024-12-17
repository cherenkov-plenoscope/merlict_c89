/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISURFACE_H_
#define MLISURFACE_H_

#include <stdint.h>
#include "../func/func.h"
#include "../color/color.h"
#include "../string/string.h"

#define MLI_MATERIAL_PHONG 100u
#define MLI_MATERIAL_TRANSPARENT 102u

struct mliSurface {
        uint32_t material;

        struct mli_Func specular_reflection;
        struct mli_Func diffuse_reflection;
};

int mliSurface_malloc(
        struct mliSurface *surface,
        const uint32_t num_points_specular_reflection,
        const uint32_t num_points_diffuse_reflection);
void mliSurface_free(struct mliSurface *surface);
struct mliSurface mliSurface_init(void);
int mliSurface_equal(const struct mliSurface *a, const struct mliSurface *b);

int mliSurface_fwrite(const struct mliSurface *srf, FILE *f);
int mliSurface_malloc_fread(struct mliSurface *srf, FILE *f);

int mli_material_type_to_string(const uint32_t type, struct mli_String *s);
int mli_material_type_from_string(const struct mli_String *s, uint32_t *id);

#endif
