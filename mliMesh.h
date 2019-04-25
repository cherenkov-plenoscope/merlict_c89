// Copyright 2019 Sebastian Achim Mueller
#ifndef MERLICT_MLIMESH_H_
#define MERLICT_MLIMESH_H_

#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>


struct mliFace {
    uint32_t a;
    uint32_t b;
    uint32_t c;};


struct mliMesh {
    uint32_t num_vertices;
    struct mliVec *vertices;
    uint32_t num_faces;
    struct mliFace *faces;};


void mliMesh_init(
    struct mliMesh* m,
    const uint32_t num_vertices,
    const uint32_t num_faces) {
    m->num_vertices = num_vertices;
    m->vertices = (struct mliVec*)malloc(m->num_vertices*sizeof(struct mliVec));
    m->num_faces = num_faces;
    m->faces = (struct mliFace*)malloc(m->num_faces*sizeof(struct mliFace));}


void mliMesh_free(struct mliMesh *m) {
    free(m->vertices);
    m->num_vertices = 0;
    free(m->faces);
    m->num_faces = 0;}

int ml_parse_three_ints(const char *line, int *a, int* b, int*c) {
    int state = 0;
    int old_state = state;
    int statemachine[][2] = {
    //  ws  print
        {0,   1},  // 0
        {2,   1},  // 1 first
        {2,   3},  // 2
        {4,   3},  // 3 second
        {4,   5},  // 4
        {6,   5},  // 5 third
    };
    int idx = 0;
    char aaa[64];
    int aidx = 0;
    while (true) {
        if (state == 6)
            break;
        char z = line[idx];
        if (z == '\0')
            state = 6;
        else if (isspace(z))
            state = statemachine[state][0];
        else
            state = statemachine[state][1];
        // printf("state: %d, char: '%c'\n", state, z);
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


int ml_parse_three_floats(const char *line, float *a, float* b, float*c) {
    int state = 0;
    int old_state = state;
    int statemachine[][2] = {
    //  ws  print
        {0,   1},  // 0
        {2,   1},  // 1 first
        {2,   3},  // 2
        {4,   3},  // 3 second
        {4,   5},  // 4
        {6,   5},  // 5 third
    };
    int idx = 0;
    char aaa[64];
    int aidx = 0;
    while (true) {
        if (state == 6)
            break;
        char z = line[idx];
        if (z == '\0')
            state = 6;
        else if (isspace(z))
            state = statemachine[state][0];
        else
            state = statemachine[state][1];
        // printf("state: %d, char: '%c'\n", state, z);
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


int mliMesch_init_from_off(const char *path, struct mliMesh* m) {
    FILE * fin;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fin = fopen(path, "r");
    if (fin == NULL) return EXIT_FAILURE;

    read = getline(&line, &len, fin);
    if (read == -1) return -1;
    if (!strcmp(line, "OFF")) return EXIT_FAILURE;

    read = getline(&line, &len, fin);
    if (read == -1) return -1;
    int num_vertices;
    int num_faces;
    int not_used;
    ml_parse_three_ints(line, &num_vertices, &num_faces, &not_used);
    // printf("num ver %i\n", num_vertices);

    mliMesh_init(m, num_vertices, num_faces);

    uint32_t vertex_idx = 0;
    while (true) {
        read = getline(&line, &len, fin);
        if (read == -1) return - 1;
        if (vertex_idx == m->num_vertices) break;
        if (strlen(line) > 1) {
            // printf("'%s'\n", line);
            struct mliVec poi;
            ml_parse_three_floats(line, &poi.x, &poi.y, &poi.z);
            m->vertices[vertex_idx] = poi;
            // printf("%f %f %f\n", a, b, c);
            vertex_idx++;
        }
    }

    uint32_t face_idx = 0;
    while (true) {
        if (face_idx == m->num_faces) break;
        if (strlen(line) > 1) {
            int ia, ib, ic;
            ml_parse_three_ints(strchr(line, ' '), &ia, &ib, &ic);
            struct mliFace fa;
            fa.a = ia; fa.b = ib; fa.c = ic;
            m->faces[face_idx] = fa;
            // printf("%f %f %f\n", a, b, c);
            face_idx++;
        }
        read = getline(&line, &len, fin);
        if (read == -1) return -1;
    }

    fclose(fin);
    if (line)
        free(line);
    return EXIT_SUCCESS;
}


#endif  // MERLICT_MLIMESH_H_
