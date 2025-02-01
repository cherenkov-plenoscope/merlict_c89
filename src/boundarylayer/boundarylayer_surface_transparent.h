/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_TRANSPARENT_H_
#define MLI_BOUNDARYLAYER_SURFACE_TRANSPARENT_H_

#include "../io/io.h"
struct mli_Map;
struct mli_String;

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT 1000

struct mli_BoundaryLayer_Surface_Transparent {
        uint64_t nothing;
};

int mli_BoundaryLayer_Surface_Transparent_equal(
        const struct mli_BoundaryLayer_Surface_Transparent *a,
        const struct mli_BoundaryLayer_Surface_Transparent *b);

int mli_BoundaryLayer_Surface_Transparent_to_io(
        const struct mli_BoundaryLayer_Surface_Transparent *self,
        struct mli_IO *f);
int mli_BoundaryLayer_Surface_Transparent_from_io(
        struct mli_BoundaryLayer_Surface_Transparent *self,
        struct mli_IO *f);

int mli_BoundaryLayer_Surface_Transparent_from_json_string(
        struct mli_BoundaryLayer_Surface_Transparent *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string);

#endif
