/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLORMATERIALS_H_
#define MLICOLORMATERIALS_H_

#include <stdint.h>
#include "mliMaterials.h"
#include "../color/color.h"
#include "../color/colorObserver.h"

struct mli_ColorSurface {
        struct mli_Color specular_reflection;
        struct mli_Color diffuse_reflection;
};

struct mli_ColorMedium {
        struct mli_Color refraction;
        struct mli_Color absorbtion;
};

struct mli_ColorMaterials {
        uint64_t num_surfaces;
        struct mli_ColorSurface *surfaces;

        uint64_t num_media;
        struct mli_ColorMedium *media;
};

struct mli_ColorMaterials mli_ColorMaterials_init(void);
int mli_ColorMaterials_malloc(
        struct mli_ColorMaterials *colmat,
        const struct mliMaterialsCapacity rescap);
int mli_ColorMaterials_malloc_from_Materials(
        struct mli_ColorMaterials *colmat,
        const struct mliMaterials *mat,
        const struct mli_ColorObserver *colobs);
void mli_ColorMaterials_free(struct mli_ColorMaterials *colmat);
#endif
