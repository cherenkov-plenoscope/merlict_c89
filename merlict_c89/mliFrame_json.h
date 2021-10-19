/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFRAME_JSON_H_
#define MERLICT_C89_MLIFRAME_JSON_H_

#include <stdint.h>
#include "mliJson.h"
#include "mliFrame.h"

int mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliDynMap *object_names,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names);
int _mliFrame_id_from_json(
        uint32_t *id,
        const struct mliJson *json,
        const uint64_t token);
int _mliFrame_pos_rot_from_json(
        struct mliHomTraComp *frame2mother,
        const struct mliJson *json,
        const uint64_t token);
int _mliFrame_type_from_json(
        uint64_t *type,
        const struct mliJson *json,
        const uint64_t token_child);
int _mliFrame_boundary_layers_form_json(
        struct mliDynUint32 *boundary_layers,
        const uint32_t object_idx,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names,
        const struct mliJson *json,
        const uint64_t token);
#endif