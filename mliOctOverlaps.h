/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTOVERLAPS_H_
#define MERLICT_MLIOCTOVERLAPS_H_

#include <stdint.h>

typedef struct {
    uint32_t num[8];
    uint32_t **objects;
} mliOctOverlaps;

void mliOctOverlaps_init(mliOctOverlaps *a, const uint32_t max_num_objects) {
    uint32_t c;
    a->objects = (uint32_t**)malloc(8u*sizeof(uint32_t*));
    for (c = 0; c < 8u; c++) {
        a->objects[c] = (uint32_t*)malloc(max_num_objects*sizeof(uint32_t));}}

void mliOctOverlaps_free(mliOctOverlaps *a) {
    uint32_t c;
    for (c = 0; c < 8u; c++) {
        free(a->objects[c]);}
    free(a->objects);}

#endif
