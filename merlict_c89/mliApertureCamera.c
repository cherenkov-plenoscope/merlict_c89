#include "mliApertureCamera.h"

struct mliVec mliApCam_get_pixel_center_on_image_sensor_plane(
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

struct mliVec mliApCam_get_random_pixel_support_on_image_sensor_plane(
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const double image_sensor_distance,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliMT19937 *prng)
{
        double pixel_bin_width_x = image_sensor_width_x / (double)num_pixel_x;
        double pixel_bin_width_y = image_sensor_width_y / (double)num_pixel_y;
        struct mliVec support = mliApCam_get_pixel_center_on_image_sensor_plane(
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

struct mliVec mliApCam_get_object_point(
        const double focal_length,
        const struct mliVec pixel_support)
{
        const double object_distance = mli_object_given_focal_and_image(
                focal_length, -1.0 * pixel_support.z);
        const double scaleing = object_distance / pixel_support.z;
        return mliVec_set(
                scaleing * pixel_support.x,
                scaleing * pixel_support.y,
                object_distance);
}

struct mliVec mliApCam_ray_support_on_aperture(
        const double aperture_radius,
        struct mliMT19937 *prng)
{
        /* use a perfect disc as aperture */
        return mli_random_position_on_disc(aperture_radius, prng);
}

double mli_object_given_focal_and_image(
        const double focal_length,
        const double image_distance)
{
        /* 1/f = 1/g + 1/b */
        /* 1/g = 1/f - 1/b */
        /* g = 1/(1/f - 1/b) */
        return 1.0 / (1.0 / focal_length - 1.0 / image_distance);
}

double mli_image_given_focal_and_object(
        const double focal_length,
        const double object_distance)
{
        /* 1/f = 1/g + 1/b */
        /* 1/b = 1/f - 1/g */
        /* b = 1/(1/f - 1/g) */
        return 1.0 / (1.0 / focal_length - 1.0 / object_distance);
}

double mliApCam_focal_length_given_field_of_view_and_sensor_width(
        const double field_of_view,
        const double image_sensor_width)
{
        const double image_sensor_radius = 0.5 * image_sensor_width;
        const double fov_opening_angle = 0.5 * field_of_view;
        return image_sensor_radius / tan(fov_opening_angle);
}

struct mliRay mliApCam_get_ray_for_pixel(
        const double focal_length,
        const double aperture_radius,
        const double image_sensor_distance,
        const double image_sensor_width_x,
        const double image_sensor_width_y,
        const uint64_t num_pixel_x,
        const uint64_t num_pixel_y,
        const uint64_t pixel_x,
        const uint64_t pixel_y,
        struct mliMT19937 *prng)
{
        struct mliVec direction;
        struct mliVec aperture_support =
                mliApCam_ray_support_on_aperture(aperture_radius, prng);

        struct mliVec image_sensor_support =
                (mliApCam_get_random_pixel_support_on_image_sensor_plane(
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
                struct mliVec object_point = mliApCam_get_object_point(
                        focal_length, image_sensor_support);

                direction = mliVec_substract(object_point, aperture_support);
        }

        return mliRay_set(aperture_support, direction);
}

void _mliApCam_aquire_pixels(
        struct mliMT19937 *prng,
        const struct mliApertureCamera camera,
        const struct mliImage *image,
        const struct mliHomTraComp camera2root_comp,
        const struct mliCombine *combine,
        const struct mliPixels *pixels_to_do,
        struct mliImage *colors)
{
        uint64_t i;
        struct mliHomTra camera2root = mliHomTra_from_compact(camera2root_comp);
        for (i = 0; i < pixels_to_do->num_pixels_to_do; i++) {
                struct mliRay ray_wrt_camera = mliApCam_get_ray_for_pixel(
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
                        mli_trace(combine, ray_wrt_root);

                mliImage_set(colors, i, 0u, set_color);
        }

        return;
}

void _mliApCam_assign_pixel_colors_to_sum_and_exposure_image(
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

void _mliApCam_image_from_sum_and_exposure(
        const struct mliImage *sum_image,
        const struct mliImage *exposure_image,
        struct mliImage *out)
{
        uint64_t pix;
        for (pix = 0; pix < out->num_rows * out->num_cols; pix++) {
                out->raw[pix].r =
                        sum_image->raw[pix].r / exposure_image->raw[pix].r;
                out->raw[pix].g =
                        sum_image->raw[pix].g / exposure_image->raw[pix].g;
                out->raw[pix].b =
                        sum_image->raw[pix].b / exposure_image->raw[pix].b;
        }
}

int mliApertureCamera_render_image(
        struct mliMT19937 *prng,
        const struct mliApertureCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliCombine *combine,
        struct mliImage *image)
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

        mli_check(
                mliImage_malloc(&sum_image, image->num_cols, image->num_rows),
                "Failed to malloc sum_image.");
        mli_check(
                mliImage_malloc(
                        &exposure_image, image->num_cols, image->num_rows),
                "Failed to malloc exposure_image.");
        mli_check(
                mliImage_malloc(&to_do_image, image->num_cols, image->num_rows),
                "Failed to malloc to_do_image.");
        mli_check(
                mliImage_malloc(&sobel_image, image->num_cols, image->num_rows),
                "Failed to malloc sobel_image.");
        mli_check(
                mliImage_malloc(
                        &previous_sobel_image,
                        image->num_cols,
                        image->num_rows),
                "Failed to malloc previous_sobel_image.");
        mli_check(
                mliImage_malloc(&diff_image, image->num_cols, image->num_rows),
                "Failed to malloc diff_image.");
        mli_check(
                mliImage_malloc(&colors, image->num_cols * image->num_rows, 1),
                "Failed to malloc colors.");

        mliImage_set_all_pixel(image, zero_color);
        mliImage_set_all_pixel(&sum_image, zero_color);
        mliImage_set_all_pixel(&exposure_image, zero_color);
        mliImage_set_all_pixel(&to_do_image, zero_color);
        mliImage_set_all_pixel(&sobel_image, zero_color);
        mliImage_set_all_pixel(&previous_sobel_image, zero_color);
        mliImage_set_all_pixel(&colors, zero_color);

        mli_check(
                mliPixels_malloc(
                        &pixels_to_do, image->num_cols * image->num_rows),
                "Failed to malloc pixels_to_do.");

        /*
        initial image
        =============
        */
        mliPixels_set_all_from_image(&pixels_to_do, image);
        pixels_to_do.num_pixels_to_do = image->num_cols * image->num_rows;

        _mliApCam_aquire_pixels(
                prng,
                camera,
                image,
                camera2root_comp,
                combine,
                &pixels_to_do,
                &colors);

        _mliApCam_assign_pixel_colors_to_sum_and_exposure_image(
                &pixels_to_do, &colors, &sum_image, &exposure_image);

        _mliApCam_image_from_sum_and_exposure(
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

                printf("loop %3ld / %3ld, %d,%03d pixel left\n",
                       iteration + 1,
                       MAX_ITERATIONS,
                       pixels_to_do.num_pixels_to_do / 1000,
                       pixels_to_do.num_pixels_to_do % 1000);
                _mliApCam_aquire_pixels(
                        prng,
                        camera,
                        image,
                        camera2root_comp,
                        combine,
                        &pixels_to_do,
                        &colors);

                _mliApCam_assign_pixel_colors_to_sum_and_exposure_image(
                        &pixels_to_do, &colors, &sum_image, &exposure_image);

                _mliApCam_image_from_sum_and_exposure(
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
