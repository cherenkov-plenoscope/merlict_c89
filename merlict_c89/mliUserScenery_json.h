/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIUSERSCENERY_JSON_H_
#define MERLICT_C89_MLIUSERSCENERY_JSON_H_

#include <stdint.h>
#include "mliJson.h"
#include "mliUserScenery.h"

int __mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliDynMap *object_names,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names);
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
int __mliBoundaryLayer_from_json(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t token_surface);

int __mliMaterialsCapacity_from_materials_json(
        struct mliMaterialsCapacity *rescap,
        const struct mliJson *json);
int __mliMaterials_assign_surfaces_from_json(
        struct mliMaterials *resources,
        struct mliDynMap *surface_names,
        const struct mliDynMap *function_names,
        const struct mliDynMap *color_names,
        const struct mliJson *json);
int __mliMaterials_assign_boundary_layers_from_json(
        struct mliMaterials *materials,
        struct mliDynMap *boundary_layer_names,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json);
int __mliSurface_from_json(
        struct mliSurface *surface,
        const struct mliDynMap *function_names,
        const struct mliDynMap *color_names,
        const struct mliJson *json,
        const uint64_t token_s);
int __mliMaterials_assign_media_from_json(
        struct mliMaterials *resources,
        struct mliDynMap *medium_names,
        const struct mliDynMap *function_names,
        const struct mliJson *json);
int __mliMedium_from_json(
        struct mliMedium *medium,
        const struct mliDynMap *function_names,
        const struct mliJson *json,
        const uint64_t token_s);
int __mliMaterials_assign_colors_from_json(
        struct mliMaterials *resources,
        struct mliDynMap *color_names,
        const struct mliJson *json);
int _mliDynMap_get_value_for_string_from_json(
        const struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_value);
int _mliDynMap_insert_key_from_json(
        struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        const uint64_t value);
int __mliMaterialsCapacity_from_json(
        struct mliMaterialsCapacity *res_cap,
        const struct mliJson *json);
#endif
