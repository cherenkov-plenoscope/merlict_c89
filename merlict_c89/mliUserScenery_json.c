/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery_json.h"
#include "mliDynMap_json.h"
#include "mliVec_json.h"
#include "mliColor_json.h"


int __mliMaterials_assign_boundary_layers_from_json(
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
                uint64_t token_s_name =
                        mliJson_array_child_token(json, token, s);
                uint64_t token_s = token_s_name + 1;

                chk_msg(json->tokens[token_s_name].type == JSMN_STRING,
                        "Expected boundary_layer to be a String.");

                chk_msg(mliDynMap_insert_key_from_json(
                                boundary_layer_names, json, token_s_name, s),
                        "Failed to insert boundary_layer's name into map.");

                chk_msg(json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected boundary_layer to be of type object {}.");

                chk_msg(__mliBoundaryLayer_from_json(
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

int __mliFrame_type_from_json(
        uint64_t *type,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t _t;
        int has_obj = mliJson_find_key(json, token, "obj", &_t);
        int has_children = mliJson_find_key(json, token, "children", &_t);

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
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int __mliFrame_set_id(
        uint32_t *id,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_id;
        int64_t _id;
        chk_msg(mliJson_find_key(json, token, "id", &token_id),
                "Expected Frame to have key 'id'.");
        chk_msg(mliJson_as_int64(json, token_id + 1, &_id),
                "Failed to parse Frame's id.");
        chk_msg(_id >= 0, "Expected Frame's id >= 0.");
        (*id) = _id;

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int __mliFrame_set_pos_rot(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_pos, token_rot;
        /* pos */
        chk_msg(mliJson_find_key(json, token, "pos", &token_pos),
                "Expected Frame to have key 'pos'.");
        chk_msg(mliVec_from_json_token(
                        &frame->frame2mother.translation, json, token_pos + 1),
                "Failed to parse Frame's 'pos' from json.");

        /* rot */
        chk_msg(mliJson_find_key(json, token, "rot", &token_rot),
                "Expected Frame to have key 'rot'.");
        chk_msg(__mliQuaternion_from_json(
                        &frame->frame2mother.rotation, json, token_rot + 1),
                "Failed to parse Frame's 'rot' from json.");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int __mliSide_set(
        struct mliSide *side,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t side_token)
{
        uint64_t token_medium, token_surface;

        chk_msg(mliJson_find_key(json, side_token + 1, "medium", &token_medium),
                "Expected key 'medium' in side.");
        chk_msg(mliDynMap_get_value_for_string_from_json(
                        medium_names, json, token_medium + 1, &side->medium),
                "Failed to get medium-idx from map");

        chk_msg(mliJson_find_key(
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

int __mliBoundaryLayer_from_json(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t token_surface)
{
        uint64_t token_inner_side, token_outer_side;
        chk_msg(mliJson_find_key(
                        json, token_surface, "inner", &token_inner_side),
                "Expected key 'inner' in surface.");
        chk_msg(mliJson_find_key(
                        json, token_surface, "outer", &token_outer_side),
                "Expected key 'outer' in surface.");

        chk_msg(__mliSide_set(
                        &boundary_layer->inner,
                        surface_names,
                        medium_names,
                        json,
                        token_inner_side),
                "Failed to parse inner side.");
        chk_msg(__mliSide_set(
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

int __mliFrame_set_boundary_layers(
        struct mliDynUint32 *boundary_layers,
        const uint32_t object_idx,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_mtl_key, token_mtl;
        uint64_t material_idx;
        chk_msg(mliJson_find_key(json, token, "mtl", &token_mtl_key),
                "Expected 'mtl' in Frame.");
        token_mtl = token_mtl_key + 1;
        chk_msg(json->tokens[token_mtl].type == JSMN_OBJECT,
                "Expected 'mtl' to be a json-object {}.");

        for (material_idx = 0u;
             material_idx < objects[object_idx].num_materials;
             material_idx++) {
                const char *material_key_in_object =
                        objects[object_idx].material_names[material_idx].c_str;

                uint64_t token_material_key = 0u;
                uint32_t boundary_layer_idx = 0u;
                chk_msg(mliJson_find_key(
                                json,
                                token_mtl,
                                material_key_in_object,
                                &token_material_key),
                        "Expected object's material-key to be in "
                        "object-reference's mtls in tree.json.");

                chk_msg(mliDynMap_get_value_for_string_from_json(
                                boundary_layer_names,
                                json,
                                token_material_key + 1,
                                &boundary_layer_idx),
                        "Expected boundary-layer to exist in materials.");

                chk_msg(mliDynUint32_push_back(
                                boundary_layers, boundary_layer_idx),
                        "Failed to push-back boundary_layer_idx into "
                        "frame's boundary_layers.");
        }

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int __mliFrame_set_object_reference(
        uint32_t *object_reference,
        const struct mliJson *json,
        const uint64_t token,
        const struct mliDynMap *object_names)
{
        uint64_t token_obj_key;
        chk_msg(mliJson_find_key(json, token, "obj", &token_obj_key),
                "Expected object to have key 'obj'.");
        chk_msg(mliDynMap_get_value_for_string_from_json(
                        object_names,
                        json,
                        token_obj_key + 1,
                        object_reference),
                "Failed to get object-reference 'obj' from map");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int __mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliDynMap *object_names,
        const struct mliObject *objects,
        const struct mliDynMap *boundary_layer_names)
{
        uint64_t num_children;
        uint64_t c;
        chk_msg(json->tokens[token_children].type == JSMN_ARRAY,
                "Expected Frame's children to be a json-array '[]'.");
        num_children = json->tokens[token_children].size;
        for (c = 0; c < num_children; c++) {
                uint64_t token_child =
                        mliJson_array_child_token(json, token_children, c);
                struct mliFrame *child = NULL;
                uint64_t type;
                uint64_t token_grandchildren;

                chk_msg(__mliFrame_type_from_json(&type, json, token_child),
                        "Failed to read type of Frame.");

                child = mliFrame_add(mother, type);
                chk_msg(child, "Failed to add child to frame.");

                chk_msg(__mliFrame_set_pos_rot(child, json, token_child),
                        "Failed to set pos, and rot of Frame from json.");

                chk_msg(__mliFrame_set_id(&child->id, json, token_child),
                        "Failed to set id of Frame from json.");

                switch (type) {
                case MLI_FRAME:
                        chk_msg(mliJson_find_key(
                                        json,
                                        token_child,
                                        "children",
                                        &token_grandchildren),
                                "Expected child of type Frame to have "
                                "key 'children'.");
                        chk_msg(__mliFrame_from_json(
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
                        chk_msg(__mliFrame_set_object_reference(
                                        &child->object,
                                        json,
                                        token_child,
                                        object_names),
                                "Failed to parse object-reference "
                                "from json.");
                        chk_msg(__mliFrame_set_boundary_layers(
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
error:
        return 0;
}
