/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "thin_lens.h"

double mli_thin_lens_get_object_given_focal_and_image(
        const double focal_length,
        const double image_distance)
{
        /* 1/f = 1/g + 1/b */
        /* 1/g = 1/f - 1/b */
        /* g = 1/(1/f - 1/b) */
        return 1.0 / (1.0 / focal_length - 1.0 / image_distance);
}

double mli_thin_lens_get_image_given_focal_and_object(
        const double focal_length,
        const double object_distance)
{
        /* 1/f = 1/g + 1/b */
        /* 1/b = 1/f - 1/g */
        /* b = 1/(1/f - 1/g) */
        return 1.0 / (1.0 / focal_length - 1.0 / object_distance);
}
