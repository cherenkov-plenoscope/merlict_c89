/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict_c89_corsika/tests/test.c -o build/test_merlict_corsika -lm
 */

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "mli_testing.h"
#include "all_headers.h"
#include "all_sources.c"
#include "mli_corsika_all_headers.h"
#include "mli_corsika_all_sources.c"

int main(void)
{
        printf("MERLICT %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);
        printf("MERLICT_CORSIKA %d.%d.%d\n",
               MLI_CORSIKA_VERSION_MAYOR,
               MLI_CORSIKA_VERSION_MINOR,
               MLI_CORSIKA_VERSION_PATCH);

#include "mli_corsika_EventIo_test.c"
#include "mli_corsika_TarIo_test.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
