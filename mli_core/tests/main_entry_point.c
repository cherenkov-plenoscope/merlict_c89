/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict/mli_test.c -o test -lm
 */

#include <math.h>
#include <assert.h>

#include "../../chk_debug/include/chk_debug.h"
#include "../../chk_debug/src.c"

#include "../include/mli_core.h"
#include "../src.c"

#include "mliDynArray_testing.h"
#include "mliDynArray_color_testing.h"
#include "mliDynArray_color_testing.c"

int main(void)
{
        printf("MLI_VERSION %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);

#include "mliMagicId.test.c"
#include "mliFrame.test.c"
#include "mli_cstr.test.c"
#include "mli_cstr_numbers.test.c"
#include "mliStr_numbers.test.c"
#include "mliTar.test.c"
#include "mli_quadratic_equation.test.c"
#include "mli_random.test.c"
#include "mli_from_outside_to_inside.test.c"
#include "mliCube.test.c"
#include "mliDynArray.test.c"
#include "mliQuaternion.test.c"
#include "mliHomTra.test.c"
#include "mli_math.test.c"
#include "mliColor.test.c"
#include "mliPixels.test.c"
#include "mliPixelWalk.test.c"
#include "mliImage.test.c"
#include "mliVec.test.c"
#include "mliMat.test.c"
#include "mli_lambertian_cosine_law.test.c"
#include "mliDynMap.test.c"
#include "mliArchive.test.c"
#include "mli_barycentric.test.c"
#include "mli_triangle_intersection.test.c"
#include "mliObject_wavefront.test.c"
#include "mliRay.test.c"
#include "mliFunc.test.c"
#include "mliMedium.test.c"
#include "mliSurface.test.c"
#include "mli_json.test.c"
#include "mliFresnel.test.c"
#include "mliApertureCamera.test.c"
#include "mli_photon_sources.test.c"
#include "mliOcTree.test.c"
#include "mliUserScenery.test.c"
#include "mliAccelerator.test.c"
#include "mliGeometry.test.c"
#include "mli_photon_propagation.test.c"
#include "mli_photon_propagation_prism.test.c"
#include "mli_photon_propagation_focussing_mirror.test.c"
#include "mliRenderConfig.test.c"
#include "mliStr.test.c"
#include "mliIo.test.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
