/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIUSERSCENERY_JSON_H_
#define MLIUSERSCENERY_JSON_H_

#include <stdint.h>
#include "../json/json.h"
#include "mliUserScenery.h"
#include "mliBoundaryLayer.h"

int mliSide_from_json(
        struct mliSide *side,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t side_token);
int mliBoundaryLayer_from_json(
        struct mliBoundaryLayer *boundary_layer,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t token_surface);
int mliMaterials_assign_boundary_layers_from_json(
        struct mliMaterials *materials,
        struct mli_Map *boundary_layer_names,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json);

#endif
