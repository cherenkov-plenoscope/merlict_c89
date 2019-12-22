/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_TESTING_H_
#define MERLICT_TESTING_H_

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define CHECK_MARGIN(a, b, margin) \
    do { \
        if ( (fabs((a) - (b)) > margin) ) { \
            printf("  In %s, line %d\n", __FILE__, __LINE__); \
            printf("  Expected fabs(first - second) < %f.\n", margin); \
            printf("  first: %f\nsecond: %f\nmargin: %f\n", a, b, margin); \
            goto test_failure; \
        } \
    } while (0)

#define CHECK(test) \
    do { \
        if ( !(test) ) { \
            printf("  In %s, line %d\n", __FILE__, __LINE__); \
            printf("  Expected true\n"); \
            goto test_failure; \
        } \
    } while (0)

#define CASE(msg) \
    printf("%s: %d, %s\n", __FILE__, __LINE__, msg); \
    fflush(stdout); \

#endif
