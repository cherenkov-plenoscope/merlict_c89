/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict_c89/mli_corsika_test.c -o build/mli_corsika_test -lm
 */

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include "mli.h"
#include "mli.c"
#include "mli_testing.h"
#include "mli_corsika.h"
#include "mli_corsika.c"
#include "mli_corsika_TarIo_testing.h"
#include "mli_corsika_TarIo_testing.c"

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
