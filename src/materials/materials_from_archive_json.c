/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_from_archive_json.h"
#include "../map/map_json.h"
#include "../vec/vec_json.h"
#include "../color/color_json.h"
#include "../materials/materials.h"
#include "../chk/chk.h"

int mli_Materials_from_Archive__assign_boundary_layers_from_json(
        struct mli_Materials *materials,
        struct mli_Map *boundary_layer_names,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json)
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
                uint64_t token_s_name = mli_Json_token_by_index(json, token, s);
                uint64_t token_s = token_s_name + 1;

                chk_msg(json->tokens[token_s_name].type == JSMN_STRING,
                        "Expected boundary_layer to be a String.");

                chk_msg(mli_Map_insert_key_from_json(
                                boundary_layer_names, json, token_s_name, s),
                        "Failed to insert boundary_layer's name into map.");

                chk_msg(json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected boundary_layer to be of type object {}.");

                chk_msg(mli_Materials_from_Archive__BoundaryLayer_from_json(
                                &materials->boundary_layers[s],
                                surface_names,
                                medium_names,
                                json,
                                token_s),
                        "Failed to copy boundary_layer from json.");
        }

        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mli_Materials_from_Archive__boundarylayer_Side_from_json(
        struct mli_boundarylayer_Side *side,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t side_token)
{
        uint64_t token_medium, token_surface;

        chk_msg(mli_Json_token_by_key(
                        json, side_token + 1, "medium", &token_medium),
                "Expected key 'medium' in side.");
        chk_msg(mli_Map_get_value_for_string_from_json(
                        medium_names, json, token_medium + 1, &side->medium),
                "Failed to get medium-idx from map");

        chk_msg(mli_Json_token_by_key(
                        json, side_token + 1, "surface", &token_surface),
                "Expected key 'surface' in side.");
        chk_msg(mli_Map_get_value_for_string_from_json(
                        surface_names, json, token_surface + 1, &side->surface),
                "Failed to get surface-idx from map");

        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, side_token + 1);
        return 0;
}

int mli_Materials_from_Archive__BoundaryLayer_from_json(
        struct mli_BoundaryLayer *boundary_layer,
        const struct mli_Map *surface_names,
        const struct mli_Map *medium_names,
        const struct mli_Json *json,
        const uint64_t token_surface)
{
        uint64_t token_inner_side, token_outer_side;
        chk_msg(mli_Json_token_by_key(
                        json, token_surface, "inner", &token_inner_side),
                "Expected key 'inner' in surface.");
        chk_msg(mli_Json_token_by_key(
                        json, token_surface, "outer", &token_outer_side),
                "Expected key 'outer' in surface.");

        chk_msg(mli_Materials_from_Archive__boundarylayer_Side_from_json(
                        &boundary_layer->inner,
                        surface_names,
                        medium_names,
                        json,
                        token_inner_side),
                "Failed to parse inner side.");
        chk_msg(mli_Materials_from_Archive__boundarylayer_Side_from_json(
                        &boundary_layer->outer,
                        surface_names,
                        medium_names,
                        json,
                        token_outer_side),
                "Failed to parse outer side.");
        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token_surface);
        return 0;
}
