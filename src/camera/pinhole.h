/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CAMERA_PINHOLE_H_
#define MLI_CAMERA_PINHOLE_H_

#include <stdint.h>
#include "../view/view.h"
#include "../vec/vec.h"

struct mli_PathTracer;
struct mli_Image;
struct mli_Prng;

struct mli_camera_PinHole {
        struct mli_Vec optical_axis;
        struct mli_Vec col_axis;
        struct mli_Vec row_axis;
        struct mli_Vec principal_point;
        double distance_to_principal_point;
        double row_over_column_pixel_ratio;
};

struct mli_camera_PinHole mli_camera_PinHole_set(
        const double field_of_view,
        const struct mli_Image *image,
        const double row_over_column_pixel_ratio);

void mli_camera_PinHole_render_image(
        struct mli_camera_PinHole self,
        const struct mli_HomTraComp camera2root_comp,
        const struct mli_PathTracer *pathtracer,
        struct mli_Image *image,
        struct mli_Prng *prng);

void mli_camera_PinHole_render_image_with_view(
        const struct mli_View view,
        const struct mli_PathTracer *pathtracer,
        struct mli_Image *image,
        const double row_over_column_pixel_ratio,
        struct mli_Prng *prng);

struct mli_Ray mli_camera_PinHole_ray_at_row_col(
        const struct mli_camera_PinHole *self,
        const struct mli_Image *image,
        const uint32_t row,
        const uint32_t col);

#endif
