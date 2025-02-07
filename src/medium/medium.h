/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_MEDIUM_H_
#define MLI_MEDIUM_H_

#include "../string/string.h"
#include "../chk/chk.h"
#include "../bool/bool.h"
#include <stdint.h>
struct mli_IO;
struct mli_Materials;
struct mli_Map;

struct mli_Medium {
        struct mli_String name;
        uint64_t refraction_spectrum;
        uint64_t absorption_spectrum;
};

struct mli_Medium mli_Medium_init(void);

void mli_Medium_free(struct mli_Medium *self);

mli_bool mli_Medium_valid_wrt_materials(
        const struct mli_Medium *self,
        const struct mli_Materials *materials);

mli_bool mli_Medium_equal(
        const struct mli_Medium *a,
        const struct mli_Medium *b);

chk_rc mli_Medium_to_io(const struct mli_Medium *self, struct mli_IO *f);
chk_rc mli_Medium_from_io(struct mli_Medium *self, struct mli_IO *f);

chk_rc mli_Medium_from_json_string_and_name(
        struct mli_Medium *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string,
        const struct mli_String *name);

#endif
