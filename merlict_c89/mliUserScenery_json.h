/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIUSERSCENERY_JSON_H_
#define MERLICT_C89_MLIUSERSCENERY_JSON_H_

#include <stdint.h>
#include "mliJson.h"
#include "mliUserScenery.h"

int __mliSide_set(
        struct mliSide *side,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t side_token);
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
int __mliMaterialsCapacity_from_json(
        struct mliMaterialsCapacity *res_cap,
        const struct mliJson *json);
#endif
