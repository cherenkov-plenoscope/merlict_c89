/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_TRANSPARENT_H_
#define MLI_SURFACE_TRANSPARENT_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "../io/io.h"

struct mli_Map;
struct mli_String;
struct mli_Materials;

#define MLI_SURFACE_TYPE_TRANSPARENT 1000

struct mli_Surface_Transparent {
        uint64_t nothing;
};

mli_bool mli_Surface_Transparent_equal(
        const struct mli_Surface_Transparent *a,
        const struct mli_Surface_Transparent *b);

chk_rc mli_Surface_Transparent_to_io(
        const struct mli_Surface_Transparent *self,
        struct mli_IO *f);
chk_rc mli_Surface_Transparent_from_io(
        struct mli_Surface_Transparent *self,
        struct mli_IO *f);

chk_rc mli_Surface_Transparent_from_json_string(
        struct mli_Surface_Transparent *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string);

chk_rc mli_Surface_Transparent_valid_wrt_materials(
        const struct mli_Surface_Transparent *self,
        const struct mli_Materials *materials);

#endif
