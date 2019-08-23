/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_STRING_TO_H_
#define MERLICT_MLI_STRING_TO_H_

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include "mli_debug.h"


int mli_string_to_int(int64_t *out, char *s, uint64_t base) {
    char *end;
    int64_t l;
    mli_check(! (s[0] == '\0' || isspace(s[0])),
        "Can not convert string to int64, bad string.");
    errno = 0;
    l = strtol(s, &end, base);
    mli_check(errno != ERANGE,
        "Can not convert string to int64, over-, under-flow.");
    mli_check(*end == '\0', "Can not convert string to int64, bad string.");
    *out = l;
    return 1;
error:
    return 0;
}


int mli_string_to_float(double *out, char *s) {
    char *end;
    double l;
    mli_check(! (s[0] == '\0' || isspace(s[0])),
        "Can not convert string to float64, bad string.");
    errno = 0;
    l = strtod(s, &end);
    mli_check(errno != ERANGE,
        "Can not convert string to float64, over-, under-flow.");
    mli_check(*end == '\0', "Can not convert string to float64, bad string.");
    *out = l;
    return 1;
error:
    return 0;
}

int mli_string_ends_with(const char *str, const char *suffix) {
    uint64_t len_str, len_suffix;
    if (!str || !suffix) {return 0;}
    len_str = strlen(str);
    len_suffix = strlen(suffix);
    if (len_suffix >  len_str) {return 0;}
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;}

#endif
