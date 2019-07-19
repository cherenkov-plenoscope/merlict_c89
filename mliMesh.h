/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIMESH_H_
#define MERLICT_MLIMESH_H_

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliFace.h"

typedef struct {
    uint32_t num_vertices;
    mliVec *vertices;
    uint32_t num_faces;
    mliFace *faces;
} mliMesh;

mliMesh mliMesh_init() {
    mliMesh m;
    m.num_vertices = 0;
    m.vertices = NULL;
    m.num_faces = 0;
    m.faces = NULL;
    return m;}

int mliMesh_malloc(mliMesh* m) {
    mli_malloc(m->vertices, mliVec, m->num_vertices);
    mli_malloc(m->faces, mliFace, m->num_faces);
    return 1;
error:
    return 0;}

void mliMesh_free(mliMesh *m) {
    free(m->vertices);
    m->num_vertices = 0;
    free(m->faces);
    m->num_faces = 0;}

int mli_parse_three_ints(const char *line, int *a, int* b, int*c) {
    int state = 0;
    int old_state = state;
    int statemachine[][2] = {
    /*  ws  print */
        {0,   1},  /* 0 */
        {2,   1},  /* 1 first */
        {2,   3},  /* 2 */
        {4,   3},  /* 3 second */
        {4,   5},  /* 4 */
        {6,   5},  /* 5 third  */
    };
    int idx = 0;
    char aaa[64];
    int aidx = 0;
    char z;
    while (1) {
        if (state == 6)
            break;
         z = line[idx];
        if (z == '\0')
            state = 6;
        else if (isspace(z))
            state = statemachine[state][0];
        else
            state = statemachine[state][1];
        /* printf("state: %d, char: '%c'\n", state, z); */
        idx++;

        if (state == 1) {
            aaa[aidx] = z;
            aidx++;
        }
        if (state == 2 && old_state == 1) {
            aaa[aidx] = '\0';
            aidx = 0;
            *a = atoi(aaa);
        }

        if (state == 3) {
            aaa[aidx] = z;
            aidx++;
        }
        if (state == 4 && old_state == 3) {
            aaa[aidx] = '\0';
            aidx = 0;
            *b = atoi(aaa);
        }

        if (state == 5) {
            aaa[aidx] = z;
            aidx++;
        }
        if (state == 6 && old_state == 5) {
            aaa[aidx] = '\0';
            aidx = 0;
            *c = atoi(aaa);
        }
        old_state = state;
    }
    return state;
}


int mli_parse_three_doubles(const char *line, double *a, double* b, double*c) {
    int state = 0;
    int old_state = state;
    int statemachine[][2] = {
    /*  ws  print  */
        {0,   1},  /* 0        */
        {2,   1},  /* 1 first  */
        {2,   3},  /* 2        */
        {4,   3},  /* 3 second */
        {4,   5},  /* 4        */
        {6,   5},  /* 5 third  */
    };
    int idx = 0;
    char aaa[64];
    int aidx = 0;
    char z;
    while (1) {
        if (state == 6)
            break;
        z = line[idx];
        if (z == '\0')
            state = 6;
        else if (isspace(z))
            state = statemachine[state][0];
        else
            state = statemachine[state][1];
        /* printf("state: %d, char: '%c'\n", state, z); */
        idx++;

        if (state == 1) {
            aaa[aidx] = z;
            aidx++;
        }
        if (state == 2 && old_state == 1) {
            aaa[aidx] = '\0';
            aidx = 0;
            *a = atof(aaa);
        }

        if (state == 3) {
            aaa[aidx] = z;
            aidx++;
        }
        if (state == 4 && old_state == 3) {
            aaa[aidx] = '\0';
            aidx = 0;
            *b = atof(aaa);
        }

        if (state == 5) {
            aaa[aidx] = z;
            aidx++;
        }
        if (state == 6 && old_state == 5) {
            aaa[aidx] = '\0';
            aidx = 0;
            *c = atof(aaa);
        }
        old_state = state;
    }
    return state;
}

int mliMesh_malloc_from_object_file(const char *path, mliMesh* m) {
    FILE * fin;
    char line[1024];
    int len = 1024;
    int num_vertices;
    int num_faces;
    int not_used;
    uint32_t vertex_idx = 0;
    uint32_t face_idx = 0;

    fin = fopen(path, "r");
    mli_check(fin != NULL, "Can not open object-file for reading.");
    mli_check(fgets(line, len, fin) != NULL, "Can not read first line.");
    mli_check(strcmp(line, "OFF\n") == 0, "Expected first line to be 'OFF\\n'");
    mli_check(fgets(line, len, fin) != NULL, "Can not read second line.");
    mli_check(
        mli_parse_three_ints(line, &num_vertices, &num_faces, &not_used),
        "Can not parse num_vertices and num_faces.");
    m->num_faces = num_faces;
    m->num_vertices = num_vertices;
    mliMesh_malloc(m);

    while (1) {
        mli_check(fgets(line, len, fin) != NULL, "Can not read vertex line.");
        if (vertex_idx == m->num_vertices) break;
        if (strlen(line) > 1) {
            mliVec poi;
            mli_check(
                mli_parse_three_doubles(line, &poi.x, &poi.y, &poi.z),
                "Can not parse vertex.");
            m->vertices[vertex_idx] = poi;
            vertex_idx++;
        }
    }

    while (1) {
        if (strlen(line) > 1) {
            int ia, ib, ic;
            mliFace fa;
            mli_check(
                mli_parse_three_ints(strchr(line, ' '), &ia, &ib, &ic),
                "Can not parse face.");
            fa.a = ia; fa.b = ib; fa.c = ic;
            m->faces[face_idx] = fa;
            face_idx++;
        }
        if (face_idx == m->num_faces) break;
        mli_check(fgets(line, len, fin) != NULL, "Can not read face line.");
    }
    fclose(fin);
    return 1;
error:
    fclose(fin);
    return 0;
}


#endif
