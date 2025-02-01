/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_H_
#define MLI_SURFACE_H_

#include "surface_transparent.h"
#include "surface_cooktorrance.h"
#include "../string/string.h"
struct mli_IO;
struct mli_Map;

#define MLI_SURFACE_TYPE_NONE 0

union mli_SurfaceData {
        struct mli_Surface_Transparent transparent;
        struct mli_Surface_CookTorrance cook_torrance;
};

struct mli_Surface {
        struct mli_String name;
        uint64_t type;
        union mli_SurfaceData data;
};
struct mli_Surface mli_Surface_init(void);

void mli_Surface_free(struct mli_Surface *self);

int mli_Surface_equal(const struct mli_Surface *a, const struct mli_Surface *b);

int mli_Surface_type_to_string(const uint64_t type, struct mli_String *s);
int mli_Surface_type_from_string(const struct mli_String *s, uint64_t *id);

int mli_Surface_to_io(const struct mli_Surface *self, struct mli_IO *f);
int mli_Surface_from_io(struct mli_Surface *self, struct mli_IO *f);

int mli_Surface_from_json_string_and_name(
        struct mli_Surface *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string,
        const struct mli_String *name);

#endif
