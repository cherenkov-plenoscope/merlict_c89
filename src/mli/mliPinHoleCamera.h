/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATH_PIN_HOLE_CAMERA_H_
#define MLI_MATH_PIN_HOLE_CAMERA_H_

#include <stdint.h>
#include "mliTracer.h"
#include "mliView.h"
#include "../image/image.h"

struct mliPinHoleCamera {
        struct mliVec optical_axis;
        struct mliVec col_axis;
        struct mliVec row_axis;
        struct mliVec principal_point;
        double distance_to_principal_point;
        double row_over_column_pixel_ratio;
};

struct mliPinHoleCamera mliPinHoleCamera_init(
        const double field_of_view,
        const struct mli_Image *image,
        const double row_over_column_pixel_ratio);

void mliPinHoleCamera_render_image(
        struct mliPinHoleCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliTracer *tracer,
        struct mli_Image *image,
        struct mli_Prng *prng);

void mliPinHoleCamera_render_image_with_view(
        const struct mliView view,
        const struct mliTracer *tracer,
        struct mli_Image *image,
        const double row_over_column_pixel_ratio,
        struct mli_Prng *prng);

struct mliRay mliPinHoleCamera_ray_at_row_col(
        const struct mliPinHoleCamera *camera,
        const struct mli_Image *image,
        const uint32_t row,
        const uint32_t col);

#endif
