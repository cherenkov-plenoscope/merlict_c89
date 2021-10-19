/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict_c89/tests/test.c -o test -lm
 */

#include <math.h>
#include <assert.h>
#include "../../merlict_c89/all_headers.h"
#include "../../merlict_c89/all_sources.c"
#include "../../merlict_c89/mli_testing.h"

int main(void)
{
        printf("MLI_VERSION %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);

#include "../../merlict_c89/mliMagicId_test.c"
#include "../../merlict_c89/mliFrame_test.c"
#include "../../merlict_c89/mli_cstr_test.c"
#include "../../merlict_c89/mli_cstr_and_numbers_test.c"
#include "../../merlict_c89/mliTar_test.c"
#include "../../merlict_c89/mli_quadratic_equation_test.c"
#include "../../merlict_c89/mli_random_test.c"
#include "../../merlict_c89/mli_from_outside_to_inside_test.c"
#include "../../merlict_c89/mliCube_test.c"
#include "../../merlict_c89/mliDynArray_template_test.c"
#include "../../merlict_c89/mliQuaternion_test.c"
#include "../../merlict_c89/mliHomTra_test.c"
#include "../../merlict_c89/mli_math_test.c"
#include "../../merlict_c89/mliColor_test.c"
#include "../../merlict_c89/mliPixels_test.c"
#include "../../merlict_c89/mliPixelWalk_test.c"
#include "../../merlict_c89/mliImage_test.c"
#include "../../merlict_c89/mliVec_test.c"
#include "../../merlict_c89/mli_lambertian_cosine_law_test.c"
#include "../../merlict_c89/mliDynMap_test.c"
#include "../../merlict_c89/mliArchive_test.c"
#include "../../merlict_c89/mli_barycentric_test.c"
#include "../../merlict_c89/mli_triangle_intersection_test.c"
#include "../../merlict_c89/mliObject_wavefront_test.c"
#include "../../merlict_c89/mliRay_test.c"
#include "test_mliUserScenery.c"
#include "test_scenery.c"
#include "../../merlict_c89/mliFunc_test.c"
#include "../../merlict_c89/mliMedium_test.c"
#include "../../merlict_c89/mliSurface_test.c"
#include "../../merlict_c89/mli_json_test.c"
#include "../../merlict_c89/mliFresnel_test.c"
#include "test_mliAccelerator.c"
#include "test_mliApertureCamera.c"
#include "test_photon_sources.c"
#include "test_cache_aware_octree.c"
#include "test_photon.c"
#include "test_optics_prism.c"
#include "test_optics_focussing_mirror.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
