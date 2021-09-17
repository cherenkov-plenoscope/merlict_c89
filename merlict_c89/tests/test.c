/* Copyright 2019 Sebastian Achim Mueller                                     */

/*
 *  Compile with e.g.:
 *  gcc merlict_c89/tests/test.c -o test -lm
 */

#include <math.h>
#include <assert.h>
#include "../../merlict_c89/all_headers.h"
#include "../../merlict_c89/all_sources.c"
#include "mli_testing.h"

int main(void)
{
        printf("MLI_VERSION %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);

#include "test_mliMagicId.c"
#include "test_frame.c"
#include "test_parsing_from_string.c"
#include "test_tape_archive.c"
#include "test_comma_seperated_values.c"
#include "test_upper_bound.c"
#include "test_histogram.c"
#include "test_quadratic_equation.c"
#include "test_pseudo_random_number_generator.c"
#include "test_from_outside_to_inside.c"
#include "test_orientated_bounding_box.c"
#include "test_mliUserScenery.c"
#include "test_scenery.c"
#include "test_mliDynArray.c"
#include "test_mliQuaternion.c"
#include "test_homogeneous_transformation.c"
#include "test_math.c"
#include "test_mliColor.c"
#include "test_mliPixels.c"
#include "test_mliPixelWalk.c"
#include "test_mliImage.c"
#include "test_mliVec.c"
#include "test_linear_interpolation.c"
#include "test_mliFunc.c"
#include "test_mliRay.c"
#include "test_json.c"
#include "test_mliFresnel.c"
#include "test_mliAccelerator.c"
#include "test_mliApertureCamera.c"
#include "test_photon_sources.c"
#include "test_lambertian_cosine_law.c"
#include "test_map.c"
#include "test_cache_aware_octree.c"
#include "test_barycentric_coordinates.c"
#include "test_triangle_intersection.c"
#include "test_object_wavefront.c"
#include "test_mliArchive.c"
#include "test_photon.c"

#include "test_optics_prism.c"
#include "test_optics_focussing_mirror.c"

        printf("__SUCCESS__\n");
        return EXIT_SUCCESS;
test_failure:
        printf("__FAILURE__\n");
        return EXIT_FAILURE;
}
