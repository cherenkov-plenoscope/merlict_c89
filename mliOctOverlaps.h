/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOCTOVERLAPS_H_
#define MERLICT_MLIOCTOVERLAPS_H_

#include <stdint.h>

typedef struct {
    uint32_t num;
    uint32_t count;
    uint32_t *objects;
} mliOctOverlap;

mliOctOverlap mliOctOverlap_init() {
    mliOctOverlap oo;
    oo.num = 0u;
    oo.count = 0u;
    oo.objects = NULL;
    return oo;}

void mliOctOverlap_free(mliOctOverlap *a) {
    free(a->objects);
    (*a) = mliOctOverlap_init();}

int mliOctOverlap_malloc(mliOctOverlap *a) {
    mli_malloc(a->objects, uint32_t, a->num);
    return 1;
error:
    return 0;}

int mliOctOverlap_push_back(mliOctOverlap *a, const uint32_t v) {
    mli_check(
        a->count < a->num,
        "Expected mliOctOverlap to have at least one free field");
    a->objects[a->count] = v;
    a->count += 1u;
    return 1;
error:
    return 0;}

#endif
