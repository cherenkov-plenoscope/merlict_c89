/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_JSON_H_
#define MERLICT_MLISCENERY_JSON_H_

#include <stdint.h>
#include <stdlib.h>
#include "mli_debug.h"
#include "mli_json.h"
#include "mliUserScenery.h"
#include "mliFunc_malloc_from_json_token.h"
#include "mliVec_from_json_token.h"


int __mliScenery_surface_capacity_from_json(
    mliScenery *surface_resources,
    const mliJson *json) {
    uint64_t token;
    mli_check(
        mliJson_find_key(json, 0, "functions", &token),
        "Expected scenery-json to have key 'functions'.");
    mli_check(
        json->tokens[token + 1].type == JSMN_ARRAY,
        "Expected key 'functions' to point to a json-array.")
    surface_resources->num_functions = json->tokens[token + 1].size;

    mli_check(
        mliJson_find_key(json, 0, "colors", &token),
        "Expected scenery-json to have key 'colors'.");
    mli_check(
        json->tokens[token + 1].type == JSMN_ARRAY,
        "Expected key 'colors' to point to a json-array.")
    surface_resources->num_colors = json->tokens[token + 1].size;

    mli_check(
        mliJson_find_key(json, 0, "surfaces", &token),
        "Expected scenery-json to have key 'surfaces'.");
    mli_check(
        json->tokens[token + 1].type == JSMN_ARRAY,
        "Expected key 'colors' to point to a json-array.")
    surface_resources->num_surfaces = json->tokens[token + 1].size;
    return 1;
error:
    return 0;}

int __mliScenery_malloc_functions_from_json(
    mliScenery *surface_resources,
    const mliJson *json) {
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
        surface_resources->num_functions ==
        (uint64_t)json->tokens[token_functions].size,
        "Expected num_functions in mliScenery to match json-array.");

    for (f = 0; f < surface_resources->num_functions; f++) {
        uint64_t token_f = mliJson_array_child_token(json, token_functions, f);
        mli_check(
            mliFunc_malloc_from_json_token(
                &surface_resources->functions[f],
                json,
                token_f),
            "Could not malloc mliFunc in mliScenery from json.");
    }
    return 1;
error:
    return 0;}

int __mliScenery_assign_colors_from_json(
    mliScenery *surface_resources,
    const mliJson *json) {
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
        surface_resources->num_colors ==
        (uint64_t)json->tokens[token_colors].size,
        "Expected num_colors in mliScenery to match json-array.");
    for (c = 0; c < surface_resources->num_colors; c++) {
        uint64_t token_c = mliJson_array_child_token(json, token_colors, c);
        mli_check(
            mliColor_from_json_token(
                &surface_resources->colors[c],
                json,
                token_c),
            "Could not assign color from json to mliScenery.");
    }
    return 1;
error:
    return 0;}

int __mliSurface_from_json(
    mliSurface* surface,
    const mliJson *json,
    const uint64_t token_s) {
    int64_t idx_tmp;
    uint64_t token_tmp;

    mli_check(mliJson_find_key(json, token_s, "material", &token_tmp),
        "Expected json-surface-item to contain key 'material'.");
    mli_check(mliJson_as_int64(json, token_tmp + 1, &idx_tmp),
        "Could not parse material from json.");
    mli_check(idx_tmp >= 0, "Expected material-index to be positive.");
    surface->material = idx_tmp;

    mli_check(mliJson_find_key(json, token_s, "medium_refraction", &token_tmp),
        "Expected json-surface-item to contain key 'medium_refraction'.");
    mli_check(mliJson_as_int64(json, token_tmp + 1, &idx_tmp),
        "Could not parse medium_refraction from json.");
    mli_check(idx_tmp >= 0, "Expected medium_refraction-index to be positive.");
    surface->medium_refraction = idx_tmp;

    mli_check(mliJson_find_key(json, token_s, "medium_absorbtion", &token_tmp),
        "Expected json-surface-item to contain key 'medium_absorbtion'.");
    mli_check(mliJson_as_int64(json, token_tmp + 1, &idx_tmp),
        "Could not parse medium_absorbtion from json.");
    mli_check(idx_tmp >= 0, "Expected medium_absorbtion-index to be positive.");
    surface->medium_absorbtion = idx_tmp;

    mli_check(mliJson_find_key(json, token_s, "boundary_layer_specular_reflection", &token_tmp),
        "Expected json-surface-item to contain key 'boundary_layer_specular_reflection'.");
    mli_check(mliJson_as_int64(json, token_tmp + 1, &idx_tmp),
        "Could not parse boundary_layer_specular_reflection from json.");
    mli_check(idx_tmp >= 0, "Expected boundary_layer_specular_reflection-index to be positive.");
    surface->boundary_layer_specular_reflection = idx_tmp;

    mli_check(mliJson_find_key(json, token_s, "boundary_layer_diffuse_reflection", &token_tmp),
        "Expected json-surface-item to contain key 'boundary_layer_diffuse_reflection'.");
    mli_check(mliJson_as_int64(json, token_tmp + 1, &idx_tmp),
        "Could not parse boundary_layer_diffuse_reflection from json.");
    mli_check(idx_tmp >= 0, "Expected boundary_layer_diffuse_reflection-index to be positive.");
    surface->boundary_layer_diffuse_reflection = idx_tmp;

    mli_check(mliJson_find_key(json, token_s, "color", &token_tmp),
        "Expected json-surface-item to contain key 'color'.");
    mli_check(mliJson_as_int64(json, token_tmp + 1, &idx_tmp),
        "Could not parse color from json.");
    mli_check(idx_tmp >= 0, "Expected color-index to be positive.");
    surface->color = idx_tmp;

    return 1;
error:
    return 0;}

int __mliScenery_assign_surfaces_from_json(
    mliScenery *surface_resources,
    const mliJson *json) {
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
        surface_resources->num_surfaces ==
        (uint64_t)json->tokens[token_surfaces].size,
        "Expected num_surfaces in mliScenery to match json-array.");
    for (s = 0; s < surface_resources->num_surfaces; s++) {
        uint64_t token_s = mliJson_array_child_token(json, token_surfaces, s);
        mli_check(
            __mliSurface_from_json(
                &surface_resources->surfaces[s],
                json,
                token_s),
            "Could not copy surface from json.");
    }
    return 1;
error:
    return 0;}


int __mliFrame_type_from_json(
    uint64_t* type,
    const mliJson *json,
    const uint64_t token_child) {
    uint64_t token_type;
    char* type_string = NULL;
    uint64_t num_chars_for_type;
    mli_check(
        mliJson_find_key(json, token_child, "type", &token_type),
        "Expected json-frame to have key 'type'.");
    num_chars_for_type = (
        json->tokens[token_type + 1].end -
        json->tokens[token_type + 1].start +
        1u);
    mli_malloc(type_string, char, num_chars_for_type);
    mliJson_as_string(json, token_type + 1, type_string, num_chars_for_type);
    mli_check(
        mli_string_to_type(type_string, type),
        "Expected frame-type to be known.");
    free(type_string);
    return 1;
error:
    return 0;}

int __mliQuaternion_tait_bryan_from_json(
    struct mliQuaternion* quat,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_xyz;
    struct mliVec xyz;
    mli_check(
        mliJson_find_key(json, token, "xyz", &token_xyz),
        "Expected tait_bryan to have key 'xyz'.");
    mli_check(
        mliVec_from_json_token(&xyz, json, token_xyz + 1),
        "Failed to parse tait_bryan's 'xyz' from json.");
    *quat = mliQuaternion_set_tait_bryan(xyz.x, xyz.y, xyz.z);
    return 1;
error:
    return 0;}

int __mliQuaternion_axis_angle_from_json(
    struct mliQuaternion* quat,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_axis, token_angle;
    double angle;
    struct mliVec axis;
    mli_check(
        mliJson_find_key(json, token, "axis", &token_axis),
        "Expected axis_angle to have key 'axis'.");
    mli_check(
        mliVec_from_json_token(&axis, json, token_axis + 1),
        "Failed to parse axis_angle's 'axis' from json.");
    mli_check(
        mliJson_find_key(json, token, "angle", &token_angle),
        "Expected axis_angle to have key 'axis'.");
    mli_check(
        mliJson_as_float64(json, token_angle + 1, &angle),
        "Failed to parse axis_angle's 'angle' from json.");
    *quat = mliQuaternion_set_rotaxis_and_angle(axis, angle);
    return 1;
error:
    return 0;}

int __mliQuaternion_quaternion_from_json(
    struct mliQuaternion* quat,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_xyz;
    double w;
    struct mliVec q;
    mli_check(
        mliJson_find_key(json, token, "xyz", &token_xyz),
        "Expected quaternion to have key 'xyz'.");
    mli_check(
        mliVec_from_json_token(&q, json, token_xyz + 1),
        "Failed to parse quaternion's 'xyz' from json.");
    /*
        Recover 4th element: q.w.
        Expect unit-quaternion:
        1.0 != q.w**2 + q.x**2 + q.y**2 + q.z**2
        thus:
        q.w**2 = 1.0 - q.x**2 - q.y**2 - q.z**2
        q.w = sqrt(1.0 - q.x**2 - q.y**2 - q.z**2)
    */
    w = sqrt(1. - q.x*q.x - q.y*q.y - q.z*q.z);
    *quat = mliQuaternion_set(w, q.x, q.y, q.z);
    mli_check(
        fabs(mliQuaternion_norm(*quat) - 1.) < 1e-6,
        "Expected norm(quaternion) < 1e-6. Expected unit-quaternion.");
    return 1;
error:
    return 0;}

int __mliQuaternion_from_json(
    struct mliQuaternion* quat,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_repr = 0u;
    char* repr_str = NULL;
    uint64_t num_chars = 0u;
    mli_check(
        mliJson_find_key(json, token, "repr", &token_repr),
        "Expected 'rot' to have key 'repr'.");
    num_chars = (
        json->tokens[token_repr + 1].end -
        json->tokens[token_repr + 1].start +
        1u);
    mli_malloc(repr_str, char, num_chars);
    mliJson_as_string(json, token_repr + 1, repr_str, num_chars);
    if (strcmp(repr_str, "tait_bryan") == 0) {
        mli_check(
            __mliQuaternion_tait_bryan_from_json(quat, json, token),
            "Failed to parse tait_bryan rotation.");
    } else if (strcmp(repr_str, "axis_angle") == 0) {
        mli_check(__mliQuaternion_axis_angle_from_json(quat, json, token),
            "Failed to parse axis_angle rotation.");
    } else if (strcmp(repr_str, "quaternion") == 0) {
        mli_check(__mliQuaternion_quaternion_from_json(quat, json, token),
            "Failed to parse quaternion rotation.");
    } else {
        mli_sentinel("Unknown representation ('repr') in rotation.");
    }
    free(repr_str);
    return 1;
error:
    return 0;}

int __mliFrame_set_id_pos_rot(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_id, token_pos, token_rot;
    int64_t id;
    /* id */
    mli_check(
        mliJson_find_key(json, token, "id", &token_id),
        "Expected json-frame to have key 'id'.");
    mli_check(
        mliJson_as_int64(json, token_id + 1, &id),
        "Failed parsing frame-id.");
    mli_check(id >= 0, "Expected frame-id >= 0.");
    frame->id = id;

    /* pos */
    mli_check(
        mliJson_find_key(json, token, "pos", &token_pos),
        "Expected json-frame to have key 'pos'.");
    mli_check(
        mliVec_from_json_token(&frame->frame2mother.trans, json, token_pos + 1),
        "Failed to parse frame's 'pos' from json.");

    /* rot */
    mli_check(
        mliJson_find_key(json, token, "rot", &token_rot),
        "Expected json-frame to have key 'rot'.");
    mli_check(
        __mliQuaternion_from_json(
            &frame->frame2mother.rot,
            json,
            token_rot + 1),
        "Failed to parse frame's 'rot' from json.");
    return 1;
error:
    return 0;}

int __mliFrame_set_surface_idx(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_surface, token_inner, token_outer;
    int64_t inner_surface_idx, outer_surface_idx;
    mli_check(
        mliJson_find_key(json, token, "surface", &token_surface),
        "Expected primitive (except for Frame) to have key 'surface'.");

    mli_check(
        mliJson_find_key(json, token_surface + 1, "inner", &token_inner),
        "Expected key 'inner' in surface.");
    mli_check(
        mliJson_find_key(json, token_surface + 1, "outer", &token_outer),
        "Expected key 'outer' in surface.");

    mli_check(
        mliJson_as_int64(json, token_inner + 1, &inner_surface_idx),
        "Failed to parse mliFrame's 'surface->inner' integer from json.");
    mli_check(inner_surface_idx >= 0, "Expected inner_surface_idx >= 0.");
    mli_check(
        mliJson_as_int64(json, token_outer + 1, &outer_surface_idx),
        "Failed to parse mliFrame's 'surface->outer' integer from json.");
    mli_check(outer_surface_idx >= 0, "Expected outer_surface_idx >= 0.");
    frame->surfaces.inner = inner_surface_idx;
    frame->surfaces.outer = outer_surface_idx;
    return 1;
error:
    return 0;}

int __mliFrame_set_Sphere(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_radius;
    mli_check(
        mliJson_find_key(json, token, "radius", &token_radius),
        "Expected json-Sphere to have key 'radius'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_radius + 1,
            &frame->primitive.sphere->radius),
        "Failed parsing Sphere's radius from json.");
    return 1;
error:
    return 0;}


int __mliFrame_set_Cylinder(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_radius;
    uint64_t token_length;
    /* radius */
    mli_check(
        mliJson_find_key(json, token, "radius", &token_radius),
        "Expected json-Cylinder to have key 'radius'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_radius + 1,
            &frame->primitive.cylinder->radius),
        "Failed parsing Cylinder's radius from json.");

    /* length */
    mli_check(
        mliJson_find_key(json, token, "length", &token_length),
        "Expected json-Cylinder to have key 'length'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_length + 1,
            &frame->primitive.cylinder->length),
        "Failed parsing Cylinder's length from json.");
    return 1;
error:
    return 0;}


int __mliFrame_set_SphericalCapHex(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_curvature_radius;
    uint64_t token_inner_hex_radius;
    /* curvature_radius */
    mli_check(
        mliJson_find_key(
            json,
            token,
            "curvature_radius",
            &token_curvature_radius),
        "Expected json-SphericalCapHex to have key 'curvature_radius'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_curvature_radius + 1,
            &frame->primitive.spherical_cap_hex->curvature_radius),
        "Failed parsing SphericalCapHex's radius from json.");

    /* inner_hex_radius */
    mli_check(
        mliJson_find_key(
            json,
            token,
            "inner_hex_radius",
            &token_inner_hex_radius),
        "Expected json-SphericalCapHex to have key 'inner_hex_radius'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_inner_hex_radius + 1,
            &frame->primitive.spherical_cap_hex->inner_hex_radius),
        "Failed parsing SphericalCapHex's inner_hex_radius from json.");
    return 1;
error:
    return 0;}


int __mliFrame_set_Hexagon(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_inner_radius;
    mli_check(
        mliJson_find_key(json, token, "inner_radius", &token_inner_radius),
        "Expected json-Hexagon to have key 'inner_radius'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_inner_radius + 1,
            &frame->primitive.hexagon->inner_radius),
        "Failed parsing Hexagon's inner_radius from json.");
    return 1;
error:
    return 0;}


int __mliFrame_set_BiCirclePlane(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_x_height;
    uint64_t token_y_width;
    /* x_height */
    mli_check(
        mliJson_find_key(
            json,
            token,
            "x_height",
            &token_x_height),
        "Expected json-BiCirclePlane to have key 'x_height'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_x_height + 1,
            &frame->primitive.bicircleplane->x_height),
        "Failed parsing BiCirclePlane's radius from json.");

    /* y_width */
    mli_check(
        mliJson_find_key(
            json,
            token,
            "y_width",
            &token_y_width),
        "Expected json-BiCirclePlane to have key 'y_width'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_y_width + 1,
            &frame->primitive.bicircleplane->y_width),
        "Failed parsing BiCirclePlane's y_width from json.");
    return 1;
error:
    return 0;}


int __mliFrame_set_Disc(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_radius;
    mli_check(
        mliJson_find_key(json, token, "radius", &token_radius),
        "Expected json-Disc to have key 'radius'.");
    mli_check(
        mliJson_as_float64(
            json,
            token_radius + 1,
            &frame->primitive.disc->radius),
        "Failed parsing Disc's radius from json.");
    return 1;
error:
    return 0;}


int mliFace_from_json_token(
    mliFace* f,
    const mliJson *json,
    const uint64_t token) {
    int64_t tmp;
    mli_check(
        json->tokens[token].type == JSMN_ARRAY,
        "Expected face-token to be a json-array.");
    mli_check(
        json->tokens[token].size == 3,
        "Expected face-token to contain exactly 3 tokens [a,b,c].");
    mli_check(
        mliJson_as_int64(json, token + 1, &tmp),
        "Failed parsing mliFace's 1st-value.");
    mli_check(tmp >= 0, "Expected mliMesh index 'a' >= 0.");
    f->a = tmp;
    mli_check(
        mliJson_as_int64(json, token + 2, &tmp),
        "Failed parsing mliFace's 2nd-value.");
    mli_check(tmp >= 0, "Expected mliMesh index 'b' >= 0.");
    f->b = tmp;
    mli_check(
        mliJson_as_int64(json, token + 3, &tmp),
        "Failed parsing mliFace's 3rd-value.");
    mli_check(tmp >= 0, "Expected mliMesh index 'c' >= 0.");
    f->c = tmp;
    return 1;
error:
    return 0;}


int __mliFrame_set_Mesh(
    mliFrame* frame,
    const mliJson *json,
    const uint64_t token) {
    uint64_t token_vertices;
    uint64_t token_faces;
    uint64_t v, f;
    /* vertices */
    mli_check(
        mliJson_find_key(json, token, "vertices", &token_vertices),
        "Expected json-Mesh to have key 'vertices'.");
    frame->primitive.mesh->num_vertices = json->tokens[token_vertices + 1].size;
    /* faces */
    mli_check(
        mliJson_find_key(json, token, "faces", &token_faces),
        "Expected json-Mesh to have key 'faces'.");
    frame->primitive.mesh->num_faces = json->tokens[token_faces + 1].size;
    /* malloc */
    mli_check(
        mliMesh_malloc(frame->primitive.mesh),
        "Failed to allocate mesh from json.");
    for (v = 0; v < frame->primitive.mesh->num_vertices; v++) {
        uint64_t token_vertex = mliJson_array_child_token(
            json,
            token_vertices + 1,
            v);
        mli_check(
            mliVec_from_json_token(
                &frame->primitive.mesh->vertices[v],
                json,
                token_vertex),
            "Failed to parse Mesh's vertex from json.");
    }
    for (f = 0; f < frame->primitive.mesh->num_faces; f++) {
        uint64_t token_face = mliJson_array_child_token(
            json,
            token_faces + 1,
            f);
        mli_check(
            mliFace_from_json_token(
                &frame->primitive.mesh->faces[f],
                json,
                token_face),
            "Failed to parse Mesh's face from json.");
    }
    return 1;
error:
    return 0;}


int __mliFrame_from_json(
    mliFrame* mother,
    const mliJson *json,
    const uint64_t token_children) {
    uint64_t num_children;
    uint64_t c;
    mli_check(
        json->tokens[token_children].type == JSMN_ARRAY,
        "Expected frame-children to be a json-array '[]'.");
    num_children = json->tokens[token_children].size;
    for (c = 0; c < num_children; c++) {
        uint64_t token_child = mliJson_array_child_token(
            json,
            token_children,
            c);
        mliFrame* child = NULL;
        uint64_t type;
        uint64_t token_grandchildren;
        mli_check(
            __mliFrame_type_from_json(&type, json, token_child),
            "Failed reading type of frame.");
        child = mliFrame_add(mother, type);
        mli_check(
            __mliFrame_set_id_pos_rot(child, json, token_child),
            "Failed to set id, pos, and rot of frame from json.");
        if (type != MLI_FRAME) {
            mli_check(
                __mliFrame_set_surface_idx(child, json, token_child),
                "Failed parsing mliFrame's surface index from json.");}
        switch(type) {
            case MLI_FRAME:
                mli_check(
                    mliJson_find_key(
                        json,
                        token_child,
                        "children",
                        &token_grandchildren),
                    "Expected child of type Frame to have key 'children'.");
                mli_check(
                    __mliFrame_from_json(child, json, token_grandchildren + 1),
                    "Could not populate grandchildren mliFrames from json.");
                break;
            case MLI_MESH:
                mli_check(
                    __mliFrame_set_Mesh(child, json, token_child),
                    "Failed setting Mesh from json.");
                break;
            case MLI_SPHERICAL_CAP_HEX:
                mli_check(
                    __mliFrame_set_SphericalCapHex(child, json, token_child),
                    "Failed setting SphericalCapHex from json.");
                break;
            case MLI_SPHERE:
                mli_check(
                    __mliFrame_set_Sphere(child, json, token_child),
                    "Failed setting Sphere from json.");
                break;
            case MLI_CYLINDER:
                mli_check(
                    __mliFrame_set_Cylinder(child, json, token_child),
                    "Failed setting Cylinder from json.");
                break;
            case MLI_HEXAGON:
                mli_check(
                    __mliFrame_set_Hexagon(child, json, token_child),
                    "Failed setting Hexagon from json.");
                break;
            case MLI_BICIRCLEPLANE:
                mli_check(
                    __mliFrame_set_BiCirclePlane(child, json, token_child),
                    "Failed setting BiCirclePlane from json.");
                break;
            case MLI_DISC:
                mli_check(
                    __mliFrame_set_Disc(child, json, token_child),
                    "Failed setting Disc from json.");
                break;
            default: mli_sentinel("Unknown type of frame."); break;
        }
    }
    return 1;
error:
    return 0;}


int mliUserScenery_malloc_from_json(mliUserScenery *uscn, const mliJson *json) {
    uint64_t token;
    mli_check(
        __mliScenery_surface_capacity_from_json(&uscn->surface_resources, json),
        "Could not estimate capacity for surface_resources.");
    mli_check(
        mliUserScenery_malloc(uscn),
        "Could not allocate memory for mliUserScenery.");
    mli_check(
        __mliScenery_malloc_functions_from_json(&uscn->surface_resources, json),
        "Could not copy functions from json.");
    mli_check(
        __mliScenery_assign_colors_from_json(&uscn->surface_resources, json),
        "Could not copy colors from json.");
    mli_check(
        __mliScenery_assign_surfaces_from_json(&uscn->surface_resources, json),
        "Could not copy surfaces from json.");

    mli_check(
        mliJson_find_key(json, 0, "children", &token),
        "Expected scenery-json to have key 'children'.");
    mli_check(
        __mliFrame_from_json(&uscn->root, json, token + 1),
        "Could not populate tree of mliFrames from json.");

    return 1;
error:
    return 0;}

#endif
