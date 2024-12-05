/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMATERIALS_H_
#define MLIMATERIALS_H_

#include <stdint.h>
#include "mliObject.h"
#include "../func/func.h"
#include "mliSurface.h"
#include "mliMedium.h"
#include "mliBoundaryLayer.h"
#include "mliName.h"

struct mliMaterialsCapacity {
        uint64_t num_surfaces;
        uint64_t num_media;
        uint64_t num_boundary_layers;
};

struct mliMaterialsCapacity mliMaterialsCapacity_init(void);

struct mliMaterials {
        uint64_t num_surfaces;
        struct mliSurface *surfaces;
        struct mliName *surface_names;

        uint64_t num_media;
        struct mliMedium *media;
        struct mliName *medium_names;

        uint64_t num_boundary_layers;
        struct mliBoundaryLayer *boundary_layers;
        struct mliName *boundary_layer_names;

        uint64_t default_medium;
};

int mliMaterials_malloc(
        struct mliMaterials *res,
        const struct mliMaterialsCapacity rescap);
void mliMaterials_free(struct mliMaterials *res);
struct mliMaterials mliMaterials_init(void);
void mliMaterials_info_fprint(FILE *f, const struct mliMaterials *res);
#endif
