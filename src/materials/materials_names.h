/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_NAMES_H_
#define MLI_MATERIALS_NAMES_H_

#include "../map/map.h"
#include "../chk/chk.h"

struct mli_materials_Names {
        struct mli_Map spectra;
        struct mli_Map media;
        struct mli_Map surfaces;
        struct mli_Map boundary_layers;
};
struct mli_materials_Names mli_materials_Names_init(void);
chk_rc mli_materials_Names_malloc(struct mli_materials_Names *namemap);
void mli_materials_Names_free(struct mli_materials_Names *namemap);

#endif
