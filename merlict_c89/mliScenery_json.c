/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_json.h"

int __mliScenery_surface_capacity_from_json(
        struct mliSceneryResourcesCapacity *res_cap,
        const struct mliJson *json)
{
        uint64_t token;
        mli_check(
                mliJson_find_key(json, 0, "functions", &token),
                "Expected scenery-json to have key 'functions'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'functions' to point to a json-array.")
                res_cap->num_functions = json->tokens[token + 1].size;

        mli_check(
                mliJson_find_key(json, 0, "colors", &token),
                "Expected scenery-json to have key 'colors'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'colors' to point to a json-array.")
                res_cap->num_colors = json->tokens[token + 1].size;

        mli_check(
                mliJson_find_key(json, 0, "media", &token),
                "Expected scenery-json to have key 'media'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'media' to point to a json-array.")
                res_cap->num_media = json->tokens[token + 1].size;

        mli_check(
                mliJson_find_key(json, 0, "surfaces", &token),
                "Expected scenery-json to have key 'surfaces'.");
        mli_check(
                json->tokens[token + 1].type == JSMN_ARRAY,
                "Expected key 'surfaces' to point to a json-array.")
                res_cap->num_surfaces = json->tokens[token + 1].size;
        return 1;
error:
        return 0;
}

int _mliMap2_key_from_json(
        struct mliMap2 *map,
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
                mliMap2_insert(map, name_str, value),
                "Failed to insert name and value into map.") free(name_str);
        return 1;
error:
        free(name_str);
        return 0;
}

int _mliMap2_get_value_for_string_from_json(
        const struct mliMap2 *map,
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_value)
{
        char *name_str = NULL;
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
                mliMap2_get_value(map, name_str, out_value),
                "Failed to get value for json-string-key from map.");
        free(name_str);
        return 1;
error:
        free(name_str);
        return 0;
}

int __mliScenery_malloc_functions_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *function_names,
        const struct mliJson *json)
{
        uint64_t token;
        uint64_t token_functions;
        uint64_t f;
        mli_check(
                mliJson_find_key(json, 0, "functions", &token),
                "Expected scenery-json to have key 'functions'.");
        token_functions = token + 1;
        mli_check(
                json->tokens[token_functions].type == JSMN_ARRAY,
                "Expected key 'functions' to point to a json-array.")
                mli_check(
                        resources->num_functions ==
                                (uint64_t)json->tokens[token_functions].size,
                        "Expected num_functions in mliScenery to "
                        "match json-array.");

        for (f = 0; f < resources->num_functions; f++) {
                uint64_t token_f =
                        mliJson_array_child_token(json, token_functions, f);
                uint64_t token_f_name;
                uint64_t token_f_val;
                mli_check(
                        json->tokens[token_f].type == JSMN_OBJECT,
                        "Expected function-object with 'name' and 'values'.");
                mli_check(
                        mliJson_find_key(json, token_f, "name", &token_f_name),
                        "Expected function to have key 'name'.");
                mli_check(
                        mliJson_find_key(json, token_f, "values", &token_f_val),
                        "Expected function to have key 'values'.");
                mli_check(
                        mliFunc_malloc_from_json_token(
                                &resources->functions[f],
                                json,
                                token_f_val + 1),
                        "Failed to malloc mliFunc in mliScenery from json.");
                mli_check(
                        _mliMap2_key_from_json(
                                function_names, json, token_f_name, f),
                        "Failed to read and insert function's name into map.");
        }
        return 1;
error:
        return 0;
}

int __mliScenery_assign_colors_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *color_names,
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
                        _mliMap2_key_from_json(
                                color_names, json, token_c_name, c),
                        "Failed to read and insert color's name into map.");
        }
        return 1;
error:
        return 0;
}

int __mliMedium_from_json(
        struct mliMedium *medium,
        const struct mliMap2 *function_names,
        const struct mliJson *json,
        const uint64_t token_s)
{
        uint64_t token_refr, token_abso;
        mli_check(
                mliJson_find_key(json, token_s, "refraction", &token_refr),
                "Expected surface-item to contain key 'refraction'.");
        mli_check(
                _mliMap2_get_value_for_string_from_json(
                        function_names, json, token_refr, &medium->refraction),
                "Failed to get idx from map for string from json");

        mli_check(
                mliJson_find_key(json, token_s, "absorbtion", &token_abso),
                "Expected surface-item to contain key 'absorbtion'.");
        mli_check(
                _mliMap2_get_value_for_string_from_json(
                        function_names, json, token_abso, &medium->absorbtion),
                "Failed to get idx from map for string from json");

        return 1;
error:
        return 0;
}

int __mliScenery_assign_media_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *medium_names,
        const struct mliMap2 *function_names,
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
                        "Expected num_media in struct mliScenery to match "
                        "json-array.");
        for (m = 0; m < resources->num_media; m++) {
                uint64_t token_m =
                        mliJson_array_child_token(json, token_surfaces, m);
                uint64_t token_m_name;
                mli_check(
                        json->tokens[token_m].type == JSMN_OBJECT,
                        "Expected medium to be of type object {}.");
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
                        _mliMap2_key_from_json(
                                medium_names, json, token_m_name, m),
                        "Failed to read and insert medium's name into map.");
        }
        return 1;
error:
        return 0;
}

int __mliSurface_from_json(
        struct mliSurface *surface,
        const struct mliMap2 *function_names,
        const struct mliMap2 *color_names,
        const struct mliMap2 *material_names,
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
                _mliMap2_get_value_for_string_from_json(
                        material_names, json, token_mate, &surface->material),
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
                _mliMap2_get_value_for_string_from_json(
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
                _mliMap2_get_value_for_string_from_json(
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
                _mliMap2_get_value_for_string_from_json(
                        color_names, json, token_colo, &surface->color),
                "Failed to get function-idx from map for color");

        return 1;
error:
        return 0;
}

int __mliScenery_assign_surfaces_from_json(
        struct mliSceneryResources *resources,
        struct mliMap2 *surface_names,
        const struct mliMap2 *function_names,
        const struct mliMap2 *color_names,
        const struct mliMap2 *material_names,
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
                        _mliMap2_key_from_json(
                                surface_names, json, token_s_name, s),
                        "Failed to read and insert surface's name into map.");
                mli_check(
                        __mliSurface_from_json(
                                &resources->surfaces[s],
                                function_names,
                                color_names,
                                material_names,
                                json,
                                token_s),
                        "Failed to copy surface from json.");
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
                "Expected Frame's type to be known.");
        free(type_string);
        return 1;
error:
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
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names,
        const struct mliJson *json,
        const uint64_t side_token)
{
        uint64_t token_medium, token_surface;

        mli_check(
                mliJson_find_key(json, side_token + 1, "medium", &token_medium),
                "Expected key 'medium' in side.");
        mli_check(
                _mliMap2_get_value_for_string_from_json(
                        medium_names, json, token_medium, &side->medium),
                "Failed to get medium-idx from map");

        mli_check(
                mliJson_find_key(
                        json, side_token + 1, "surface", &token_surface),
                "Expected key 'surface' in side.");
        mli_check(
                _mliMap2_get_value_for_string_from_json(
                        surface_names, json, token_surface, &side->surface),
                "Failed to get surface-idx from map");

        return 1;
error:
        return 0;
}

int __mliFrame_set_boundary_layer(
        struct mliBoundaryLayer *boundary_layer,
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names,
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
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_surface, token_surface_key;
        mli_check(
                mliJson_find_key(json, token, "surface", &token_surface_key),
                "Expected primitive to have key 'surface'.");

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

int mliFace_from_json_token(
        struct mliFace *f,
        const struct mliJson *json,
        const uint64_t token)
{
        int64_t tmp;
        mli_check(
                json->tokens[token].type == JSMN_ARRAY,
                "Expected face-token to be a json-array.");
        mli_check(
                json->tokens[token].size == 3,
                "Expected face-token to contain exactly 3 tokens [a,b,c].");
        mli_check(
                mliJson_as_int64(json, token + 1, &tmp),
                "Failed to parse mliFace's 1st-value.");
        mli_check(tmp >= 0, "Expected struct mliMesh index 'a' >= 0.");
        f->a = tmp;
        mli_check(
                mliJson_as_int64(json, token + 2, &tmp),
                "Failed to  parse mliFace's 2nd-value.");
        mli_check(tmp >= 0, "Expected struct mliMesh index 'b' >= 0.");
        f->b = tmp;
        mli_check(
                mliJson_as_int64(json, token + 3, &tmp),
                "Failed to parse mliFace's 3rd-value.");
        mli_check(tmp >= 0, "Expected struct mliMesh index 'c' >= 0.");
        f->c = tmp;
        return 1;
error:
        return 0;
}

int __mliFrame_set_Mesh(
        struct mliFrame *frame,
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_vertices, token_surfaces;
        uint64_t v, f, s;
        uint32_t num_vertices, num_surfaces, num_faces_in_all_surfaces,
                num_faces_in_surface, face_idx;
        /* vertices */
        mli_check(
                mliJson_find_key(json, token, "vertices", &token_vertices),
                "Expected Mesh to have key 'vertices'.");
        num_vertices = json->tokens[token_vertices + 1].size;

        /* surfaces */
        mli_check(
                mliJson_find_key(json, token, "surfaces", &token_surfaces),
                "Expected Mesh to have key 'surfaces'.");
        num_surfaces = json->tokens[token_surfaces + 1].size;

        /* find total num faces */
        num_faces_in_all_surfaces = 0;
        for (s = 0; s < num_surfaces; s++) {
                uint64_t token_faces;
                uint64_t token_surface =
                        mliJson_array_child_token(json, token_surfaces + 1, s);
                mli_check(
                        mliJson_find_key(
                                json, token_surface, "faces", &token_faces),
                        "Expected Mesh's surface to have key 'faces'.");
                num_faces_in_all_surfaces += json->tokens[token_faces + 1].size;
        }

        /* malloc */
        mli_check(
                mliMesh_malloc(
                        frame->primitive.mesh,
                        num_vertices,
                        num_faces_in_all_surfaces),
                "Failed to allocate Mesh from json.");

        /* set vertices */
        for (v = 0; v < frame->primitive.mesh->num_vertices; v++) {
                uint64_t token_vertex =
                        mliJson_array_child_token(json, token_vertices + 1, v);
                mli_check(
                        mliVec_from_json_token(
                                &frame->primitive.mesh->vertices[v],
                                json,
                                token_vertex),
                        "Failed to read Mesh's vertices from json.");
        }

        /* set surfaces */
        face_idx = 0;
        for (s = 0; s < num_surfaces; s++) {
                uint64_t token_surface =
                        mliJson_array_child_token(json, token_surfaces + 1, s);
                uint64_t token_faces;
                struct mliBoundaryLayer boundary_layer_of_surface;
                uint32_t user_id_of_surface = 0;

                /* find num faces of this surface */
                mli_check(
                        mliJson_find_key(
                                json, token_surface, "faces", &token_faces),
                        "Expected Mesh-surface to have key 'faces'.");
                num_faces_in_surface = json->tokens[token_faces + 1].size;

                /* find boundary-layer of this surface */
                mli_check(
                        __mliFrame_set_boundary_layer(
                                &boundary_layer_of_surface,
                                surface_names,
                                medium_names,
                                json,
                                token_surface),
                        "Failed to set surface-properties of Mesh's surface.");

                /* find user_id of this surface */
                mli_check(
                        __mliFrame_set_id(
                                &user_id_of_surface, json, token_surface),
                        "Failed to set id of surface in Mesh.");

                /* set faces in this surface */
                for (f = 0; f < num_faces_in_surface; f++) {
                        uint64_t token_face = mliJson_array_child_token(
                                json, token_faces + 1, f);
                        struct mliFace face;
                        mli_check(
                                mliFace_from_json_token(
                                        &face, json, token_face),
                                "Failed to parse Mesh's face from json.");
                        frame->primitive.mesh->faces[face_idx] = face;
                        frame->primitive.mesh->boundary_layers[face_idx] =
                                boundary_layer_of_surface;
                        frame->primitive.mesh->user_ids[face_idx] =
                                user_id_of_surface;
                        face_idx += 1;
                }
        }

        mli_check(
                face_idx == num_faces_in_all_surfaces,
                "Expected to set all faces in Mesh");
        mli_check(
                mliMesh_assert_valid_faces(frame->primitive.mesh),
                "Expected Mesh faces to have valid vertex indices.");

        return 1;
error:
        return 0;
}

int __mliFrame_from_json(
        struct mliFrame *mother,
        const struct mliJson *json,
        const uint64_t token_children,
        const struct mliMap2 *surface_names,
        const struct mliMap2 *medium_names)
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
                mli_check(
                        __mliFrame_set_pos_rot(child, json, token_child),
                        "Failed to set pos, and rot of Frame from json.");
                if (type != MLI_MESH) {
                        mli_check(
                                __mliFrame_set_id(
                                        &child->id, json, token_child),
                                "Failed to set id of Frame from json.");
                }
                if (type != MLI_FRAME && type != MLI_MESH) {
                        mli_check(
                                __mliFrame_set_surface_idx(
                                        &child->boundary_layer,
                                        surface_names,
                                        medium_names,
                                        json,
                                        token_child),
                                "Failed to parse Frame's surface "
                                "from json.");
                }
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
                                        surface_names,
                                        medium_names),
                                "Failed to populate grandchildren "
                                "Frames from json.");
                        break;
                case MLI_MESH:
                        mli_check(
                                __mliFrame_set_Mesh(
                                        child,
                                        surface_names,
                                        medium_names,
                                        json,
                                        token_child),
                                "Failed to set Mesh from json.");
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

int mliUserScenery_malloc_from_json(
        struct mliUserScenery *uscn,
        const struct mliJson *json)
{
        struct mliMap2 material_names = mliMap2_init();
        uint64_t token;
        struct mliSceneryResourcesCapacity resources_capacity =
                mliSceneryResourcesCapacity_init();

        mli_check(
                __mliScenery_surface_capacity_from_json(
                        &resources_capacity, json),
                "Failed to estimate capacity for surface_resources.");

        mli_check(
                mliUserScenery_malloc(uscn),
                "Failed to allocate mliUserScenery.");

        mli_check(
                mliSceneryResources_malloc(
                        &uscn->resources, resources_capacity),
                "Failed to allocate mliSceneryResources.");

        mli_c(mliMap2_malloc(
                &uscn->function_names, resources_capacity.num_functions));
        mli_c(mliMap2_malloc(
                &uscn->color_names, resources_capacity.num_colors));
        mli_c(mliMap2_malloc(
                &uscn->medium_names, resources_capacity.num_media));
        mli_c(mliMap2_malloc(
                &uscn->surface_names, resources_capacity.num_surfaces));

        mli_c(mliMap2_malloc(&material_names, 2));
        mli_c(mliMap2_insert(&material_names, "Phong", MLI_MATERIAL_PHONG));
        mli_c(mliMap2_insert(
                &material_names, "Transparent", MLI_MATERIAL_TRANSPARENT));
        mli_check(
                __mliScenery_malloc_functions_from_json(
                        &uscn->resources, &uscn->function_names, json),
                "Failed to copy functions from json.");
        mli_check(
                __mliScenery_assign_colors_from_json(
                        &uscn->resources, &uscn->color_names, json),
                "Failed to copy colors from json.");
        mli_check(
                __mliScenery_assign_media_from_json(
                        &uscn->resources,
                        &uscn->medium_names,
                        &uscn->function_names,
                        json),
                "Failed to copy media from json.");
        mli_check(
                __mliScenery_assign_surfaces_from_json(
                        &uscn->resources,
                        &uscn->surface_names,
                        &uscn->function_names,
                        &uscn->color_names,
                        &material_names,
                        json),
                "Failed to copy surfaces from json.");

        mli_check(
                mliJson_find_key(json, 0, "default_medium", &token),
                "Expected scenery-json to have key 'default_medium'.");
        mli_check(
                _mliMap2_get_value_for_string_from_json(
                        &uscn->medium_names,
                        json,
                        token,
                        &uscn->default_medium),
                "Failed to assign the 'default_medium'.");

        mli_check(
                mliJson_find_key(json, 0, "children", &token),
                "Expected scenery-json to have key 'children'.");
        mli_check(
                __mliFrame_from_json(
                        &uscn->root,
                        json,
                        token + 1,
                        &uscn->surface_names,
                        &uscn->medium_names),
                "Failed to populate tree of Frames from json.");

        mliMap2_free(&material_names);
        return 1;
error:
        return 0;
}
