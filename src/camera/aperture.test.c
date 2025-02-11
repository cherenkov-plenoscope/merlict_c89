/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("pixel-center on image-sensor-plane")
{
        struct mli_Vec c;
        const double image_sensor_width_x = 16.0;
        const double image_sensor_width_y = 9.0;
        const double image_sensor_distance = 1.337;
        const uint64_t num_pixel_x = 32;
        const uint64_t num_pixel_y = 18;
        uint64_t pixel_x = 0;
        uint64_t pixel_y = 0;

        double const pixel_width_x = image_sensor_width_x / (double)num_pixel_x;
        double const pixel_width_y = image_sensor_width_y / (double)num_pixel_y;
        c = mli_camera_Aperture_pixel_center_on_image_sensor_plane(
                image_sensor_width_x,
                image_sensor_width_y,
                image_sensor_distance,
                num_pixel_x,
                num_pixel_y,
                pixel_x,
                pixel_y);
        CHECK_MARGIN(
                c.x, +0.5 * image_sensor_width_x - 0.5 * pixel_width_x, 1e-6);
        CHECK_MARGIN(
                c.y, +0.5 * image_sensor_width_y - 0.5 * pixel_width_y, 1e-6);
        CHECK_MARGIN(c.z, -image_sensor_distance, 1e-6);

        pixel_x = 6;
        pixel_y = 3;
        c = mli_camera_Aperture_pixel_center_on_image_sensor_plane(
                image_sensor_width_x,
                image_sensor_width_y,
                image_sensor_distance,
                num_pixel_x,
                num_pixel_y,
                pixel_x,
                pixel_y);
        CHECK_MARGIN(
                c.x,
                (0.5 * image_sensor_width_x -
                 (0.5 + (double)pixel_x) * pixel_width_x),
                1e-6);
        CHECK_MARGIN(
                c.y,
                (0.5 * image_sensor_width_y -
                 (0.5 + (double)pixel_y) * pixel_width_y),
                1e-6);
        CHECK_MARGIN(c.z, -image_sensor_distance, 1e-6);
}

CASE("object-point")
{
        double focal_length = 1.0;
        double image_sensor_distance = -1.1;
        double object_distance = mli_thin_lens_get_object_given_focal_and_image(
                focal_length, fabs(image_sensor_distance));
        struct mli_Vec obj = mli_camera_Aperture_get_object_point(
                focal_length, mli_Vec_set(0.1, 0.1, image_sensor_distance));
        CHECK_MARGIN(object_distance, 11.0, 1e-6);
        CHECK_MARGIN(obj.x, -1.0, 1e-6);
        CHECK_MARGIN(obj.y, -1.0, 1e-6);
        CHECK_MARGIN(obj.z, object_distance, 1e-6);
}
