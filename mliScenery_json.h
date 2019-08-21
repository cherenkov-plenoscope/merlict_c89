/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERY_JSON_H_
#define MERLICT_MLISCENERY_JSON_H_

#include <stdint.h>
#include <stdlib.h>
#include "mli_debug.h"
#include "mli_json.h"
#include "mliUserScenery.h"


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

int mliUserScenery_malloc_from_json(mliUserScenery *uscn, const mliJson *json) {
    uint64_t token;
    mli_check(
        __mliScenery_surface_capacity_from_json(&uscn->surface_resources, json),
        "Could not estimate capacity for surface_resources.");
    mli_check(
        mliUserScenery_malloc(uscn),
        "Could not allocate memory for mliUserScenery.");



    mli_check(
        mliJson_find_key(json, 0, "children", &token),
        "Expected scenery-json to have key 'children'.");
    return 1;
error:
    return 0;}

int mliScenery_bumb_capacity_child_json(
    mliScenery *s,
    const mliJson *json,
    const uint64_t child_token) {
    uint64_t type_token;
    char *buff;
    mli_malloc(
        buff,
        char,
        json->tokens[child_token].end -
        json->tokens[child_token].start);
    mli_check(
        mliJson_find_key(json, child_token, "type", &type_token),
        "Expected frame to have key 'type'.");
    mliJson_string(json, type_token + 1, buff);
    /* fprintf(stderr, "type:'%s'\n", buff); */

    if (strcmp(buff, "Disc") == 0) {
        s->num_discs += 1;
    } else if (strcmp(buff, "Sphere") == 0) {
        s->num_spheres += 1;
    } else if (strcmp(buff, "Cylinder") == 0) {
        s->num_cylinders += 1;
    }

    free(buff);
    return 1;
error:
    free(buff);
    return 0;
}


int mliScenery_capacity_walk_children_json(
    mliScenery *s,
    const mliJson *json,
    const uint64_t token) {
    int64_t child_idx;
    mli_check(
        json->tokens[token].type == JSMN_ARRAY,
        "Expected key 'children' to point to an array.")
    for (child_idx = 0; child_idx < json->tokens[token].size; child_idx++) {
        uint64_t child_token = mliJson_array_child_token(
            json,
            token,
            child_idx);
        uint64_t subchild_token;

        mli_check(
            mliScenery_bumb_capacity_child_json(s, json, child_token),
            "Can not reserve capacity for child.")

        mli_check(
            mliJson_find_key(json, child_token, "children", &subchild_token),
            "Expected frame to have key 'children'.");
        mliScenery_capacity_walk_children_json(s, json, subchild_token + 1);
    }
    return 1;
error:
    return 0;
}


int mliScenery_capacity_from_json(mliScenery *s, const mliJson *json) {
    uint64_t token;
    mli_check(
        mliJson_find_key(json, 0, "functions", &token),
        "Expected scenery-json to have key 'functions'.");
    mli_check(
        json->tokens[token + 1].type == JSMN_ARRAY,
        "Expected key 'functions' to point to an array.")
    s->num_functions = json->tokens[token + 1].size;

    mli_check(
        mliJson_find_key(json, 0, "colors", &token),
        "Expected scenery-json to have key 'colors'.");
    mli_check(
        json->tokens[token + 1].type == JSMN_ARRAY,
        "Expected key 'colors' to point to an array.")
    s->num_colors = json->tokens[token + 1].size;

    mli_check(
        mliJson_find_key(json, 0, "surfaces", &token),
        "Expected scenery-json to have key 'surfaces'.");
    mli_check(
        json->tokens[token + 1].type == JSMN_ARRAY,
        "Expected key 'colors' to point to an array.")
    s->num_surfaces = json->tokens[token + 1].size;

    mli_check(
        mliJson_find_key(json, 0, "children", &token),
        "Expected scenery-json to have key 'children'.");
    mli_check(
        mliScenery_capacity_walk_children_json(s, json, token + 1),
        "Can not walk children in json-scenery.");
    return 1;
error:
    return 0;
}

#endif
