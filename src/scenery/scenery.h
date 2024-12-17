/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_H_
#define MLI_SCENERY_H_

#include "../geometry/geometry.h"
#include "../geometrytomaterialmap/geometrytomaterialmap.h"
#include "../materials/materials.h"
#include "../accelerator/accelerator.h"

struct mli_Scenery {
        struct mli_Geometry geometry;
        struct mli_Accelerator accelerator;
        struct mli_Materials materials;
        struct mli_GeometryToMaterialMap geomap;
};

struct mli_Scenery mli_Scenery_init(void);
void mli_Scenery_free(struct mli_Scenery *self);
void mli_Scenery_info_fprint(FILE *f, const struct mli_Scenery *self);
#endif
