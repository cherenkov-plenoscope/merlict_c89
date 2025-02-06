/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_FORM_ARCHIVE_H_
#define MLI_MATERIALS_FORM_ARCHIVE_H_

#include "../chk/chk.h"
#include "../map/map.h"
#include "materials_names.h"

struct mli_Materials;
struct mli_Archive;
struct mli_BoundaryLayer;

chk_rc mli_Materials__key_from_filename(
        struct mli_String *key,
        const struct mli_String *filename);

chk_rc mli_Materials_from_Archive__set_spectra(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);

chk_rc mli_Materials_from_Archive__set_media(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);

chk_rc mli_Materials_from_Archive__set_surfaces(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);

chk_rc mli_BoundaryLayer_from_json_string_and_name(
        struct mli_BoundaryLayer *self,
        const struct mli_Map *surface_names,
        const struct mli_Map *media_names,
        const struct mli_String *json_string,
        const struct mli_String *name);

chk_rc mli_Materials_from_Archive__set_boundary_layers(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);

chk_rc mli_Materials_from_Archive__set_default_medium(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);

chk_rc mli_Materials_from_Archive(
        struct mli_Materials *materials,
        struct mli_materials_Names *names,
        const struct mli_Archive *archive);
#endif
