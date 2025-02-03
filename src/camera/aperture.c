/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "aperture.h"
#include <math.h>
#include <assert.h>
#include "../vec/vec_random.h"
#include "../math/math.h"
#include "../chk/chk.h"
#include "../thin_lens/thin_lens.h"
#include "../pathtracer/pathtracer.h"

struct mli_camera_Aperture mli_camera_Aperture_init(void)
{
        const double mm = 1e-3;
        struct mli_camera_Aperture apcam;
        apcam.focal_length = 50.0 * mm;
        apcam.aperture_radius = apcam.focal_length / 2.0;
        apcam.image_sensor_distance = apcam.focal_length;
        apcam.image_sensor_width_x = 36.0 * mm;
        apcam.image_sensor_width_y = 24.0 * mm;
        return apcam;
}

struct mli_Vec mli_camera_Aperture_pixel_center_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y)
{
        struct mli_Vec pixel_center;
        pixel_center.x = -1.0 * mli_math_bin_center_in_linear_space(
                                        -0.5 * image_sensor_width_x,
                                        +0.5 * image_sensor_width_x,
                                        num_pixel_x,
                                        pixel_x);
        pixel_center.y = -1.0 * mli_math_bin_center_in_linear_space(
                                        -0.5 * image_sensor_width_y,
                                        +0.5 * image_sensor_width_y,
                                        num_pixel_y,
                                        pixel_y);
        pixel_center.z = -image_sensor_distance;
        return pixel_center;
}

struct mli_Vec mli_camera_Aperture_pixel_support_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mli_Prng *prng)
{
        double pixel_bin_width_x = image_sensor_width_x / (double)num_pixel_x;
        double pixel_bin_width_y = image_sensor_width_y / (double)num_pixel_y;
        struct mli_Vec support =
                mli_camera_Aperture_pixel_center_on_image_sensor_plane(
                        image_sensor_width_x,
                        image_sensor_width_y,
                        image_sensor_distance,
                        num_pixel_x,
                        num_pixel_y,
                        pixel_x,
                        pixel_y);
        double rnd_x =
                (mli_Prng_uniform(prng) * pixel_bin_width_x -
                 0.5 * pixel_bin_width_x);
        double rnd_y =
                (mli_Prng_uniform(prng) * pixel_bin_width_y -
                 0.5 * pixel_bin_width_y);
        support.x = support.x + rnd_x;
        support.y = support.y + rnd_y;
        return support;
}

struct mli_Vec mli_camera_Aperture_get_object_point(
        const double focal_length,
        const struct mli_Vec pixel_support)
{
        const double object_distance =
                mli_thin_lens_get_object_given_focal_and_image(
                        focal_length, -1.0 * pixel_support.z);
        const double scaleing = object_distance / pixel_support.z;
        return mli_Vec_init(
                scaleing * pixel_support.x,
                scaleing * pixel_support.y,
                object_distance);
}

struct mli_Vec mli_camera_Aperture_ray_support_on_aperture(
        const double aperture_radius,
        struct mli_Prng *prng)
{
        /* use a perfect disc as aperture */
        return mli_Vec_random_position_on_disc(aperture_radius, prng);
}

double mli_camera_Aperture_focal_length_given_field_of_view_and_sensor_width(
        const double field_of_view,
        const double image_sensor_width)
{
        const double image_sensor_radius = 0.5 * image_sensor_width;
        const double fov_opening_angle = 0.5 * field_of_view;
        return image_sensor_radius / tan(fov_opening_angle);
}

struct mli_Ray mli_camera_Aperture_get_ray_for_pixel(
        const double focal_length,
        const double aperture_radius,
        const double image_sensor_distance,
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mli_Prng *prng)
{
        struct mli_Vec direction;
        struct mli_Vec aperture_support =
                mli_camera_Aperture_ray_support_on_aperture(
                        aperture_radius, prng);

        struct mli_Vec image_sensor_support =
                (mli_camera_Aperture_pixel_support_on_image_sensor_plane(
                        image_sensor_width_x,
                        image_sensor_width_y,
                        image_sensor_distance,
                        num_pixel_x,
                        num_pixel_y,
                        pixel_x,
                        pixel_y,
                        prng));

        if (fabs(1.0 - focal_length / image_sensor_distance) < 1e-6) {
                /* focus set to infinity */
                direction = mli_Vec_multiply(image_sensor_support, -1.0);
        } else {
                struct mli_Vec object_point =
                        mli_camera_Aperture_get_object_point(
                                focal_length, image_sensor_support);

                direction = mli_Vec_substract(object_point, aperture_support);
        }

        return mli_Ray_set(aperture_support, direction);
}

void mli_camera_Aperture_aquire_pixels(
        const struct mli_camera_Aperture self,
        const struct mli_Image *image,
        const struct mli_HomTraComp camera2root_comp,
        const struct mli_Shader *tracer,
        const struct mli_image_PixelVector *pixels_to_do,
        struct mli_ColorVector *colors_to_do,
        struct mli_Prng *prng)
{
        uint64_t i;
        struct mli_HomTra camera2root =
                mli_HomTraComp_from_compact(camera2root_comp);

        colors_to_do->size = 0;
        for (i = 0; i < pixels_to_do->size; i++) {
                struct mli_Ray ray_wrt_camera =
                        mli_camera_Aperture_get_ray_for_pixel(
                                self.focal_length,
                                self.aperture_radius,
                                self.image_sensor_distance,
                                self.image_sensor_width_x,
                                self.image_sensor_width_y,
                                mli_Image_num_cols(image),
                                mli_Image_num_rows(image),
                                pixels_to_do->array[i].col,
                                pixels_to_do->array[i].row,
                                prng);

                struct mli_Ray ray_wrt_root =
                        mli_HomTraComp_ray(&camera2root, ray_wrt_camera);

                struct mli_Color set_color =
                        mli_Shader_trace_ray(tracer, ray_wrt_root, prng);

                mli_ColorVector_push_back(colors_to_do, set_color);
        }

        return;
}

void mli_camera_Aperture_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mli_image_PixelVector *pixels,
        const struct mli_ColorVector *colors,
        struct mli_Image *sum_image,
        struct mli_Image *exposure_image)
{
        uint64_t i;
        const struct mli_Color ONE = mli_Color_set(1.0, 1.0, 1.0);
        for (i = 0; i < pixels->size; i++) {
                const struct mli_image_Pixel pix = pixels->array[i];
                const struct mli_Color color = colors->array[i];

                const struct mli_Color _exposure =
                        mli_Image_get_by_Pixel(exposure_image, pix);
                const struct mli_Color _sum =
                        mli_Image_get_by_Pixel(sum_image, pix);

                mli_Image_set_by_Pixel(
                        sum_image, pix, mli_Color_add(_sum, color));
                mli_Image_set_by_Pixel(
                        exposure_image, pix, mli_Color_add(_exposure, ONE));
        }
}

int mli_camera_Aperture_render_image(
        const struct mli_camera_Aperture self,
        const struct mli_HomTraComp camera2root_comp,
        const struct mli_Shader *tracer,
        struct mli_Image *image,
        struct mli_Prng *prng)
{
        float noise_threshold = 0.05 * 255.0;
        uint64_t MAX_ITERATIONS = 128;
        uint64_t iteration = 0;

        struct mli_Color zero_color = mli_Color_set(0.0, 0.0, 0.0);
        const struct mli_Color HIGH_COLOR = mli_Color_set(255.0, 255.0, 255.0);
        struct mli_Image sum_image = mli_Image_init();
        struct mli_Image exposure_image = mli_Image_init();
        struct mli_Image to_do_image = mli_Image_init();
        struct mli_Image sobel_image = mli_Image_init();
        struct mli_Image previous_sobel_image = mli_Image_init();
        struct mli_Image diff_image = mli_Image_init();
        struct mli_ColorVector colors_to_do = mli_ColorVector_init();
        struct mli_image_PixelVector pixels_to_do =
                mli_image_PixelVector_init();
        chk_msg(mli_Image_malloc_same_size(&sum_image, image),
                "Failed to malloc sum_image.");
        chk_msg(mli_Image_malloc_same_size(&exposure_image, image),
                "Failed to malloc exposure_image.");
        chk_msg(mli_Image_malloc_same_size(&to_do_image, image),
                "Failed to malloc to_do_image.");
        chk_msg(mli_Image_malloc_same_size(&sobel_image, image),
                "Failed to malloc sobel_image.");
        chk_msg(mli_Image_malloc_same_size(&previous_sobel_image, image),
                "Failed to malloc previous_sobel_image.");
        chk_msg(mli_Image_malloc_same_size(&diff_image, image),
                "Failed to malloc diff_image.");
        chk_msg(mli_image_PixelVector_malloc(
                        &pixels_to_do, mli_Image_num_pixel(image)),
                "Failed to malloc pixels_to_do.");
        chk_msg(mli_ColorVector_malloc(
                        &colors_to_do, mli_Image_num_pixel(image)),
                "Failed to malloc colors_to_do.");

        mli_Image_set_all(image, zero_color);
        mli_Image_set_all(&sum_image, zero_color);
        mli_Image_set_all(&exposure_image, zero_color);
        mli_Image_set_all(&to_do_image, zero_color);
        mli_Image_set_all(&sobel_image, zero_color);
        mli_Image_set_all(&previous_sobel_image, zero_color);
        MLI_MATH_ARRAY_SET(
                colors_to_do.array, zero_color, colors_to_do.capacity);

        /*
        initial image
        =============
        */
        mli_image_PixelVector_push_back_all_from_image(&pixels_to_do, image);
        mli_camera_Aperture_aquire_pixels(
                self,
                image,
                camera2root_comp,
                tracer,
                &pixels_to_do,
                &colors_to_do,
                prng);
        mli_camera_Aperture_assign_pixel_colors_to_sum_and_exposure_image(
                &pixels_to_do, &colors_to_do, &sum_image, &exposure_image);
        mli_Image_divide_pixelwise(&sum_image, &exposure_image, image);
        chk(mli_Image_sobel(image, &sobel_image));
        mli_Image_luminance_threshold_dilatation(
                &sobel_image, 128.0, HIGH_COLOR, &to_do_image);

        fprintf(stderr, "\n");
        while (1) {
                if (iteration >= MAX_ITERATIONS)
                        break;
                mli_image_PixelVector_above_threshold(
                        &to_do_image, 0.5, &pixels_to_do);
                if (pixels_to_do.size < mli_Image_num_pixel(image) / 100.0)
                        break;
                fprintf(stderr,
                        "loop %3u / %3u, %ld,%03ld pixel left\n",
                        (uint32_t)iteration + 1,
                        (uint32_t)MAX_ITERATIONS,
                        pixels_to_do.size / 1000,
                        pixels_to_do.size % 1000);
                mli_camera_Aperture_aquire_pixels(
                        self,
                        image,
                        camera2root_comp,
                        tracer,
                        &pixels_to_do,
                        &colors_to_do,
                        prng);
                mli_camera_Aperture_assign_pixel_colors_to_sum_and_exposure_image(
                        &pixels_to_do,
                        &colors_to_do,
                        &sum_image,
                        &exposure_image);
                mli_Image_divide_pixelwise(&sum_image, &exposure_image, image);
                chk(mli_Image_copy(&sobel_image, &previous_sobel_image));
                chk(mli_Image_sobel(image, &sobel_image));
                chk(mli_Image_fabs_difference(
                        &previous_sobel_image, &sobel_image, &diff_image));
                mli_Image_set_all(&to_do_image, zero_color);
                chk(mli_Image_luminance_threshold_dilatation(
                        &diff_image,
                        noise_threshold,
                        HIGH_COLOR,
                        &to_do_image));
                iteration += 1;
        }

        mli_Image_free(&sum_image);
        mli_Image_free(&exposure_image);
        mli_Image_free(&to_do_image);
        mli_Image_free(&sobel_image);
        mli_Image_free(&previous_sobel_image);
        mli_Image_free(&diff_image);
        mli_ColorVector_free(&colors_to_do);
        mli_image_PixelVector_free(&pixels_to_do);
        return 1;
chk_error:
        mli_Image_free(&sum_image);
        mli_Image_free(&exposure_image);
        mli_Image_free(&to_do_image);
        mli_Image_free(&sobel_image);
        mli_Image_free(&previous_sobel_image);
        mli_Image_free(&diff_image);
        mli_ColorVector_free(&colors_to_do);
        mli_image_PixelVector_free(&pixels_to_do);
        return 0;
}
