/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_H_
#define MLI_MATERIALS_H_

#include <stdint.h>
#include "../object/object.h"
#include "../func/func.h"
#include "../func/func_info.h"
#include "../surface/surface.h"
#include "../medium/medium.h"
#include "../boundarylayer/boundarylayer.h"
#include "../string/string.h"
#include "../spectrum/spectrum_array.h"
#include "../boundarylayer/boundarylayer_array.h"
#include "../boundarylayer/boundarylayer_surface_array.h"

struct mli_MaterialsCapacity {
        uint64_t num_spectra;
        uint64_t num_surfaces;
        uint64_t num_media;
        uint64_t num_boundary_layers;
};

struct mli_MaterialsCapacity mli_MaterialsCapacity_init(void);

struct mli_Materials {
        struct mli_SpectrumArray spectra;
        struct mli_SurfaceArray surfaces2;
        struct mli_MediumArray media2;
        struct mli_BoundaryLayer2Array layers2;

        uint64_t default_medium;
};

int mli_Materials_malloc(
        struct mli_Materials *self,
        const struct mli_MaterialsCapacity rescap);
void mli_Materials_free(struct mli_Materials *self);
struct mli_Materials mli_Materials_init(void);
int mli_Materials_info_fprint(FILE *f, const struct mli_Materials *self);
#endif
