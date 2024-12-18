/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_FORM_ARCHIVE_H_
#define MLI_MATERIALS_FORM_ARCHIVE_H_

#include "../map/map.h"
#include "materials_names.h"

struct mli_Materials;
struct mli_Archive;
int mli_Materials_from_Archive(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);
#endif
