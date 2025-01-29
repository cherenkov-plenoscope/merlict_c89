/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_FROM_ARCHIVE_JSON_H_
#define MLI_MATERIALS_FROM_ARCHIVE_JSON_H_

#include <stdint.h>
#include "../json/json.h"
#include "../map/map.h"
#include "../boundarylayer/boundarylayer.h"

struct mli_Materials;

int mli_Materials_from_Archive__boundarylayer_Side_from_json(
        struct mli_boundarylayer_Side *side,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t side_token);
int mli_Materials_from_Archive__BoundaryLayer_from_json(
        struct mli_BoundaryLayer *boundary_layer,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t token_surface);
int mli_Materials_from_Archive__assign_boundary_layers_from_json(
        struct mli_Materials *materials,
        struct mli_Map *boundary_layer_names,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json);

int mli_Materials_from_Archive__assign_default_medium_from_json(
        struct mli_Materials *materials,
        struct mli_Map *spectra_names,
        const struct mli_Json *json);

#endif
