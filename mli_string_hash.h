/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_STRING_HASH_H_
#define MERLICT_STRING_HASH_H_

#include <math.h>
#include <stdint.h>

#define MLI_DJB2_START_HASH 5381u

uint64_t mli_string_hash(char *str) {
    /* djb2 */
    /* http://www.cse.yorku.ca/~oz/hash.html */
    /* First reported by: Dan Bernstein */
    uint64_t hash = MLI_DJB2_START_HASH;
    int64_t c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;}

#endif
