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
#include "test_from_outside_to_inside.c"
#include "test_orientated_bounding_box.c"
#include "test_mliDynArray.c"
#include "../../merlict_c89/mliQuaternion_test.c"
#include "test_homogeneous_transformation.c"
#include "../../merlict_c89/mli_math_test.c"
#include "test_mliColor.c"
#include "test_mliPixels.c"
#include "test_mliPixelWalk.c"
#include "test_mliImage.c"
#include "../../merlict_c89/mliVec_test.c"
#include "test_lambertian_cosine_law.c"
#include "test_map.c"
#include "test_mliArchive.c"
#include "../../merlict_c89/mli_barycentric_test.c"
#include "test_triangle_intersection.c"
#include "test_object_wavefront.c"
#include "test_mliRay.c"
#include "test_mliUserScenery.c"
#include "test_scenery.c"
#include "../../merlict_c89/mliFunc_test.c"
#include "test_mliMedium.c"
#include "test_mliSurface.c"
#include "test_json.c"
#include "test_mliFresnel.c"
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
