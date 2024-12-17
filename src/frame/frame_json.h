/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FRAME_JSON_H_
#define MLI_FRAME_JSON_H_

#include <stdint.h>
#include "frame.h"
#include "../json/json.h"
#include "../object/object.h"
#include "../map/map.h"

int mli_Frame_from_json(
        struct mli_Frame *mother,
        const struct mli_Json *json,
        const uint64_t token_children,
        const struct mli_Map *object_names,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names);
int mli_Frame_id_from_json_token(
        uint32_t *id,
        const struct mli_Json *json,
        const uint64_t token);
int mli_Frame_pos_rot_from_json_token(
        struct mli_HomTraComp *frame2mother,
        const struct mli_Json *json,
        const uint64_t token);
int mli_Frame_type_from_json_token(
        uint64_t *type,
        const struct mli_Json *json,
        const uint64_t token);
int mli_Frame_boundary_layers_form_json_token(
        struct mli_Uint32Vector *boundary_layers,
        const uint32_t object_idx,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names,
        const struct mli_Json *json,
        const uint64_t token);
int mli_Frame_object_reference_form_json_token(
        uint32_t *object_reference,
        const struct mli_Json *json,
        const uint64_t token,
        const struct mli_Map *object_names);
#endif
