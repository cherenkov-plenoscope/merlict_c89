/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIUSERSCENERY_JSON_H_
#define MLIUSERSCENERY_JSON_H_

#include <stdint.h>
#include "../json/json.h"
#include "mliUserScenery.h"
#include "../boundarylayer/boundarylayer.h"

int mli_boundarylayer_Side_from_json(
        struct mli_boundarylayer_Side *side,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t side_token);
int mli_BoundaryLayer_from_json(
        struct mli_BoundaryLayer *boundary_layer,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t token_surface);
int mli_Materials_assign_boundary_layers_from_json(
        struct mli_Materials *materials,
        struct mli_Map *boundary_layer_names,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json);

#endif
