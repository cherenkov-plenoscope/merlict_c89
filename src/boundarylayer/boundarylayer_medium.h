/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_MEDIUM_H_
#define MLI_BOUNDARYLAYER_MEDIUM_H_

#include "boundarylayer_medium_transparent.h"
#include "../string/string.h"
#include "../io/io.h"

#define MLI_BOUNDARYLAYER_MEDIUM_TYPE_NONE 0

union mli_BoundaryLayer_MediumData {
        struct mli_BoundaryLayerMedium_Transparent transparent;
};

struct mli_BoundaryLayer_Medium {
        struct mli_String name;
        uint64_t type;
        union mli_BoundaryLayer_MediumData data;
};

struct mli_BoundaryLayer_Medium mli_BoundaryLayer_Medium_init(void);

int mli_BoundaryLayer_Medium_equal(
        const struct mli_BoundaryLayer_Medium *a,
        const struct mli_BoundaryLayer_Medium *b);

int mli_BoundaryLayer_Medium_type_to_string(
        const uint64_t type,
        struct mli_String *s);
int mli_BoundaryLayer_Medium_type_from_string(
        const struct mli_String *s,
        uint64_t *id);

int mli_BoundaryLayer_Medium_to_io(
        const struct mli_BoundaryLayer_Medium *self,
        struct mli_IO *f);
int mli_BoundaryLayer_Medium_from_io(
        struct mli_BoundaryLayer_Medium *self,
        struct mli_IO *f);

int mli_BoundaryLayer_Medium_from_json_string_and_name(
        struct mli_BoundaryLayer_Medium *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string,
        const struct mli_String *name);

#endif
