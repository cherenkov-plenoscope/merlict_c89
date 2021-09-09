/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliUserScenery_json.h"
#include "mli_debug.h"
#include "mliVec_json.h"
#include "mliColor_json.h"

int __mli_material_type_from_name_token(
        const struct mliJson *json,
        const uint64_t token,
        uint32_t *out_material_type)
{
        char buff[MLI_NAME_CAPACITY] = {'\0'};
        const uint64_t name_strlen =
                (json->tokens[token + 1].end - json->tokens[token + 1].start);
        chk_msg(name_strlen < sizeof(buff), "Value of 'name' is too long");
        chk_msg(json->tokens[token + 1].type == JSMN_STRING,
                "Expected 'name' to be of type string.");
        chk_msg(mliJson_as_string(json, token + 1, buff, name_strlen + 1),
                "Failed to extract string from json.");
        chk_msg(mli_material_type_from_string(buff, out_material_type),
                "Failed to parse material type from json-string.");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token + 1);
        return 0;
}

int _mliDynMap_insert_key_from_json(
        struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token,
        const uint64_t value)
{
        char buff[MLI_NAME_CAPACITY] = {'\0'};
        const uint64_t name_strlen =
                (json->tokens[token + 1].end - json->tokens[token + 1].start);
        chk_msg(name_strlen < sizeof(buff), "Key is too long");
        chk_msg(json->tokens[token + 1].type == JSMN_STRING,
                "Expected key to be of type string.");
        chk_msg(mliJson_as_string(json, token + 1, buff, name_strlen + 1),
                "Failed to extract string from json.");
        chk_msg(mliDynMap_insert(map, buff, value),
                "Failed to insert name and value into map.");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int _mliDynMap_get_value_for_string_from_json(
        const struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token,
        uint32_t *out_value)
{
        char buff[MLI_NAME_CAPACITY] = {'\0'};
        uint64_t value;
        uint64_t name_strlen =
                (json->tokens[token + 1].end - json->tokens[token + 1].start);
        chk_msg(name_strlen < sizeof(buff), "Key is too long");
        chk_msg(json->tokens[token + 1].type == JSMN_STRING,
                "Expected token to be of type string to be given to mliMap.");
        chk_msg(mliJson_as_string(json, token + 1, buff, name_strlen + 1),
                "Failed to extract string from json.");
        chk_msg(mliDynMap_get(map, buff, &value),
                "Failed to get value for json-string-key from map.");
        (*out_value) = (uint32_t)value;

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int __mliMaterialsCapacity_from_materials_json(
        struct mliMaterialsCapacity *rescap,
        const struct mliJson *json)
{
        uint64_t token = 0u;
        chk_msg(mliJson_find_key(json, 0, "colors", &token),
                "Expected materials-json to have key 'colors'.");
        chk_msg(json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'colors' to point to a json-array.");
        rescap->num_colors = json->tokens[token + 1].size;
        chk_msg(mliJson_find_key(json, 0, "media", &token),
                "Expected materials-json to have key 'media'.");
        chk_msg(json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'media' to point to a json-array.");
        rescap->num_media = json->tokens[token + 1].size;
        chk_msg(mliJson_find_key(json, 0, "surfaces", &token),
                "Expected materials-json to have key 'surfaces'.");
        chk_msg(json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'surfaces' to point to a json-array.");
        rescap->num_surfaces = json->tokens[token + 1].size;
        chk_msg(mliJson_find_key(json, 0, "boundary_layers", &token),
                "Expected materials-json to have key 'boundary_layers'.");
        chk_msg(json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'boundary_layers' to be a json-array.");
        rescap->num_boundary_layers = json->tokens[token + 1].size;
        return 1;
error:
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
        chk_msg(mliJson_find_key(json, 0, "colors", &token),
                "Expected scenery-json to have key 'colors'.");
        token_colors = token + 1;
        chk_msg(json->tokens[token_colors].type == JSMN_ARRAY,
                "Expected key 'colors' to point to a json-array.")
                chk_msg(resources->num_colors ==
                                (uint64_t)json->tokens[token_colors].size,
                        "Expected num_colors in SceneryResources to match "
                        "json-array.");
        for (c = 0; c < resources->num_colors; c++) {
                uint64_t token_c =
                        mliJson_array_child_token(json, token_colors, c);
                uint64_t token_c_name;
                uint64_t token_c_rgb;
                chk_msg(json->tokens[token_c].type == JSMN_OBJECT,
                        "Expected color-object with 'name' and 'rgb'.");
                chk_msg(mliJson_find_key(json, token_c, "name", &token_c_name),
                        "Expected function to have key 'name'.");
                chk_msg(mliJson_find_key(json, token_c, "rgb", &token_c_rgb),
                        "Expected function to have key 'rgb'.");
                chk_msg(mliColor_from_json_token(
                                &resources->colors[c], json, token_c_rgb + 1),
                        "Failed to assign color from json.");
                chk_msg(_mliDynMap_insert_key_from_json(
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
        const uint64_t token)
{
        uint64_t token_refr, token_abso;
        chk_msg(mliJson_find_key(json, token, "refraction", &token_refr),
                "Expected surface-item to contain key 'refraction'.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        function_names, json, token_refr, &medium->refraction),
                "Failed to get idx from map for string from json");

        chk_msg(mliJson_find_key(json, token, "absorbtion", &token_abso),
                "Expected surface-item to contain key 'absorbtion'.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        function_names, json, token_abso, &medium->absorbtion),
                "Failed to get idx from map for string from json");

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
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
        chk_msg(mliJson_find_key(json, 0, "media", &token),
                "Expected scenery-json to have key 'media'.");
        token_surfaces = token + 1;
        chk_msg(json->tokens[token_surfaces].type == JSMN_ARRAY,
                "Expected key 'media' to point to a json-array.")
                chk_msg(resources->num_media ==
                                (uint64_t)json->tokens[token_surfaces].size,
                        "Expected num_media in struct mliGeometry to match "
                        "json-array.");
        for (m = 0; m < resources->num_media; m++) {
                uint64_t token_m =
                        mliJson_array_child_token(json, token_surfaces, m);
                uint64_t token_m_name;
                chk_msg(json->tokens[token_m].type == JSMN_OBJECT,
                        "Expected medium to be a json-object {}.");
                chk_msg(mliJson_find_key(json, token_m, "name", &token_m_name),
                        "Expected medium-object to have key 'name'.");
                chk_msg(__mliMedium_from_json(
                                &resources->media[m],
                                function_names,
                                json,
                                token_m),
                        "Failed to copy medium from json.");
                chk_msg(_mliDynMap_insert_key_from_json(
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

        chk_msg(mliJson_find_key(json, token_s, "material", &token_mate),
                "Expected json-surface-item to contain key 'material'.");
        chk_msg(json->tokens[token_mate].type == JSMN_STRING,
                "Expected medium's material to be of type string.");
        chk_msg(__mli_material_type_from_name_token(
                        json, token_mate, &surface->material),
                "Failed to get material-idx from map for string from json");

        chk_msg(mliJson_find_key(
                        json, token_s, "specular_reflection", &token_spec),
                "Expected json-surface-item to contain key "
                "'specular_reflection'.");
        chk_msg(json->tokens[token_spec].type == JSMN_STRING,
                "Expected medium's specular_reflection to be of type string.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        function_names,
                        json,
                        token_spec,
                        &surface->specular_reflection),
                "Failed to get material-idx from map for string from json");

        chk_msg(mliJson_find_key(
                        json, token_s, "diffuse_reflection", &token_diff),
                "Expected json-surface-item to contain key "
                "'diffuse_reflection'.");
        chk_msg(json->tokens[token_diff].type == JSMN_STRING,
                "Expected medium's diffuse_reflection to be of type string.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        function_names,
                        json,
                        token_diff,
                        &surface->diffuse_reflection),
                "Failed to get function-idx from map for diffuse_reflection");

        chk_msg(mliJson_find_key(json, token_s, "color", &token_colo),
                "Expected json-surface-item to contain key 'color'.");
        chk_msg(json->tokens[token_colo].type == JSMN_STRING,
                "Expected medium's diffuse_reflection to be of type string.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        color_names, json, token_colo, &surface->color),
                "Failed to get function-idx from map for color");

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token_s);
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
        chk_msg(mliJson_find_key(json, 0, "surfaces", &token),
                "Expected scenery-json to have key 'surfaces'.");
        token_surfaces = token + 1;
        chk_msg(json->tokens[token_surfaces].type == JSMN_ARRAY,
                "Expected key 'surfaces' to be a json-array.")
                chk_msg(resources->num_surfaces ==
                                (uint64_t)json->tokens[token_surfaces].size,
                        "Expected num_surfaces in materials to match "
                        "json-array.size.");
        for (s = 0; s < resources->num_surfaces; s++) {
                uint64_t token_s =
                        mliJson_array_child_token(json, token_surfaces, s);
                uint64_t token_s_name;
                chk_msg(json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected surface to be of type object {}.");
                chk_msg(mliJson_find_key(json, token_s, "name", &token_s_name),
                        "Expected surface-object to have key 'name'.");
                chk_msg(_mliDynMap_insert_key_from_json(
                                surface_names, json, token_s_name, s),
                        "Failed to insert surface's name into map.");
                chk_msg(__mliSurface_from_json(
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
        chk_msg(mliJson_find_key(json, 0, "boundary_layers", &token),
                "Expected scenery-json to have key 'boundary_layers'.");
        token_layers = token + 1;
        chk_msg(json->tokens[token_layers].type == JSMN_ARRAY,
                "Expected key 'boundary_layers' to be a json-array.");
        chk_msg(materials->num_boundary_layers ==
                        (uint32_t)json->tokens[token_layers].size,
                "Expected num_boundary_layers in materials to match "
                "json-array.size.");
        for (s = 0; s < materials->num_boundary_layers; s++) {
                uint64_t token_s =
                        mliJson_array_child_token(json, token_layers, s);
                uint64_t token_s_name;
                chk_msg(json->tokens[token_s].type == JSMN_OBJECT,
                        "Expected boundary_layer to be of type object {}.");
                chk_msg(mliJson_find_key(json, token_s, "name", &token_s_name),
                        "Expected boundary_layers-object to have key 'name'.");
                chk_msg(_mliDynMap_insert_key_from_json(
                                boundary_layer_names, json, token_s_name, s),
                        "Failed to insert boundary_layer's name into map.");
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
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        medium_names, json, token_medium, &side->medium),
                "Failed to get medium-idx from map");

        chk_msg(mliJson_find_key(
                        json, side_token + 1, "surface", &token_surface),
                "Expected key 'surface' in side.");
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        surface_names, json, token_surface, &side->surface),
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

                chk_msg(_mliDynMap_get_value_for_string_from_json(
                                boundary_layer_names,
                                json,
                                token_material_key,
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
        chk_msg(_mliDynMap_get_value_for_string_from_json(
                        object_names, json, token_obj_key, object_reference),
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
