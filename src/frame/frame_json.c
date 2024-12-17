/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "frame_json.h"
#include "../chk/chk.h"
#include "../map/map_json.h"
#include "../vec/vec_json.h"
#include "../quaternion/quaternion_json.h"

int mliFrame_type_from_json_token(
        uint64_t *type,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t _t;
        int has_obj = mli_Json_token_by_key(json, token, "obj", &_t);
        int has_children = mli_Json_token_by_key(json, token, "children", &_t);

        if (has_obj && has_children) {
                chk_bad("Frame must not have both keys 'obj', and 'children'.");
        } else if (!has_obj && !has_children) {
                chk_bad("Frame must have either of keys 'obj', or 'children'.");
        } else if (has_obj && !has_children) {
                (*type) = MLI_OBJECT;
        } else if (!has_obj && has_children) {
                (*type) = MLI_FRAME;
        } else {
                chk_bad("Not expected to happen");
        }

        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliFrame_id_from_json_token(
        uint32_t *id,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_id;
        int64_t _id;
        chk_msg(mli_Json_token_by_key(json, token, "id", &token_id),
                "Expected Frame to have key 'id'.");
        chk_msg(mli_Json_int64_by_token(json, token_id + 1, &_id),
                "Failed to parse Frame's id.");
        chk_msg(_id >= 0, "Expected Frame's id >= 0.");
        (*id) = _id;

        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliFrame_pos_rot_from_json_token(
        struct mli_HomTraComp *frame2mother,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_pos, token_rot;
        /* pos */
        chk_msg(mli_Json_token_by_key(json, token, "pos", &token_pos),
                "Expected Frame to have key 'pos'.");
        chk_msg(mli_Vec_from_json_token(
                        &frame2mother->translation, json, token_pos + 1),
                "Failed to parse Frame's 'pos' from json.");

        /* rot */
        chk_msg(mli_Json_token_by_key(json, token, "rot", &token_rot),
                "Expected Frame to have key 'rot'.");
        chk_msg(mli_Quaternion_from_json(
                        &frame2mother->rotation, json, token_rot + 1),
                "Failed to parse Frame's 'rot' from json.");
        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliFrame_boundary_layers_form_json_token(
        struct mli_Uint32Vector *boundary_layers,
        const uint32_t object_idx,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_mtl_key, token_mtl;
        uint64_t material_idx;
        chk_msg(mli_Json_token_by_key(json, token, "mtl", &token_mtl_key),
                "Expected 'mtl' in Frame.");
        token_mtl = token_mtl_key + 1;
        chk_msg(json->tokens[token_mtl].type == JSMN_OBJECT,
                "Expected 'mtl' to be a json-object {}.");

        for (material_idx = 0u;
             material_idx < objects[object_idx].num_materials;
             material_idx++) {
                const char *material_key_in_object =
                        objects[object_idx].material_names[material_idx].array;

                uint64_t token_material_key = 0u;
                uint32_t boundary_layer_idx = 0u;
                chk_msg(mli_Json_token_by_key(
                                json,
                                token_mtl,
                                material_key_in_object,
                                &token_material_key),
                        "Expected object's material-key to be in "
                        "object-reference's mtls in tree.json.");

                chk_msg(mli_Map_get_value_for_string_from_json(
                                boundary_layer_names,
                                json,
                                token_material_key + 1,
                                &boundary_layer_idx),
                        "Expected boundary-layer to exist in materials.");

                chk_msg(mli_Uint32Vector_push_back(
                                boundary_layers, boundary_layer_idx),
                        "Failed to push-back boundary_layer_idx into "
                        "frame's boundary_layers.");
        }

        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliFrame_object_reference_form_json_token(
        uint32_t *object_reference,
        const struct mli_Json *json,
        const uint64_t token,
        const struct mli_Map *object_names)
{
        uint64_t token_obj_key;
        chk_msg(mli_Json_token_by_key(json, token, "obj", &token_obj_key),
                "Expected object to have key 'obj'.");
        chk_msg(mli_Map_get_value_for_string_from_json(
                        object_names,
                        json,
                        token_obj_key + 1,
                        object_reference),
                "Failed to get object-reference 'obj' from map");
        return 1;
chk_error:
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliFrame_from_json(
        struct mliFrame *mother,
        const struct mli_Json *json,
        const uint64_t token_children,
        const struct mli_Map *object_names,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names)
{
        uint64_t num_children;
        uint64_t c;
        chk_msg(json->tokens[token_children].type == JSMN_ARRAY,
                "Expected Frame's children to be a json-array '[]'.");
        num_children = json->tokens[token_children].size;
        for (c = 0; c < num_children; c++) {
                uint64_t token_child =
                        mli_Json_token_by_index(json, token_children, c);
                struct mliFrame *child = NULL;
                uint64_t type;
                uint64_t token_grandchildren;

                chk_msg(mliFrame_type_from_json_token(&type, json, token_child),
                        "Failed to read type of Frame.");

                child = mliFrame_add(mother, type);
                chk_msg(child, "Failed to add child to frame.");

                chk_msg(mliFrame_pos_rot_from_json_token(
                                &child->frame2mother, json, token_child),
                        "Failed to set pos, and rot of Frame from json.");

                chk_msg(mliFrame_id_from_json_token(
                                &child->id, json, token_child),
                        "Failed to set id of Frame from json.");

                switch (type) {
                case MLI_FRAME:
                        chk_msg(mli_Json_token_by_key(
                                        json,
                                        token_child,
                                        "children",
                                        &token_grandchildren),
                                "Expected child of type Frame to have "
                                "key 'children'.");
                        chk_msg(mliFrame_from_json(
                                        child,
                                        json,
                                        token_grandchildren + 1,
                                        object_names,
                                        objects,
                                        boundary_layer_names),
                                "Failed to populate grandchildren "
                                "Frames from json.");
                        break;
                case MLI_OBJECT:
                        chk_msg(mliFrame_object_reference_form_json_token(
                                        &child->object,
                                        json,
                                        token_child,
                                        object_names),
                                "Failed to parse object-reference "
                                "from json.");
                        chk_msg(mliFrame_boundary_layers_form_json_token(
                                        &child->boundary_layers,
                                        child->object,
                                        objects,
                                        boundary_layer_names,
                                        json,
                                        token_child),
                                "Failed to set boundary_layers of Frame "
                                "from json.");
                        break;
                default:
                        chk_bad("Unknown type of frame.");
                        break;
                }
        }
        return 1;
chk_error:
        return 0;
}
