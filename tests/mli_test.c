/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict/mli_test.c -o test -lm
 */

#include <math.h>
#include <assert.h>
#include "mli_poor_mans_build_system.h"
#include "mli_poor_mans_build_system.c"

int main(void)
{
        printf("MLI_VERSION %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);

#include "../src/mliMagicId_test.c"
#include "../src/mliFrame_test.c"
#include "../src/mli_cstr_test.c"
#include "../src/mli_cstr_numbers_test.c"
#include "../src/mliStr_numbers_test.c"
#include "../src/mliTar_test.c"
#include "../src/mli_quadratic_equation_test.c"
#include "../src/mli_random_test.c"
#include "../src/mli_from_outside_to_inside_test.c"
#include "../src/mliCube_test.c"
#include "../src/mliDynArray_test.c"
#include "../src/mliQuaternion_test.c"
#include "../src/mliHomTra_test.c"
#include "../src/mli_math_test.c"
#include "../src/mliColor_test.c"
#include "../src/mliPixels_test.c"
#include "../src/mliPixelWalk_test.c"
#include "../src/mliImage_test.c"
#include "../src/mliVec_test.c"
#include "../src/mliMat_test.c"
#include "../src/mli_lambertian_cosine_law_test.c"
#include "../src/mliDynMap_test.c"
#include "../src/mliArchive_test.c"
#include "../src/mli_barycentric_test.c"
#include "../src/mli_triangle_intersection_test.c"
#include "../src/mliObject_wavefront_test.c"
#include "../src/mliRay_test.c"
#include "../src/mliFunc_test.c"
#include "../src/mliMedium_test.c"
#include "../src/mliSurface_test.c"
#include "../src/mli_json_test.c"
#include "../src/mliFresnel_test.c"
#include "../src/mliApertureCamera_test.c"
#include "../src/mli_photon_sources_test.c"
#include "../src/mliOcTree_test.c"
#include "../src/mliUserScenery_test.c"
#include "../src/mliAccelerator_test.c"
#include "../src/mliGeometry_test.c"
#include "../src/mli_photon_propagation_test.c"
#include "../src/mli_photon_propagation_prism_test.c"
#include "../src/mli_photon_propagation_focussing_mirror_test.c"
#include "../src/mliRenderConfig_test.c"
#include "../src/mliStr_test.c"
#include "../src/mliIo_test.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
