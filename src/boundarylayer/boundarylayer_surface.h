/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_H_
#define MLI_BOUNDARYLAYER_SURFACE_H_

#include "boundarylayer_surface_transparent.h"
#include "boundarylayer_surface_lambertian.h"
#include "boundarylayer_surface_mirror.h"
#include "boundarylayer_surface_phong.h"
#include "boundarylayer_surface_cook_torrance.h"
#include "../string/string.h"
#include "../io/io.h"

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_NONE 0

union mli_BoundaryLayer_SurfaceData {
        struct mli_BoundaryLayer_Surface_Transparent transparent;
        struct mli_BoundaryLayer_Surface_Lambertian lambertian;
        struct mli_BoundaryLayer_Surface_Mirror mirror;
        struct mli_BoundaryLayer_Surface_Phong phong;
        struct mli_BoundaryLayer_Surface_Cook_Torrance cook_torrance;
};

struct mli_BoundaryLayer_Surface {
        uint64_t type;
        union mli_BoundaryLayer_SurfaceData data;
};
struct mli_BoundaryLayer_Surface mli_BoundaryLayer_Surface_init(void);

int mli_BoundaryLayer_Surface_equal(
        const struct mli_BoundaryLayer_Surface *a,
        const struct mli_BoundaryLayer_Surface *b);

int mli_BoundaryLayer_Surface_type_to_string(
        const uint64_t type,
        struct mli_String *s);
int mli_BoundaryLayer_Surface_type_from_string(
        const struct mli_String *s,
        uint64_t *id);

int mli_BoundaryLayer_Surface_to_io(
        const struct mli_BoundaryLayer_Surface *self,
        struct mli_IO *f);
int mli_BoundaryLayer_Surface_from_io(
        struct mli_BoundaryLayer_Surface *self,
        struct mli_IO *f);

#endif
