/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict/mli_test.c -o test -lm
 */

#include <math.h>
#include <assert.h>

#include "../tools/include.h"
#include "../tools/src.c"

#include "../tools/src.testing.c"

int main(void)
{
        printf("MLI_VERSION %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);

#include "../tools/src.test.c"

#include "mli_photon_propagation_prism.test.c"
#include "mli_photon_propagation_focussing_mirror.test.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
