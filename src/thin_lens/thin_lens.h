/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_THIN_LENS_H_
#define MLI_THIN_LENS_H_

double mli_thin_lens_get_object_given_focal_and_image(
        const double focal_length,
        const double image_distance);

double mli_thin_lens_get_image_given_focal_and_object(
        const double focal_length,
        const double object_distance);
#endif
