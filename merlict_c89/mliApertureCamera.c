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
        pixel_center.x = -1.0*mli_bin_center_in_linear_space(
                -0.5*image_sensor_width_x,
                +0.5*image_sensor_width_x,
                num_pixel_x,
                pixel_x);
        pixel_center.y = -1.0*mli_bin_center_in_linear_space(
                -0.5*image_sensor_width_y,
                +0.5*image_sensor_width_y,
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
        double pixel_bin_width_x = image_sensor_width_x/(double)num_pixel_x;
        double pixel_bin_width_y = image_sensor_width_y/(double)num_pixel_y;
        struct mliVec support = mliApCam_get_pixel_center_on_image_sensor_plane(
                image_sensor_width_x,
                image_sensor_width_y,
                image_sensor_distance,
                num_pixel_x,
                num_pixel_y,
                pixel_x,
                pixel_y);
        double rnd_x = (
                mli_random_uniform(prng)*pixel_bin_width_x -
                0.5*pixel_bin_width_x
        );
        double rnd_y = (
                mli_random_uniform(prng)*pixel_bin_width_y -
                0.5*pixel_bin_width_y
        );
        support.x = support.x + rnd_x;
        support.y = support.y + rnd_y;
        return support;
}

struct mliVec mliApCam_get_object_point(
        const double focal_length,
        const struct mliVec pixel_support)
{
        const double object_distance = mli_object_given_focal_and_image(
                        focal_length,
                        -1.0*pixel_support.z);
        const double scaleing = object_distance/pixel_support.z;
        return mliVec_set(
                scaleing*pixel_support.x,
                scaleing*pixel_support.y,
                object_distance
        );
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
        return 1.0/(1.0/focal_length - 1.0/image_distance);
}

double mli_image_given_focal_and_object(
        const double focal_length,
        const double object_distance)
{
        /* 1/f = 1/g + 1/b */
        /* 1/b = 1/f - 1/g */
        /* b = 1/(1/f - 1/g) */
        return 1.0/(1.0/focal_length - 1.0/object_distance);
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
        struct mliVec aperture_support = mliApCam_ray_support_on_aperture(
                aperture_radius,
                prng);

        struct mliVec image_sensor_support = (
                mliApCam_get_random_pixel_support_on_image_sensor_plane(
                        image_sensor_width_x,
                        image_sensor_width_y,
                        image_sensor_distance,
                        num_pixel_x,
                        num_pixel_y,
                        pixel_x,
                        pixel_y,
                        prng)
        );

        if (fabs(1.0 - focal_length/image_sensor_distance) < 1e-6) {
                /* focus set to infinity */
                direction = mliVec_multiply(image_sensor_support, -1.0);
        } else {
                struct mliVec object_point = mliApCam_get_object_point(
                        focal_length,
                        image_sensor_support);

                direction = mliVec_substract(
                        object_point,
                        aperture_support);
        }

        return mliRay_set(aperture_support, direction);
}

void mliApertureCamera_render_image(
        struct mliMT19937 *prng,
        const struct mliApertureCamera camera,
        const struct mliHomTraComp camera2root_comp,
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        struct mliImage *image)
{
        struct mliHomTra camera2root = mliHomTra_from_compact(camera2root_comp);
        const uint64_t num_rays_per_pixel = 10;
        const double w = 1.0/(double)num_rays_per_pixel;

        uint64_t row_y, col_x, pi;

        mliImage_set_all_pixel(image, mliColor_set(0.0, 0.0, 0.0));

        for (row_y = 0; row_y < image->num_rows; row_y++) {
                fprintf(stderr, "row: %ld of %d\n", row_y+1, image->num_rows);
                for (col_x = 0; col_x < image->num_cols; col_x++) {
                        for (pi = 0; pi < num_rays_per_pixel; pi++) {

                                struct mliRay ray_wrt_camera = mliApCam_get_ray_for_pixel(
                                        camera.focal_length,
                                        camera.aperture_radius,
                                        camera.image_sensor_distance,
                                        camera.image_sensor_width_x,
                                        camera.image_sensor_width_y,
                                        image->num_cols,
                                        image->num_rows,
                                        col_x,
                                        row_y,
                                        prng);

                                struct mliRay ray_wrt_root = mliHomTra_ray(
                                        &camera2root,
                                        ray_wrt_camera);

                                struct mliColor additional_color = mli_trace(
                                        scenery,
                                        octree,
                                        ray_wrt_root);

                                struct mliColor old_color = mliImage_at(
                                        image,
                                        col_x,
                                        row_y);

                                struct mliColor set_color = mliColor_set(
                                        old_color.r + w*additional_color.r,
                                        old_color.g + w*additional_color.g,
                                        old_color.b + w*additional_color.b);

                                mliImage_set(
                                        image,
                                        col_x,
                                        row_y,
                                        set_color);
                        }
                }
        }
}
