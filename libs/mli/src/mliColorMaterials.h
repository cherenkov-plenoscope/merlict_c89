/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLORMATERIALS_H_
#define MLICOLORMATERIALS_H_

#include <stdint.h>
#include "mliMaterials.h"
#include "mliColor.h"
#include "mliColorObserver.h"

struct mliColorSurface {
        struct mliColor specular_reflection;
        struct mliColor diffuse_reflection;
};

struct mliColorMedium {
        struct mliColor refraction;
        struct mliColor absorbtion;
};

struct mliColorMaterials {
        uint64_t num_surfaces;
        struct mliColorSurface *surfaces;

        uint64_t num_media;
        struct mliColorMedium *media;
};

struct mliColorMaterials mliColorMaterials_init(void);
int mliColorMaterials_malloc(
        struct mliColorMaterials *colmat,
        const struct mliMaterialsCapacity rescap);
int mliColorMaterials_malloc_from_Materials(
        struct mliColorMaterials *colmat,
        const struct mliMaterials *mat,
        const struct mliColorObserver *colobs);
void mliColorMaterials_free(struct mliColorMaterials *colmat);
#endif
