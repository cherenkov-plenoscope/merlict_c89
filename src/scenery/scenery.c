/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery.h"
#include <math.h>
#include <stdint.h>

struct mli_Scenery mli_Scenery_init(void)
{
        struct mli_Scenery out;
        out.geometry = mli_Geometry_init();
        out.accelerator = mli_Accelerator_init();
        out.materials = mli_Materials_init();
        out.geomap = mli_GeometryToMaterialMap_init();
        return out;
}

void mli_Scenery_free(struct mli_Scenery *self)
{
        mli_Geometry_free(&self->geometry);
        mli_Accelerator_free(&self->accelerator);
        mli_Materials_free(&self->materials);
        mli_GeometryToMaterialMap_free(&self->geomap);
}

void mli_Scenery_info_fprint(FILE *f, const struct mli_Scenery *self)
{
        mli_Geometry_info_fprint(f, &self->geometry);
        fprintf(f, "\n");
        mli_Accelerator_info_fprint(f, &self->accelerator);
        fprintf(f, "\n");
        mli_Materials_info_fprint(f, &self->materials);
        fprintf(f, "\n");
        mli_GeometryToMaterialMap_info_fprint(f, &self->geomap);
        fprintf(f, "\n");
}
