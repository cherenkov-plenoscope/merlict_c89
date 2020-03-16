/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with e.g.:                                                         */
/* gcc   merlict-c89-test.c -o merlict-c89-test -std=c89 -lm -Wall -pedantic  */
/* g++   merlict-c89-test.c -o merlict-c89-test          -lm -Wall -pedantic  */
/* clang merlict-c89-test.c -o merlict-c89-test -std=c89 -lm -Wall -pedantic  */
/* clang merlict-c89-test.c -o merlict-c89-test          -lm -Wall -pedantic  */
#include <math.h>
#include <assert.h>
#include "merlict_c89.h"

int main(int argc, char *argv[]) {
    #include "tests/test_frame.c"
    #include "tests/test_upper_bound.c"
    #include "tests/test_histogram.c"
    #include "tests/test_pseudo_random_number_generator.c"
    #include "tests/test_from_outside_to_inside.c"
    #include "tests/test_orientated_bounding_box.c"
    #include "tests/test_sphere_intersection_equation.c"
    #include "tests/test_scenery.c"
    #include "tests/test_mliDynArray.c"
    #include "tests/test_mliQuaternion.c"
    #include "tests/test_homogeneous_transformation.c"
    #include "tests/test_math.c"
    #include "tests/test_quadratic_equation.c"
    #include "tests/test_mliColor.c"
    #include "tests/test_mliPixels.c"
    #include "tests/test_mliImage.c"
    #include "tests/test_mliVec.c"
    #include "tests/test_linear_interpolation.c"
    #include "tests/test_mliFunc.c"
    #include "tests/test_mliMesh.c"
    #include "tests/test_mliHexagonalPrismZ.c"
    #include "tests/test_mliSphericalCap.c"
    #include "tests/test_mliSphere.c"
    #include "tests/test_mliCylinder.c"
    #include "tests/test_mliHexagon.c"
    #include "tests/test_mliBiCirclePlane.c"
    #include "tests/test_mliDisc.c"
    #include "tests/test_mliRay.c"
    #include "tests/test_dual_spherical_prism.c"
    #include "tests/test_parsing_from_string.c"
    #include "tests/test_json.c"
    #include "tests/test_mliFresnel.c"
    #include "tests/test_photon.c"
    #include "tests/test_photon_sources.c"
    #include "tests/test_lambertian_cosine_law.c"
    #include "tests/test_lensmaker.c"
    #include "tests/test_variable_length_integer.c"
    #include "tests/test_map.c"
    #include "tests/test_cache_aware_octree.c"


    printf("__SUCCESS__\n");
    return EXIT_SUCCESS;
test_failure:
    printf("__FAILURE__\n");
    return EXIT_FAILURE;
}
