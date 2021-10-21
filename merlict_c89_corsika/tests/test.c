/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict_c89_corsika/tests/test.c -o build/test_merlict_corsika -lm
 */

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "../../merlict_c89/all_headers.h"
#include "../../merlict_c89/all_sources.c"
#include "../../merlict_c89_corsika/all_headers.h"
#include "../../merlict_c89_corsika/all_sources.c"
#include "../../merlict_c89/mli_testing.h"

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

#include "test_EventIo.c"
#include "test_mliTarIo.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
