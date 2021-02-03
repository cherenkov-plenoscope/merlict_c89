/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_JSON_H_
#define MERLICT_C89_MLISCENERY_JSON_H_

#include <stdint.h>
#include <stdlib.h>

#include "mli_debug.h"
#include "mli_json.h"
#include "mliUserScenery.h"
#include "mliFunc_malloc_from_json_token.h"
#include "mliVec_from_json_token.h"

int mliUserScenery_malloc_from_json(
        struct mliUserScenery *uscn,
        const struct mliJson *json);
int __mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliDynMap *object_names,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names);
int __mliFrame_set_surface_idx(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t token);
int __mliSide_set(
        struct mliSide *side,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t side_token);
int __mliFrame_set_id(
        uint32_t *id,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_set_pos_rot(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token);
int __mliFrame_type_from_json(
        uint64_t *type,
        const struct mliJson *json,
        const uint64_t token_child);
int __mliScenery_assign_surfaces_from_json(
        struct mliSceneryResources *resources,
        struct mliDynMap *surface_names,
        const struct mliDynMap *function_names,
        const struct mliDynMap *color_names,
        const struct mliJson *json);
int __mliSurface_from_json(
        struct mliSurface *surface,
        const struct mliDynMap *function_names,
        const struct mliDynMap *color_names,
        const struct mliJson *json,
        const uint64_t token_s);
int __mliScenery_assign_media_from_json(
        struct mliSceneryResources *resources,
        struct mliDynMap *medium_names,
        const struct mliDynMap *function_names,
        const struct mliJson *json);
int __mliMedium_from_json(
        struct mliMedium *medium,
        const struct mliDynMap *function_names,
        const struct mliJson *json,
        const uint64_t token_s);
int __mliScenery_assign_colors_from_json(
        struct mliSceneryResources *resources,
        struct mliDynMap *color_names,
        const struct mliJson *json);
int _mliDynMap_get_value_for_string_from_json(
        const struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_value);
int _mliDynMap_key_from_json(
        struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        const uint64_t value);
int __mliSceneryResourcesCapacity_from_json(
        struct mliSceneryResourcesCapacity *res_cap,
        const struct mliJson *json);
#endif
