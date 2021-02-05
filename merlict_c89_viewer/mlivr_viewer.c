/* Copyright 2019 Sebastian Achim Mueller                                     */
#include "mlivr_viewer.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "mlivr_toggle_stdin.h"

void mlivr_clear_screen(void)
{
        uint64_t n = 10;
        while (n) {
                putchar('\n');
                n--;
        }
}

void mlivr_print_help(void)
{
        mlivr_clear_screen();
        printf("merlict-c89\n");
        printf("===========\n");
        printf("\n");
        printf("- this help           [  h  ]\n");
        printf("- exit                [ Esc ]\n");
        printf("\n");
        printf("Look\n");
        printf("----\n");
        printf("- up                  [  i  ]\n");
        printf("- down                [  k  ]\n");
        printf("- left                [  j  ]\n");
        printf("- right               [  l  ]\n");
        printf("\n");
        printf("Move\n");
        printf("----\n");
        printf("- forward             [  w  ]\n");
        printf("- backward            [  s  ]\n");
        printf("- left                [  a  ]\n");
        printf("- right               [  d  ]\n");
        printf("- up                  [  q  ]\n");
        printf("- down                [  e  ]\n");
        printf("\n");
        printf("Field-of-view\n");
        printf("-------------\n");
        printf("- increace            [  n  ]\n");
        printf("- decreace            [  m  ]\n");
        printf("\n");
        printf("Quality\n");
        printf("-------\n");
        printf("- super sampling      [  b  ]\n");
        printf("- color/monochrome    [  g  ]\n");
        printf("\n");
        printf("Take picture\n");
        printf("------------\n");
        printf("- focus-finder        [  c  ]\n");
        printf("- take picture        [Space]\n");
        printf("\n");
        printf("Version\n");
        printf("-------\n");
        printf("- mli   %d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);
        printf("- mlivr %d.%d.%d\n",
               MLIVR_VERSION_MAYOR,
               MLIVR_VERSION_MINOR,
               MLIVR_VERSION_PATCH);
        printf("\n");
        printf("Copyright Sebastian Achim Mueller\n");
}

void mlivr_print_info_line(
        const struct mliView view,
        const struct mlivrCursor cursor)
{
        printf("Press 'h' for help. "
               "Camera: "
               "pos [ % -.2e, % -.2e, % -.2e]m, "
               "rot [ % -.1f, % -.1f, % -.1f]deg, "
               "fov %.2fdeg",
               view.position.x,
               view.position.y,
               view.position.z,
               mli_rad2deg(view.rotation.x),
               mli_rad2deg(view.rotation.y),
               mli_rad2deg(view.rotation.z),
               mli_rad2deg(view.field_of_view));
        if (cursor.active) {
                printf(", cursor [% 3ld, % 3ld]pix", cursor.col, cursor.row);
        }
        printf(".\n");
}

void mlivr_timestamp_now_19chars(char *buffer)
{
        time_t now = time(0);
        struct tm *nowtm;
        nowtm = localtime(&now);
        sprintf(buffer,
                "%04u-%02u-%02u_%02u-%02u-%02u",
                nowtm->tm_year + 1900,
                nowtm->tm_mon + 1,
                nowtm->tm_mday,
                nowtm->tm_hour,
                nowtm->tm_min,
                nowtm->tm_sec);
}

int mlivr_truncate_8bit(const int key)
{
        if (key == -1)
                return key;
        else
                return key & 255;
}

int _mlivr_export_image(
        const struct mliCombine *combine,
        const struct mlivrConfig config,
        const struct mliView view,
        const double object_distance,
        const char *path)
{
        struct mliMT19937 prng = mliMT19937_init(config.random_seed);
        struct mliImage full = mliImage_init();
        struct mliHomTraComp camera2root_comp;
        struct mliApertureCamera apcam;

        const double image_ratio =
                ((double)config.export_num_cols /
                 (double)config.export_num_rows);
        mli_check_mem(mliImage_malloc(
                &full, config.export_num_cols, config.export_num_rows));
        camera2root_comp = mliView_to_HomTraComp(view);
        apcam.focal_length =
                mliApCam_focal_length_given_field_of_view_and_sensor_width(
                        view.field_of_view,
                        config.aperture_camera_image_sensor_width);
        apcam.aperture_radius = 0.5 * (apcam.focal_length /
                                       config.aperture_camera_f_stop_ratio);
        apcam.image_sensor_distance = mli_image_given_focal_and_object(
                apcam.focal_length, object_distance);
        apcam.image_sensor_width_x = config.aperture_camera_image_sensor_width;
        apcam.image_sensor_width_y = apcam.image_sensor_width_x / image_ratio;
        mliApertureCamera_render_image(
                &prng, apcam, camera2root_comp, combine, &full);
        mli_check(mliImage_write_to_ppm(&full, path), "Failed to write ppm.");
        mliImage_free(&full);
        return 1;
error:
        return 0;
}

int mlivr_run_interactive_viewer(
        const struct mliCombine *combine,
        const struct mlivrConfig config)
{
        struct termios old_terminal = mlivr_disable_stdin_buffer();

        char path[1024];
        int key;
        int super_resolution = 0;
        struct mlivrCursor cursor;
        uint64_t num_screenshots = 0;
        uint64_t print_mode = MLI_ASCII_MONOCHROME;
        char timestamp[20];
        struct mliView view = config.view;
        struct mliImage img = mliImage_init();
        struct mliImage img2 = mliImage_init();
        const double row_over_column_pixel_ratio = 2.0;
        int update_image = 1;
        int print_help = 0;
        int has_probing_intersection;
        struct mliIntersection probing_intersection;

        mlivr_timestamp_now_19chars(timestamp);
        mli_check_mem(mliImage_malloc(
                &img, config.preview_num_cols, config.preview_num_rows));
        mli_check_mem(mliImage_malloc(
                &img2,
                config.preview_num_cols * 2u,
                config.preview_num_rows * 2u));

        cursor.active = 0;
        cursor.col = config.preview_num_cols / 2;
        cursor.row = config.preview_num_rows / 2;
        cursor.num_cols = config.preview_num_cols;
        cursor.num_rows = config.preview_num_rows;
        goto show_image;

        while ((key = mlivr_truncate_8bit(getchar())) != MLIVR_ESCAPE_KEY) {
                update_image = 1;
                print_help = 0;
                if (cursor.active) {
                        update_image = 0;
                        super_resolution = 0;
                        switch (key) {
                        case 'i':
                                mlivrCursor_move_up(&cursor);
                                break;
                        case 'k':
                                mlivrCursor_move_down(&cursor);
                                break;
                        case 'l':
                                mlivrCursor_move_left(&cursor);
                                break;
                        case 'j':
                                mlivrCursor_move_right(&cursor);
                                break;
                        case 'c':
                                cursor.active = !cursor.active;
                                break;
                        case 'h':
                                print_help = 1;
                                break;
                        case MLIVR_SPACE_KEY:
                                sprintf(path,
                                        "%s_%06lu.ppm",
                                        timestamp,
                                        num_screenshots);
                                num_screenshots++;
                                mli_c(_mlivr_export_image(
                                        combine,
                                        config,
                                        view,
                                        probing_intersection.distance_of_ray,
                                        path));
                                update_image = 0;
                                break;
                        default:
                                printf("Key Press unknown: %d\n", key);
                                break;
                        }
                } else {
                        switch (key) {
                        case 'w':
                                view = mliView_move_forward(
                                        view, config.step_length);
                                break;
                        case 's':
                                view = mliView_move_forward(
                                        view, -config.step_length);
                                break;
                        case 'a':
                                view = mliView_move_right(
                                        view, -config.step_length);
                                break;
                        case 'd':
                                view = mliView_move_right(
                                        view, config.step_length);
                                break;
                        case 'q':
                                view = mliView_move_up(
                                        view, config.step_length);
                                break;
                        case 'e':
                                view = mliView_move_up(
                                        view, -config.step_length);
                                break;
                        case 'i':
                                view = mliView_look_up_when_possible(view, .05);
                                break;
                        case 'k':
                                view = mliView_look_down_when_possible(
                                        view, .05);
                                break;
                        case 'l':
                                view = mliView_look_right(view, -.05);
                                break;
                        case 'j':
                                view = mliView_look_right(view, .05);
                                break;
                        case 'n':
                                view = mliView_decrease_fov(view, 1.05);
                                break;
                        case 'm':
                                view = mliView_increase_fov(view, 1.05);
                                break;
                        case 'h':
                                print_help = 1;
                                update_image = 0;
                                break;
                        case 'b':
                                super_resolution = !super_resolution;
                                break;
                        case 'c':
                                cursor.active = !cursor.active;
                                update_image = 0;
                                break;
                        case MLIVR_SPACE_KEY:
                                printf("Go into cursor-mode first.\n");
                                break;
                        case 'g':
                                if (print_mode == MLI_ASCII_MONOCHROME) {
                                        print_mode = MLI_ANSI_ESCAPE_COLOR;
                                } else if (
                                        print_mode == MLI_ANSI_ESCAPE_COLOR) {
                                        print_mode = MLI_ASCII_MONOCHROME;
                                } else {
                                        print_mode = MLI_ASCII_MONOCHROME;
                                }
                                break;
                        default:
                                printf("Key Press unknown: %d\n", key);
                                update_image = 0;
                                break;
                        }
                }
        show_image:
                if (update_image) {
                        if (super_resolution) {
                                mliPinHoleCamera_render_image_with_view(
                                        view,
                                        combine,
                                        &img2,
                                        row_over_column_pixel_ratio);
                                mliImage_scale_down_twice(&img2, &img);
                        } else {
                                mliPinHoleCamera_render_image_with_view(
                                        view,
                                        combine,
                                        &img,
                                        row_over_column_pixel_ratio);
                        }
                }
                mlivr_clear_screen();
                if (cursor.active) {
                        char symbols[1];
                        uint64_t rows[1];
                        uint64_t cols[1];
                        const uint64_t num_symbols = 1u;
                        symbols[0] = 'X';
                        rows[0] = cursor.row;
                        cols[0] = cursor.col;
                        mliImage_print_chars(
                                &img,
                                symbols,
                                rows,
                                cols,
                                num_symbols,
                                print_mode);
                        {
                                struct mliPinHoleCamera pin_hole_camera =
                                        mliPinHoleCamera_init(
                                                view.field_of_view,
                                                &img,
                                                row_over_column_pixel_ratio);

                                struct mliHomTraComp camera2root_comp =
                                        mliView_to_HomTraComp(view);
                                struct mliHomTra camera2root =
                                        mliHomTra_from_compact(
                                                camera2root_comp);
                                struct mliRay probing_ray_wrt_camera;
                                struct mliRay probing_ray_wrt_root;

                                probing_ray_wrt_camera =
                                        mliPinHoleCamera_ray_at_row_col(
                                                &pin_hole_camera,
                                                &img,
                                                cursor.row,
                                                cursor.col);
                                probing_ray_wrt_root = mliHomTra_ray(
                                        &camera2root, probing_ray_wrt_camera);

                                probing_intersection = mliIntersection_init();

                                has_probing_intersection =
                                        mli_first_casual_intersection(
                                                combine,
                                                probing_ray_wrt_root,
                                                &probing_intersection);
                        }
                } else {
                        mliImage_print(&img, print_mode);
                }
                mlivr_print_info_line(view, cursor);
                if (cursor.active) {
                        printf("Intersection: ");
                        if (has_probing_intersection) {

                                printf("(% 5d;% 5d,% 5d,% 5d)"
                                       "/"
                                       "(id;robj,obj,face), "
                                       "dist % 6.2fm, "
                                       "pos [% -.2e, % -.2e, % -.2e], "
                                       "normal [% -.3f, % -.3f, % -.3f], ",
                                       combine->scenery->robject_ids
                                               [probing_intersection
                                                        .geometry_id.robj],
                                       probing_intersection.geometry_id.robj,
                                       combine->scenery
                                               ->robjects[probing_intersection
                                                                  .geometry_id
                                                                  .robj],
                                       probing_intersection.geometry_id.face,
                                       probing_intersection.distance_of_ray,
                                       probing_intersection.position.x,
                                       probing_intersection.position.y,
                                       probing_intersection.position.z,
                                       probing_intersection.surface_normal.x,
                                       probing_intersection.surface_normal.y,
                                       probing_intersection.surface_normal.z);

                                if (probing_intersection
                                            .from_outside_to_inside) {
                                        printf("outside");
                                } else {
                                        printf(" inside");
                                }
                                printf("\n");
                        } else {
                                printf("None\n");
                        }
                } else {
                        printf("\n");
                }
                if (print_help) {
                        mlivr_print_help();
                }
        }

        mliImage_free(&img);
        mliImage_free(&img2);
        mlivr_restore_stdin_buffer(&old_terminal);
        return 1;
error:
        mliImage_free(&img);
        mliImage_free(&img2);
        mlivr_restore_stdin_buffer(&old_terminal);
        return 0;
}
