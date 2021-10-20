/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAptCam.h"
#include <math.h>
#include <assert.h>

struct mliVec mliAptCam_pixel_center_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y)
{
        struct mliVec pixel_center;
        pixel_center.x = -1.0 * mli_bin_center_in_linear_space(
                                        -0.5 * image_sensor_width_x,
                                        +0.5 * image_sensor_width_x,
                                        num_pixel_x,
                                        pixel_x);
        pixel_center.y = -1.0 * mli_bin_center_in_linear_space(
                                        -0.5 * image_sensor_width_y,
                                        +0.5 * image_sensor_width_y,
                                        num_pixel_y,
                                        pixel_y);
        pixel_center.z = -image_sensor_distance;
        return pixel_center;
}

struct mliVec mliAptCam_draw_random_support_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliPrng *prng)
{
        double pixel_bin_width_x = image_sensor_width_x / (double)num_pixel_x;
        double pixel_bin_width_y = image_sensor_width_y / (double)num_pixel_y;
        struct mliVec support = mliAptCam_pixel_center_on_image_sensor_plane(
                image_sensor_width_x,
                image_sensor_width_y,
                image_sensor_distance,
                num_pixel_x,
                num_pixel_y,
                pixel_x,
                pixel_y);
        double rnd_x =
                (mli_random_uniform(prng) * pixel_bin_width_x -
                 0.5 * pixel_bin_width_x);
        double rnd_y =
                (mli_random_uniform(prng) * pixel_bin_width_y -
                 0.5 * pixel_bin_width_y);
        support.x = support.x + rnd_x;
        support.y = support.y + rnd_y;
        return support;
}

struct mliVec mliAptCam_get_object_point(
        const double focal_length,
        const struct mliVec pixel_support)
{
        const double object_distance = mli_thin_lens_get_object_given_focal_and_image(
                focal_length, -1.0 * pixel_support.z);
        const double scaleing = object_distance / pixel_support.z;
        return mliVec_set(
                scaleing * pixel_support.x,
                scaleing * pixel_support.y,
                object_distance);
}

struct mliVec mliAptCam_ray_support_on_aperture(
        const double aperture_radius,
        struct mliPrng *prng)
{
        /* use a perfect disc as aperture */
        return mli_random_position_on_disc(aperture_radius, prng);
}

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

double mliAptCam_focal_length_given_field_of_view_and_sensor_width(
        const double field_of_view,
        const double image_sensor_width)
{
        const double image_sensor_radius = 0.5 * image_sensor_width;
        const double fov_opening_angle = 0.5 * field_of_view;
        return image_sensor_radius / tan(fov_opening_angle);
}

struct mliRay mliAptCam_get_ray_for_pixel(
        const double focal_length,
        const double aperture_radius,
        const double image_sensor_distance,
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliPrng *prng)
{
        struct mliVec direction;
        struct mliVec aperture_support =
                mliAptCam_ray_support_on_aperture(aperture_radius, prng);

        struct mliVec image_sensor_support =
                (mliAptCam_draw_random_support_on_image_sensor_plane(
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
                direction = mliVec_multiply(image_sensor_support, -1.0);
        } else {
                struct mliVec object_point = mliAptCam_get_object_point(
                        focal_length, image_sensor_support);

                direction = mliVec_substract(object_point, aperture_support);
        }

        return mliRay_set(aperture_support, direction);
}

void mliAptCam_aquire_pixels(
        const struct mliAptCam camera,
        const struct mliImage *image,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        const struct mliPixels *pixels_to_do,
        struct mliImage *colors,
        const struct mliTracerCongig *tracer_config,
        struct mliPrng *prng)
{
        uint64_t i;
        struct mliHomTra camera2root = mliHomTra_from_compact(camera2root_comp);
        for (i = 0; i < pixels_to_do->num_pixels_to_do; i++) {
                struct mliRay ray_wrt_camera = mliAptCam_get_ray_for_pixel(
                        camera.focal_length,
                        camera.aperture_radius,
                        camera.image_sensor_distance,
                        camera.image_sensor_width_x,
                        camera.image_sensor_width_y,
                        image->num_cols,
                        image->num_rows,
                        pixels_to_do->pixels[i].col,
                        pixels_to_do->pixels[i].row,
                        prng);

                struct mliRay ray_wrt_root =
                        mliHomTra_ray(&camera2root, ray_wrt_camera);

                struct mliColor set_color =
                        mli_trace(scenery, ray_wrt_root, tracer_config, prng);

                mliImage_set(colors, i, 0u, set_color);
        }

        return;
}

void mliAptCam_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mliPixels *pixels,
        const struct mliImage *colors,
        struct mliImage *sum_image,
        struct mliImage *exposure_image)
{
        uint64_t pix;
        for (pix = 0; pix < pixels->num_pixels_to_do; pix++) {
                const uint64_t idx = mliImage_idx(
                        sum_image,
                        pixels->pixels[pix].col,
                        pixels->pixels[pix].row);
                sum_image->raw[idx].r += colors->raw[pix].r;
                sum_image->raw[idx].g += colors->raw[pix].g;
                sum_image->raw[idx].b += colors->raw[pix].b;

                exposure_image->raw[idx].r += 1.;
                exposure_image->raw[idx].g += 1.;
                exposure_image->raw[idx].b += 1.;
        }
}

int mliAptCam_render_image(
        const struct mliAptCam camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        struct mliImage *image,
        const struct mliTracerCongig *tracer_config,
        struct mliPrng *prng)
{
        float noise_threshold = 0.05 * 255.0;
        uint64_t MAX_ITERATIONS = 128;
        uint64_t iteration = 0;

        struct mliColor zero_color = mliColor_set(0.0, 0.0, 0.0);
        struct mliImage sum_image = mliImage_init();
        struct mliImage exposure_image = mliImage_init();
        struct mliImage to_do_image = mliImage_init();
        struct mliImage sobel_image = mliImage_init();
        struct mliImage previous_sobel_image = mliImage_init();
        struct mliImage diff_image = mliImage_init();
        struct mliImage colors = mliImage_init();
        struct mliPixels pixels_to_do = mliPixels_init();

        chk_msg(mliImage_malloc(&sum_image, image->num_cols, image->num_rows),
                "Failed to malloc sum_image.");
        chk_msg(mliImage_malloc(
                        &exposure_image, image->num_cols, image->num_rows),
                "Failed to malloc exposure_image.");
        chk_msg(mliImage_malloc(&to_do_image, image->num_cols, image->num_rows),
                "Failed to malloc to_do_image.");
        chk_msg(mliImage_malloc(&sobel_image, image->num_cols, image->num_rows),
                "Failed to malloc sobel_image.");
        chk_msg(mliImage_malloc(
                        &previous_sobel_image,
                        image->num_cols,
                        image->num_rows),
                "Failed to malloc previous_sobel_image.");
        chk_msg(mliImage_malloc(&diff_image, image->num_cols, image->num_rows),
                "Failed to malloc diff_image.");
        chk_msg(mliImage_malloc(&colors, image->num_cols * image->num_rows, 1),
                "Failed to malloc colors.");

        mliImage_set_all_pixel(image, zero_color);
        mliImage_set_all_pixel(&sum_image, zero_color);
        mliImage_set_all_pixel(&exposure_image, zero_color);
        mliImage_set_all_pixel(&to_do_image, zero_color);
        mliImage_set_all_pixel(&sobel_image, zero_color);
        mliImage_set_all_pixel(&previous_sobel_image, zero_color);
        mliImage_set_all_pixel(&colors, zero_color);

        chk_msg(mliPixels_malloc(
                        &pixels_to_do, image->num_cols * image->num_rows),
                "Failed to malloc pixels_to_do.");

        /*
        initial image
        =============
        */
        mliPixels_set_all_from_image(&pixels_to_do, image);
        pixels_to_do.num_pixels_to_do = image->num_cols * image->num_rows;

        mliAptCam_aquire_pixels(
                camera,
                image,
                camera2root_comp,
                scenery,
                &pixels_to_do,
                &colors,
                tracer_config,
                prng);

        mliAptCam_assign_pixel_colors_to_sum_and_exposure_image(
                &pixels_to_do, &colors, &sum_image, &exposure_image);

        mliImage_divide_pixelwise(
                &sum_image, &exposure_image, image);

        mliImage_sobel(image, &sobel_image);

        mliImage_luminance_threshold_dilatation(
                &sobel_image, 128.0, &to_do_image);

        printf("\n");
        while (1) {
                if (iteration >= MAX_ITERATIONS)
                        break;

                mliPixels_above_threshold(&to_do_image, 0.5, &pixels_to_do);

                if (pixels_to_do.num_pixels_to_do <
                    image->num_rows * image->num_cols / 100.0)
                        break;

                printf("loop %3u / %3u, %d,%03d pixel left\n",
                       (uint32_t)iteration + 1,
                       (uint32_t)MAX_ITERATIONS,
                       pixels_to_do.num_pixels_to_do / 1000,
                       pixels_to_do.num_pixels_to_do % 1000);
                mliAptCam_aquire_pixels(
                        camera,
                        image,
                        camera2root_comp,
                        scenery,
                        &pixels_to_do,
                        &colors,
                        tracer_config,
                        prng);

                mliAptCam_assign_pixel_colors_to_sum_and_exposure_image(
                        &pixels_to_do, &colors, &sum_image, &exposure_image);

                mliImage_divide_pixelwise(
                        &sum_image, &exposure_image, image);

                mliImage_copy(&sobel_image, &previous_sobel_image);

                mliImage_sobel(image, &sobel_image);
                mliImage_fabs_difference(
                        &previous_sobel_image, &sobel_image, &diff_image);

                mliImage_set_all_pixel(&to_do_image, zero_color);

                mliImage_luminance_threshold_dilatation(
                        &diff_image, noise_threshold, &to_do_image);

                iteration += 1;
        }

        mliImage_free(&sum_image);
        mliImage_free(&exposure_image);
        mliImage_free(&to_do_image);
        mliImage_free(&sobel_image);
        mliImage_free(&previous_sobel_image);
        mliImage_free(&diff_image);
        mliImage_free(&colors);
        mliPixels_free(&pixels_to_do);
        return 1;
error:
        mliImage_free(&sum_image);
        mliImage_free(&exposure_image);
        mliImage_free(&to_do_image);
        mliImage_free(&sobel_image);
        mliImage_free(&previous_sobel_image);
        mliImage_free(&diff_image);
        mliImage_free(&colors);
        mliPixels_free(&pixels_to_do);
        return 0;
}
