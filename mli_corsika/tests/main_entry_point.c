/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict/mli_corsika_test.c -o build/mli_corsika_test -lm
 */

#include <math.h>
#include <assert.h>
#include <stdlib.h>

#include "../../chk_debug/include/chk_debug.h"
#include "../../chk_debug/src.c"

#include "../../mli_testing/include/mli_testing.h"
#include "../../mli_testing/src.c"

#include "../../mli_core/include/mli_core.h"
#include "../../mli_core/src.c"

#include "../include/mli_corsika.h"
#include "../src.c"

#include "../src/mli_corsika_EventTape_testing.h"
#include "../src/mli_corsika_EventTape_testing.c"

int main(void)
{
        printf("MLI %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);
        printf("MLI_CORSIKA %d.%d.%d\n",
               MLI_CORSIKA_VERSION_MAYOR,
               MLI_CORSIKA_VERSION_MINOR,
               MLI_CORSIKA_VERSION_PATCH);
        printf("MLI_CORSIKA_EVENTTAPE %d.%d.%d\n",
               MLI_CORSIKA_EVENTTAPE_VERSION_MAYOR,
               MLI_CORSIKA_EVENTTAPE_VERSION_MINOR,
               MLI_CORSIKA_EVENTTAPE_VERSION_PATCH);

#include "mli_corsika_EventIo_test.c"
#include "mli_corsika_EventTape_test.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
