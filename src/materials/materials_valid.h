/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_VALID_H_
#define MLI_MATERIALS_VALID_H_

#include "../materials/materials.h"

int mli_Materials_valid(const struct mli_Materials *materials);
int mli_Materials_valid_default_medium(const struct mli_Materials *materials);
int mli_Materials_valid_spectra(const struct mli_Materials *materials);
int mli_Materials_valid_surfaces(const struct mli_Materials *materials);
int mli_Materials_valid_media(const struct mli_Materials *materials);
int mli_Materials_valid_boundary_layers(const struct mli_Materials *materials);
#endif
