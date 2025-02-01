/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_H_
#define MLI_MATERIALS_H_

#include <stdint.h>
#include "../object/object.h"
#include "../func/func.h"
#include "../func/func_info.h"
#include "../boundarylayer/boundarylayer.h"
#include "../string/string.h"
#include "../spectrum/spectrum_array.h"
#include "../boundarylayer/boundarylayer_array.h"
#include "../boundarylayer/boundarylayer_surface_array.h"
#include "../boundarylayer/boundarylayer_medium_array.h"

struct mli_MaterialsCapacity {
        uint64_t num_spectra;
        uint64_t num_surfaces;
        uint64_t num_media;
        uint64_t num_boundary_layers;
};

struct mli_MaterialsCapacity mli_MaterialsCapacity_init(void);

struct mli_Materials {
        struct mli_SpectrumArray spectra;
        struct mli_SurfaceArray surfaces;
        struct mli_MediumArray media;
        struct mli_BoundaryLayer2Array boundary_layers;

        uint64_t default_medium;
};

int mli_Materials_malloc(
        struct mli_Materials *self,
        const struct mli_MaterialsCapacity rescap);
void mli_Materials_free(struct mli_Materials *self);
struct mli_Materials mli_Materials_init(void);
int mli_Materials_info_fprint(FILE *f, const struct mli_Materials *self);

int mli_Materials__has_surface_name_cstr(
        const struct mli_Materials *self,
        const char *name);
int mli_Materials__has_medium_name_cstr(
        const struct mli_Materials *self,
        const char *name);

#endif
