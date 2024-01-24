/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery_json.h"
#include "mliDynMap_json.h"
#include "mliVec_json.h"
#include "mliColor_json.h"
#include "../../chk_debug/src/chk_debug.h"

int mliMaterials_assign_boundary_layers_from_json(
        struct mliMaterials *materials,
        struct mliDynMap *boundary_layer_names,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json)
{
        uint64_t token = 0;
        uint64_t s;

        chk_msg(json->tokens[token].type == JSMN_OBJECT,
                "Expected boundary_layers.json to be a json-object.");

        chk_msg(materials->num_boundary_layers ==
                        (uint32_t)json->tokens[token].size,
                "Expected num_boundary_layers to match "
                "json-object.size.");

        for (s = 0; s < materials->num_boundary_layers; s++) {
                uint64_t token_s_name = mliJson_token_by_index(json, token, s);
                uint64_t token_s = token_s_name + 1;

                chk_msg(json->tokens[token_s_name].type == JSMN_STRING,
                        "Expected boundary_layer to be a String.");

                chk_msg(mliDynMap_insert_key_from_json(
                                boundary_layer_names, json, token_s_name, s),
                        "Failed to insert boundary_layer's name into map.");

                chk_msg(json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected boundary_layer to be of type object {}.");

                chk_msg(mliBoundaryLayer_from_json(
                                &materials->boundary_layers[s],
                                surface_names,
                                medium_names,
                                json,
                                token_s),
                        "Failed to copy boundary_layer from json.");
        }

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliSide_from_json(
        struct mliSide *side,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t side_token)
{
        uint64_t token_medium, token_surface;

        chk_msg(mliJson_token_by_key(
                        json, side_token + 1, "medium", &token_medium),
                "Expected key 'medium' in side.");
        chk_msg(mliDynMap_get_value_for_string_from_json(
                        medium_names, json, token_medium + 1, &side->medium),
                "Failed to get medium-idx from map");

        chk_msg(mliJson_token_by_key(
                        json, side_token + 1, "surface", &token_surface),
                "Expected key 'surface' in side.");
        chk_msg(mliDynMap_get_value_for_string_from_json(
                        surface_names, json, token_surface + 1, &side->surface),
                "Failed to get surface-idx from map");

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, side_token + 1);
        return 0;
}

int mliBoundaryLayer_from_json(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t token_surface)
{
        uint64_t token_inner_side, token_outer_side;
        chk_msg(mliJson_token_by_key(
                        json, token_surface, "inner", &token_inner_side),
                "Expected key 'inner' in surface.");
        chk_msg(mliJson_token_by_key(
                        json, token_surface, "outer", &token_outer_side),
                "Expected key 'outer' in surface.");

        chk_msg(mliSide_from_json(
                        &boundary_layer->inner,
                        surface_names,
                        medium_names,
                        json,
                        token_inner_side),
                "Failed to parse inner side.");
        chk_msg(mliSide_from_json(
                        &boundary_layer->outer,
                        surface_names,
                        medium_names,
                        json,
                        token_outer_side),
                "Failed to parse outer side.");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token_surface);
        return 0;
}
