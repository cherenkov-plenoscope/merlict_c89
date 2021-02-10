/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery_json.h"
#include "mli_debug.h"
#include "mliVec_json.h"
#include "mliColor_json.h"

int __mli_material_type_from_name_token(
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_material_type)
{
        char *name_str = NULL;
        uint64_t name_str_capacity =
                (json->tokens[token_name + 1].end -
                 json->tokens[token_name + 1].start + 1u);
        mli_check(
                json->tokens[token_name + 1].type == JSMN_STRING,
                "Expected 'name' to be of type string.");
        mli_malloc(name_str, char, name_str_capacity);
        mli_check(
                mliJson_as_string(
                        json, token_name + 1, name_str, name_str_capacity),
                "Failed to extract string from json.");
        mli_check(
                mli_material_type_from_string(name_str, out_material_type),
                "Failed to parse material type from json-string.");
        free(name_str);
        return 1;
error:
        free(name_str);
        return 0;
}

int _mliDynMap_key_from_json(
        struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        const uint64_t value)
{
        char *name_str = NULL;
        uint64_t name_str_capacity =
                (json->tokens[token_name + 1].end -
                 json->tokens[token_name + 1].start + 1u);
        mli_check(
                json->tokens[token_name + 1].type == JSMN_STRING,
                "Expected 'name' to be of type string.");
        mli_malloc(name_str, char, name_str_capacity);
        mli_check(
                mliJson_as_string(
                        json, token_name + 1, name_str, name_str_capacity),
                "Failed to extract string from json.");
        mli_check(
                mliDynMap_insert(map, name_str, value),
                "Failed to insert name and value into map.");
        free(name_str);
        return 1;
error:
        free(name_str);
        return 0;
}

int _mliDynMap_get_value_for_string_from_json(
        const struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_value)
{
        char *name_str = NULL;
        uint64_t value;
        uint64_t name_str_capacity =
                (json->tokens[token_name + 1].end -
                 json->tokens[token_name + 1].start + 1u);
        mli_check(
                json->tokens[token_name + 1].type == JSMN_STRING,
                "Expected token to be of type string to be given to mliMap.");
        mli_malloc(name_str, char, name_str_capacity);
        mli_check(
                mliJson_as_string(
                        json, token_name + 1, name_str, name_str_capacity),
                "Failed to extract string from json.");
        mli_check(
                mliDynMap_get(map, name_str, &value),
                "Failed to get value for json-string-key from map.");

        (*out_value) = (uint32_t)value;

        free(name_str);
        return 1;
error:
        free(name_str);
        return 0;
}

int __mliMaterials_assign_colors_from_json(
        struct mliMaterials *resources,
        struct mliDynMap *color_names,
        const struct mliJson *json)
{
        uint64_t token;
        uint64_t token_colors;
        uint64_t c;
        mli_check(
                mliJson_find_key(json, 0, "colors", &token),
                "Expected scenery-json to have key 'colors'.");
        token_colors = token + 1;
        mli_check(
                json->tokens[token_colors].type == JSMN_ARRAY,
                "Expected key 'colors' to point to a json-array.")
                mli_check(
                        resources->num_colors ==
                                (uint64_t)json->tokens[token_colors].size,
                        "Expected num_colors in SceneryResources to match "
                        "json-array.");
        for (c = 0; c < resources->num_colors; c++) {
                uint64_t token_c =
                        mliJson_array_child_token(json, token_colors, c);
                uint64_t token_c_name;
                uint64_t token_c_rgb;
                mli_check(
                        json->tokens[token_c].type == JSMN_OBJECT,
                        "Expected color-object with 'name' and 'rgb'.");
                mli_check(
                        mliJson_find_key(json, token_c, "name", &token_c_name),
                        "Expected function to have key 'name'.");
                mli_check(
                        mliJson_find_key(json, token_c, "rgb", &token_c_rgb),
                        "Expected function to have key 'rgb'.");
                mli_check(
                        mliColor_from_json_token(
                                &resources->colors[c], json, token_c_rgb + 1),
                        "Failed to assign color from json.");
                mli_check(
                        _mliDynMap_key_from_json(
                                color_names, json, token_c_name, c),
                        "Failed to read and insert color's name into map.");
        }
        return 1;
error:
        return 0;
}

int __mliMedium_from_json(
        struct mliMedium *medium,
        const struct mliDynMap *function_names,
        const struct mliJson *json,
        const uint64_t token_s)
{
        uint64_t token_refr, token_abso;
        mli_check(
                mliJson_find_key(json, token_s, "refraction", &token_refr),
                "Expected surface-item to contain key 'refraction'.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        function_names, json, token_refr, &medium->refraction),
                "Failed to get idx from map for string from json");

        mli_check(
                mliJson_find_key(json, token_s, "absorbtion", &token_abso),
                "Expected surface-item to contain key 'absorbtion'.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        function_names, json, token_abso, &medium->absorbtion),
                "Failed to get idx from map for string from json");

        return 1;
error:
        return 0;
}

int __mliMaterials_assign_media_from_json(
        struct mliMaterials *resources,
        struct mliDynMap *medium_names,
        const struct mliDynMap *function_names,
        const struct mliJson *json)
{
        uint64_t token;
        uint64_t token_surfaces;
        uint64_t m;
        mli_check(
                mliJson_find_key(json, 0, "media", &token),
                "Expected scenery-json to have key 'media'.");
        token_surfaces = token + 1;
        mli_check(
                json->tokens[token_surfaces].type == JSMN_ARRAY,
                "Expected key 'media' to point to a json-array.")
                mli_check(
                        resources->num_media ==
                                (uint64_t)json->tokens[token_surfaces].size,
                        "Expected num_media in struct mliGeometry to match "
                        "json-array.");
        for (m = 0; m < resources->num_media; m++) {
                uint64_t token_m =
                        mliJson_array_child_token(json, token_surfaces, m);
                uint64_t token_m_name;
                mli_check(
                        json->tokens[token_m].type == JSMN_OBJECT,
                        "Expected medium to be a json-object {}.");
                mli_check(
                        mliJson_find_key(json, token_m, "name", &token_m_name),
                        "Expected medium-object to have key 'name'.");
                mli_check(
                        __mliMedium_from_json(
                                &resources->media[m],
                                function_names,
                                json,
                                token_m),
                        "Failed to copy medium from json.");
                mli_check(
                        _mliDynMap_key_from_json(
                                medium_names, json, token_m_name, m),
                        "Failed to read and insert medium's name into map.");
        }
        return 1;
error:
        return 0;
}

int __mliSurface_from_json(
        struct mliSurface *surface,
        const struct mliDynMap *function_names,
        const struct mliDynMap *color_names,
        const struct mliJson *json,
        const uint64_t token_s)
{
        uint64_t token_mate, token_spec, token_diff, token_colo;

        mli_check(
                mliJson_find_key(json, token_s, "material", &token_mate),
                "Expected json-surface-item to contain key 'material'.");
        mli_check(
                json->tokens[token_mate].type == JSMN_STRING,
                "Expected medium's material to be of type string.");
        mli_check(
                __mli_material_type_from_name_token(
                        json, token_mate, &surface->material),
                "Failed to get material-idx from map for string from json");

        mli_check(
                mliJson_find_key(
                        json, token_s, "specular_reflection", &token_spec),
                "Expected json-surface-item to contain key "
                "'specular_reflection'.");
        mli_check(
                json->tokens[token_spec].type == JSMN_STRING,
                "Expected medium's specular_reflection to be of type string.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        function_names,
                        json,
                        token_spec,
                        &surface->specular_reflection),
                "Failed to get material-idx from map for string from json");

        mli_check(
                mliJson_find_key(
                        json, token_s, "diffuse_reflection", &token_diff),
                "Expected json-surface-item to contain key "
                "'diffuse_reflection'.");
        mli_check(
                json->tokens[token_diff].type == JSMN_STRING,
                "Expected medium's diffuse_reflection to be of type string.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        function_names,
                        json,
                        token_diff,
                        &surface->diffuse_reflection),
                "Failed to get function-idx from map for diffuse_reflection");

        mli_check(
                mliJson_find_key(json, token_s, "color", &token_colo),
                "Expected json-surface-item to contain key 'color'.");
        mli_check(
                json->tokens[token_colo].type == JSMN_STRING,
                "Expected medium's diffuse_reflection to be of type string.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        color_names, json, token_colo, &surface->color),
                "Failed to get function-idx from map for color");

        return 1;
error:
        return 0;
}

int __mliMaterials_assign_surfaces_from_json(
        struct mliMaterials *resources,
        struct mliDynMap *surface_names,
        const struct mliDynMap *function_names,
        const struct mliDynMap *color_names,
        const struct mliJson *json)
{
        uint64_t token;
        uint64_t token_surfaces;
        uint64_t s;
        mli_check(
                mliJson_find_key(json, 0, "surfaces", &token),
                "Expected scenery-json to have key 'surfaces'.");
        token_surfaces = token + 1;
        mli_check(
                json->tokens[token_surfaces].type == JSMN_ARRAY,
                "Expected key 'surfaces' to point to a json-array.")
                mli_check(
                        resources->num_surfaces ==
                                (uint64_t)json->tokens[token_surfaces].size,
                        "Expected num_surfaces in SceneryResources "
                        "json-array.size.");
        for (s = 0; s < resources->num_surfaces; s++) {
                uint64_t token_s =
                        mliJson_array_child_token(json, token_surfaces, s);
                uint64_t token_s_name;
                mli_check(
                        json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected surface to be of type object {}.");
                mli_check(
                        mliJson_find_key(json, token_s, "name", &token_s_name),
                        "Expected surface-object to have key 'name'.");
                mli_check(
                        _mliDynMap_key_from_json(
                                surface_names, json, token_s_name, s),
                        "Failed to read and insert surface's name into map.");
                mli_check(
                        __mliSurface_from_json(
                                &resources->surfaces[s],
                                function_names,
                                color_names,
                                json,
                                token_s),
                        "Failed to copy surface from json.");
        }
        return 1;
error:
        return 0;
}

int __mliMaterials_assign_boundary_layers_from_json(
        struct mliMaterials *materials,
        struct mliDynMap *boundary_layer_names,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json)
{
        uint64_t token;
        uint64_t token_layers;
        uint64_t s;
        mli_check(
                mliJson_find_key(json, 0, "boundary_layers", &token),
                "Expected scenery-json to have key 'boundary_layers'.");
        token_layers = token + 1;
        mli_check(
                json->tokens[token_layers].type == JSMN_ARRAY,
                "Expected key 'boundary_layers' to be a json-array.");
        mli_check(
                materials->num_boundary_layers ==
                        (uint32_t)json->tokens[token_layers].size,
                "Expected num_boundary_layers in materials to match "
                "json-array.size.");
        for (s = 0; s < materials->num_boundary_layers; s++) {
                uint64_t token_s =
                        mliJson_array_child_token(json, token_layers, s);
                uint64_t token_s_name;
                mli_check(
                        json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected boundary_layer to be of type object {}.");
                mli_check(
                        mliJson_find_key(json, token_s, "name", &token_s_name),
                        "Expected boundary_layers-object to have key 'name'.");
                mli_check(
                        _mliDynMap_key_from_json(
                                boundary_layer_names, json, token_s_name, s),
                        "Failed to insert boundary_layer's name into map.");
                mli_check(
                        __mliFrame_set_boundary_layer(
                                &materials->boundary_layers[s],
                                surface_names,
                                medium_names,
                                json,
                                token_s),
                        "Failed to copy boundary_layer from json.");
        }
        return 1;
error:
        return 0;
}

int __mliFrame_type_from_json(
        uint64_t *type,
        const struct mliJson *json,
        const uint64_t token_child)
{
        uint64_t token_type;
        char *type_string = NULL;
        uint64_t num_chars_for_type;
        mli_check(
                mliJson_find_key(json, token_child, "type", &token_type),
                "Expected Frame to have key 'type'.");
        num_chars_for_type =
                (json->tokens[token_type + 1].end -
                 json->tokens[token_type + 1].start + 1u);
        mli_malloc(type_string, char, num_chars_for_type);
        mliJson_as_string(
                json, token_type + 1, type_string, num_chars_for_type);
        mli_check(
                mli_string_to_type(type_string, type),
                "Expected Frame's type to be either 'frame' or 'object'.");
        free(type_string);
        return 1;
error:
        free(type_string);
        return 0;
}

int __mliFrame_set_id(
        uint32_t *id,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_id;
        int64_t _id;
        mli_check(
                mliJson_find_key(json, token, "id", &token_id),
                "Expected Frame to have key 'id'.");
        mli_check(
                mliJson_as_int64(json, token_id + 1, &_id),
                "Failed to parse Frame's id.");
        mli_check(_id >= 0, "Expected Frame's id >= 0.");
        (*id) = _id;

        return 1;
error:
        return 0;
}

int __mliFrame_set_pos_rot(
        struct mliFrame *frame,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_pos, token_rot;
        /* pos */
        mli_check(
                mliJson_find_key(json, token, "pos", &token_pos),
                "Expected Frame to have key 'pos'.");
        mli_check(
                mliVec_from_json_token(
                        &frame->frame2mother.translation, json, token_pos + 1),
                "Failed to parse Frame's 'pos' from json.");

        /* rot */
        mli_check(
                mliJson_find_key(json, token, "rot", &token_rot),
                "Expected Frame to have key 'rot'.");
        mli_check(
                __mliQuaternion_from_json(
                        &frame->frame2mother.rotation, json, token_rot + 1),
                "Failed to parse Frame's 'rot' from json.");
        return 1;
error:
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

        mli_check(
                mliJson_find_key(json, side_token + 1, "medium", &token_medium),
                "Expected key 'medium' in side.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        medium_names, json, token_medium, &side->medium),
                "Failed to get medium-idx from map");

        mli_check(
                mliJson_find_key(
                        json, side_token + 1, "surface", &token_surface),
                "Expected key 'surface' in side.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        surface_names, json, token_surface, &side->surface),
                "Failed to get surface-idx from map");

        return 1;
error:
        return 0;
}

int __mliFrame_set_boundary_layer(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t token_surface)
{
        uint64_t token_inner_side, token_outer_side;
        mli_check(
                mliJson_find_key(
                        json, token_surface, "inner", &token_inner_side),
                "Expected key 'inner' in surface.");
        mli_check(
                mliJson_find_key(
                        json, token_surface, "outer", &token_outer_side),
                "Expected key 'outer' in surface.");

        mli_check(
                __mliSide_set(
                        &boundary_layer->inner,
                        surface_names,
                        medium_names,
                        json,
                        token_inner_side),
                "Failed to parse inner side.");
        mli_check(
                __mliSide_set(
                        &boundary_layer->outer,
                        surface_names,
                        medium_names,
                        json,
                        token_outer_side),
                "Failed to parse outer side.");
        return 1;
error:
        return 0;
}

int __mliFrame_set_surface_idx(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_surface, token_surface_key;
        mli_check(
                mliJson_find_key(json, token, "layer", &token_surface_key),
                "Expected primitive to have key 'layer'.");

        token_surface = token_surface_key + 1;
        mli_check(
                __mliFrame_set_boundary_layer(
                        boundary_layer,
                        surface_names,
                        medium_names,
                        json,
                        token_surface),
                "Failed to set inner, and outer boundary-layer.");
        return 1;
error:
        return 0;
}

int __mliFrame_set_object_reference(
        uint32_t *object_reference,
        const struct mliJson *json,
        const uint64_t token,
        const struct mliDynMap *object_names)
{
        uint64_t token_obj_key;
        mli_check(
                mliJson_find_key(json, token, "obj", &token_obj_key),
                "Expected object to have key 'obj'.");
        mli_check(
                _mliDynMap_get_value_for_string_from_json(
                        object_names, json, token_obj_key, object_reference),
                "Failed to get object-reference from map");
        return 1;
error:
        return 0;
}

int __mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliDynMap *object_names,
        const struct mliDynMap *surface_names,
        const struct mliDynMap *medium_names)
{
        uint64_t num_children;
        uint64_t c;
        mli_check(
                json->tokens[token_children].type == JSMN_ARRAY,
                "Expected Frame's children to be a json-array '[]'.");
        num_children = json->tokens[token_children].size;
        for (c = 0; c < num_children; c++) {
                uint64_t token_child =
                        mliJson_array_child_token(json, token_children, c);
                struct mliFrame *child = NULL;
                uint64_t type;
                uint64_t token_grandchildren;

                mli_check(
                        __mliFrame_type_from_json(&type, json, token_child),
                        "Failed to read type of Frame.");

                child = mliFrame_add(mother, type);
                mli_check(child, "Failed to add child to frame.");

                mli_check(
                        __mliFrame_set_pos_rot(child, json, token_child),
                        "Failed to set pos, and rot of Frame from json.");

                mli_check(
                        __mliFrame_set_id(&child->id, json, token_child),
                        "Failed to set id of Frame from json.");

                switch (type) {
                case MLI_FRAME:
                        mli_check(
                                mliJson_find_key(
                                        json,
                                        token_child,
                                        "children",
                                        &token_grandchildren),
                                "Expected child of type Frame to have "
                                "key 'children'.");
                        mli_check(
                                __mliFrame_from_json(
                                        child,
                                        json,
                                        token_grandchildren + 1,
                                        object_names,
                                        surface_names,
                                        medium_names),
                                "Failed to populate grandchildren "
                                "Frames from json.");
                        break;
                case MLI_OBJECT:
                        mli_check(
                                __mliFrame_set_surface_idx(
                                        &child->boundary_layer,
                                        surface_names,
                                        medium_names,
                                        json,
                                        token_child),
                                "Failed to parse objects's surface "
                                "from json.");
                        mli_check(
                                __mliFrame_set_object_reference(
                                        &child->object,
                                        json,
                                        token_child,
                                        object_names),
                                "Failed to parse object-reference "
                                "from json.");
                        break;
                default:
                        mli_sentinel("Unknown type of frame.");
                        break;
                }
        }
        return 1;
error:
        return 0;
}
