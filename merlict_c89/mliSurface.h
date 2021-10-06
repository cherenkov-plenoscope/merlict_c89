/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISURFACE_H_
#define MERLICT_C89_MLISURFACE_H_

#include <stdint.h>
#include "mliFunc.h"
#include "mliColor.h"
#include "mliJson.h"

#define MLI_MATERIAL_PHONG 100u
#define MLI_MATERIAL_TRANSPARENT 102u

struct mliSurface {
        uint32_t material;

        struct mliFunc specular_reflection;
        struct mliFunc diffuse_reflection;

        /*
         *  The color is only relevant for fast rendering of images.
         *  Color will not effect the propagation of photons.
         */
        struct mliColor color;
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

int mli_material_type_to_string(const uint32_t type, char *s);
int mli_material_type_from_string(const char *s, uint32_t *id);

int mliSurface_malloc_from_json_str(
        struct mliSurface *surface,
        const char *json_str);
int mliSurface_malloc_from_json_token(
        struct mliSurface *surface,
        const struct mliJson *json,
        const uint64_t token);
#endif
