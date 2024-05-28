/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFRAME_JSON_H_
#define MLIFRAME_JSON_H_

#include <stdint.h>
#include "mli_json.h"
#include "mliFrame.h"
#include "mliObject.h"
#include "mliDynMap.h"

int mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliDynMap *object_names,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names);
int mliFrame_id_from_json_token(
        uint32_t *id,
        const struct mliJson *json,
        const uint64_t token);
int mliFrame_pos_rot_from_json_token(
        struct mliHomTraComp *frame2mother,
        const struct mliJson *json,
        const uint64_t token);
int mliFrame_type_from_json_token(
        uint64_t *type,
        const struct mliJson *json,
        const uint64_t token);
int mliFrame_boundary_layers_form_json_token(
        struct mliDynUint32 *boundary_layers,
        const uint32_t object_idx,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names,
        const struct mliJson *json,
        const uint64_t token);
int mliFrame_object_reference_form_json_token(
        uint32_t *object_reference,
        const struct mliJson *json,
        const uint64_t token,
        const struct mliDynMap *object_names);
#endif
