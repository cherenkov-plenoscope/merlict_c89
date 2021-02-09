/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMATERIALS_H_
#define MERLICT_C89_MLIMATERIALS_H_

#include <stdint.h>
#include "mliObject.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliSurface.h"
#include "mliMedium.h"

struct mliMaterialsCapacity {
        uint32_t num_functions;
        uint32_t num_colors;
        uint32_t num_surfaces;
        uint32_t num_media;
};

struct mliMaterialsCapacity mliMaterialsCapacity_init(void);

struct mliMaterials {
        uint32_t num_functions;
        struct mliFunc *functions;

        uint32_t num_colors;
        struct mliColor *colors;

        uint32_t num_surfaces;
        struct mliSurface *surfaces;

        uint32_t num_media;
        struct mliMedium *media;

        uint32_t default_medium;
};

int mliMaterials_malloc(
        struct mliMaterials *res,
        const struct mliMaterialsCapacity rescap);
void mliMaterials_free(struct mliMaterials *res);
struct mliMaterials mliMaterials_init(void);
int mliMaterials_cpy(
        struct mliMaterials *destination,
        const struct mliMaterials *source);
void mliMaterials_info_fprint(FILE *f, const struct mliMaterials *res);
#endif
