/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_MEDIUM_H_
#define MLI_BOUNDARYLAYER_MEDIUM_H_

#include "../string/string.h"
struct mli_IO;
struct mli_Materials;
struct mli_Map;

struct mli_BoundaryLayer_Medium {
        struct mli_String name;
        uint64_t refraction_spectrum;
        uint64_t absorbtion_spectrum;
};

struct mli_BoundaryLayer_Medium mli_BoundaryLayer_Medium_init(void);

void mli_BoundaryLayer_Medium_free(struct mli_BoundaryLayer_Medium *self);

int mli_BoundaryLayer_Medium_valid_wrt_materials(
        const struct mli_BoundaryLayer_Medium *self,
        const struct mli_Materials *materials);

int mli_BoundaryLayer_Medium_equal(
        const struct mli_BoundaryLayer_Medium *a,
        const struct mli_BoundaryLayer_Medium *b);

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
