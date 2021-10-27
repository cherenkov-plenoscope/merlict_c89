/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict_c89/mli_test.c -o test -lm
 */

#include <math.h>
#include <assert.h>
#include "mli.h"
#include "mli.c"
#include "mli_testing.h"

int main(void)
{
        printf("MLI_VERSION %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);

#include "mliMagicId_test.c"
#include "mliFrame_test.c"
#include "mli_cstr_test.c"
#include "mli_cstr_numbers_test.c"
#include "mliTar_test.c"
#include "mli_quadratic_equation_test.c"
#include "mli_random_test.c"
#include "mli_from_outside_to_inside_test.c"
#include "mliCube_test.c"
#include "mliDynArray_template_test.c"
#include "mliQuaternion_test.c"
#include "mliHomTra_test.c"
#include "mli_math_test.c"
#include "mliColor_test.c"
#include "mliPixels_test.c"
#include "mliPixelWalk_test.c"
#include "mliImage_test.c"
#include "mliVec_test.c"
#include "mli_lambertian_cosine_law_test.c"
#include "mliDynMap_test.c"
#include "mliArchive_test.c"
#include "mli_barycentric_test.c"
#include "mli_triangle_intersection_test.c"
#include "mliObject_wavefront_test.c"
#include "mliRay_test.c"
#include "mliFunc_test.c"
#include "mliMedium_test.c"
#include "mliSurface_test.c"
#include "mli_json_test.c"
#include "mliFresnel_test.c"
#include "mliApertureCamera_test.c"
#include "mli_photon_sources_test.c"
#include "mliOcTree_test.c"
#include "mliUserScenery_test.c"
#include "mliAccelerator_test.c"
#include "mliGeometry_test.c"
#include "mli_photon_propagation_test.c"
#include "mli_photon_propagation_prism_test.c"
#include "mli_photon_propagation_focussing_mirror_test.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
