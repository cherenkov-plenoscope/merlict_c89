// Copyright 2019 Sebastian Achim Mueller
#ifndef MERLICT_UNITTEST_H_
#define MERLICT_UNITTEST_H_

#include <math.h>
#include <stdio.h>

#define CHECK_MARGIN(a, b, margin) \
    do { \
        if ( (fabs(a - b) > margin) ) { \
            printf("In %s, line %d\n", __FILE__, __LINE__); \
            printf("Expected fabs(%f - %f) < %f.\n", a, b, margin); \
            return EXIT_FAILURE; \
        } \
    } while (0)

#define CHECK(test) \
    do { \
        if ( !(test) ) { \
            printf("In %s, line %d\n", __FILE__, __LINE__); \
            printf("Expected true\n"); \
            return EXIT_FAILURE; \
        } \
    } while (0)

#endif  // MERLICT_UNITTEST_H_
