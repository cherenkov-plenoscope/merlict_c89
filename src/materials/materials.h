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

        struct mli_BoundaryLayer2Array layers2;

        uint64_t default_refraction;
        uint64_t default_absorbtion;

        uint64_t num_surfaces;
        struct mli_Surface *surfaces;
        struct mli_String *surface_names;

        uint64_t num_media;
        struct mli_Medium *media;
        struct mli_String *medium_names;

        uint64_t num_boundary_layers;
        struct mli_BoundaryLayer *boundary_layers;
        struct mli_String *boundary_layer_names;

        uint64_t default_medium;
};

int mli_Materials_malloc(
        struct mli_Materials *res,
        const struct mli_MaterialsCapacity rescap);
void mli_Materials_free(struct mli_Materials *res);
struct mli_Materials mli_Materials_init(void);
void mli_Materials_info_fprint(FILE *f, const struct mli_Materials *res);
#endif
