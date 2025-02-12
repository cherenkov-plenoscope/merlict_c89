/* Copyright 2019 Sebastian Achim Mueller                                     */
#include "viewer.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "../math/math.h"
#include "../intersection/intersection_surface_normal.h"
#include "../thin_lens/thin_lens.h"
#include "../image/image.h"
#include "../image/image_print.h"
#include "../image/image_ppm.h"
#include "../camera/pinhole.h"
#include "../camera/aperture.h"
#include "../raytracing/ray_scenery_query.h"
#include "../version/version.h"
#include "../pathtracer/pathtracer_config.h"
#include "toggle_stdin.h"

void mli_viewer_clear_screen(void)
{
        uint64_t n = 20;
        while (n) {
                putchar('\n');
                n--;
        }
}

void mli_viewer_print_help(void)
{
        mli_viewer_clear_screen();
        mli_version_logo_fprint(stdout);
        printf("  v%d.%d.%d\n",
               MLI_VERSION_MAYOR,
               MLI_VERSION_MINOR,
               MLI_VERSION_PATCH);
        printf("\n");
        printf("  General                       Inspect\n");
        printf("    exit              [ esc ]     focus/cursor      [  c  ]\n");
        printf("    this help         [  h  ]     take picture      [space]\n");
        printf("    scenery-info      [  p  ]\n");
        printf("\n");
        printf("  Look                          Move\n");
        printf("    up                [  i  ]     forward           [  w  ]\n");
        printf("    down              [  k  ]     backward          [  s  ]\n");
        printf("    left              [  j  ]     left              [  a  ]\n");
        printf("    right             [  l  ]     right             [  d  ]\n");
        printf("                                  up                [  q  ]\n");
        printf("                                  down              [  e  ]\n");
        printf("\n");
        printf("  Field-of-view                 Quality\n");
        printf("    increase          [  m  ]     super sampling    [  b  ]\n");
        printf("    decrease          [  n  ]     color/monochrome  [  g  ]\n");
        printf("\n");
        printf("  Atmosphere                    Sun\n");
        printf("    on/off            [  0  ]     later daytime     [  9  ]\n");
        printf("    - altitude        [  4  ]     earlier daytime   [  8  ]\n");
        printf("    + altitude        [  5  ]     + latitude        [  7  ]\n");
        printf("                                  - latitude        [  6  ]\n");
        printf("  Colors                                                   \n");
        printf("    + gamma           [  x  ]                              \n");
        printf("    - gamma           [  z  ]                              \n");
        printf("    + gain            [  .  ]                              \n");
        printf("    - gain            [  ,  ]                              \n");
        printf("\n");
        mli_version_authors_and_affiliations_fprint(stdout);
}

void mli_viewer_print_info_line(
        const struct mli_View view,
        const struct mli_viewer_Cursor cursor,
        const struct mli_pathtracer_Config tracer_config,
        const double gamma,
        const double gain)
{
        printf("Help 'h', "
               "Cam: "
               "pos[% -.2e, % -.2e, % -.2e]m, "
               "rot[% -.1f, % -.1f, % -.1f]deg, "
               "fov %.2fdeg, ",
               view.position.x,
               view.position.y,
               view.position.z,
               mli_math_rad2deg(view.rotation.x),
               mli_math_rad2deg(view.rotation.y),
               mli_math_rad2deg(view.rotation.z),
               mli_math_rad2deg(view.field_of_view));
        printf("gamma %.2f, ", gamma);
        printf("gain %.2f, ", gain);
        printf("Sun: lat % 3.0fdeg, %02d:%02dh, alt % 3.1fkm",
               mli_math_rad2deg(tracer_config.atmosphere.sunLatitude),
               (int)(tracer_config.atmosphere.sunHourAngle),
               (int)(tracer_config.atmosphere.sunHourAngle * 60) % 60,
               tracer_config.atmosphere.altitude * 1e-3);
        if (cursor.active) {
                printf(", Cursor[%3ld, %3ld]pix", cursor.col, cursor.row);
        }
        printf(".\n");
}

void mli_viewer_timestamp_now_19chars(char *buffer)
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

mli_key_code mli_viewer_get_key(void)
{
        /* Waits for keystroke and returns ascii-code.
         */
        const mli_key_code key = getchar();
        if (key == EOF) {
                /* In case of EOF, return EOF */
                return key;
        } else {
                /* On some systems we have to truncate. I dont know why. */
                return key & 255;
        }
}

chk_rc mli_viewer_image_to_path(const struct mli_Image *img, const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO_open_file_cstr(&f, path, "w"),
                "Can't open path to write image.");
        chk_msg(mli_Image_to_io(img, &f, MLI_IMAGE_PPM_COLOR_DEPTH_16BIT),
                "Can't write image to file.");
        mli_IO_close(&f);
        return CHK_SUCCESS;
chk_error:
        mli_IO_close(&f);
        return CHK_FAIL;
}

chk_rc mli_viewer_export_image(
        const struct mli_PathTracer *tracer,
        const struct mli_viewer_Config config,
        const struct mli_View view,
        struct mli_Prng *prng,
        const double object_distance,
        const double gamma,
        const double gain,
        const char *path)
{
        struct mli_Image full = mli_Image_init();
        struct mli_HomTraComp camera2root_comp;
        struct mli_camera_Aperture apcam = mli_camera_Aperture_init();

        const double image_ratio =
                ((double)config.export_num_cols /
                 (double)config.export_num_rows);
        chk_mem(mli_Image_malloc(
                &full, config.export_num_cols, config.export_num_rows));
        camera2root_comp = mli_View_to_HomTraComp(view);
        apcam.focal_length =
                mli_camera_Aperture_focal_length_given_field_of_view_and_sensor_width(
                        view.field_of_view,
                        config.aperture_camera_image_sensor_width);
        apcam.aperture_radius = 0.5 * (apcam.focal_length /
                                       config.aperture_camera_f_stop_ratio);
        apcam.image_sensor_distance =
                mli_thin_lens_get_image_given_focal_and_object(
                        apcam.focal_length, object_distance);
        apcam.image_sensor_width_x = config.aperture_camera_image_sensor_width;
        apcam.image_sensor_width_y = apcam.image_sensor_width_x / image_ratio;
        mli_camera_Aperture_render_image(
                apcam, camera2root_comp, tracer, &full, prng);
        mli_Image_multiply(&full, mli_Color_set(gain, gain, gain));
        mli_Image_power(&full, mli_Color_set(gamma, gamma, gamma));
        chk_msg(mli_viewer_image_to_path(&full, path), "Failed to write ppm.");
        mli_Image_free(&full);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_viewer_run_interactive_viewer_try_non_canonical_stdin(
        const struct mli_Scenery *scenery,
        const struct mli_viewer_Config config)
{
#ifdef HAVE_TERMIOS_H
        struct termios old_terminal = mli_viewer_non_canonical_stdin();
#endif
        chk_rc rc = mli_viewer_run_interactive_viewer(scenery, config);

#ifdef HAVE_TERMIOS_H
        mli_viewer_restore_stdin(&old_terminal);
#endif
        return rc;
}

chk_rc mli_viewer_run_interactive_viewer(
        const struct mli_Scenery *scenery,
        const struct mli_viewer_Config config)
{
        struct mli_Prng prng = mli_Prng_init_MT19937(config.random_seed);
        struct mli_pathtracer_Config tracer_config =
                mli_pathtracer_Config_init();
        struct mli_PathTracer tracer = mli_pathtracer_init();
        struct mli_ColorMaterials color_materials = mli_ColorMaterials_init();
        char path[1024];
        mli_key_code key;
        mli_bool super_resolution = MLI_FALSE;
        struct mli_viewer_Cursor cursor;
        uint64_t num_screenshots = 0;
        uint64_t print_mode = MLI_IMAGE_PRINT_ASCII_MONOCHROME;
        char timestamp[20];
        struct mli_View view = config.view;
        struct mli_Image img = mli_Image_init();
        struct mli_Image img_gamma = mli_Image_init();
        struct mli_Image img2 = mli_Image_init();
        const double row_over_column_pixel_ratio = 2.0;
        mli_bool update_image = MLI_TRUE;
        mli_bool print_help = MLI_FALSE;
        mli_bool print_scenery_info = MLI_FALSE;
        mli_bool has_probing_intersection = MLI_FALSE;
        struct mli_IntersectionSurfaceNormal probing_intersection;
        double gamma = config.gamma;
        double gain = config.gain;

        chk_msg(mli_ColorMaterials_malloc_from_Materials(
                        &color_materials, &scenery->materials),
                "Can't malloc color materials from scenery materials.");

        tracer.scenery = scenery;
        tracer.config = &tracer_config;
        tracer.scenery_color_materials = &color_materials;

        mli_viewer_timestamp_now_19chars(timestamp);
        chk_mem(mli_Image_malloc(
                &img, config.preview_num_cols, config.preview_num_rows));
        chk_mem(mli_Image_malloc(
                &img2,
                config.preview_num_cols * 2u,
                config.preview_num_rows * 2u));

        cursor.active = 0;
        cursor.col = config.preview_num_cols / 2;
        cursor.row = config.preview_num_rows / 2;
        cursor.num_cols = config.preview_num_cols;
        cursor.num_rows = config.preview_num_rows;
        goto show_image;

        while ((key = mli_viewer_get_key()) != MLI_VIEWER_ESCAPE_KEY) {
                update_image = MLI_TRUE;
                print_help = MLI_FALSE;
                print_scenery_info = MLI_FALSE;
                if (cursor.active) {
                        update_image = MLI_FALSE;
                        super_resolution = MLI_FALSE;
                        switch (key) {
                        case 'i':
                                mli_viewer_Cursor_move_up(&cursor);
                                break;
                        case 'k':
                                mli_viewer_Cursor_move_down(&cursor);
                                break;
                        case 'l':
                                mli_viewer_Cursor_move_left(&cursor);
                                break;
                        case 'j':
                                mli_viewer_Cursor_move_right(&cursor);
                                break;
                        case 'c':
                                cursor.active = !cursor.active;
                                break;
                        case 'h':
                                print_help = MLI_TRUE;
                                break;
                        case MLI_VIEWER_SPACE_KEY:
                                sprintf(path,
                                        "%s_%06lu.ppm",
                                        timestamp,
                                        num_screenshots);
                                num_screenshots++;
                                chk(mli_viewer_export_image(
                                        &tracer,
                                        config,
                                        view,
                                        &prng,
                                        probing_intersection.distance_of_ray,
                                        gamma,
                                        gain,
                                        path));
                                update_image = MLI_FALSE;
                                break;
                        default:
                                printf("Key Press unknown: %d\n", key);
                                break;
                        }
                } else {
                        switch (key) {
                        case 'w':
                                view = mli_View_move_forward(
                                        view, config.step_length);
                                break;
                        case 's':
                                view = mli_View_move_forward(
                                        view, -config.step_length);
                                break;
                        case 'a':
                                view = mli_View_move_right(
                                        view, -config.step_length);
                                break;
                        case 'd':
                                view = mli_View_move_right(
                                        view, config.step_length);
                                break;
                        case 'q':
                                view = mli_View_move_up(
                                        view, config.step_length);
                                break;
                        case 'e':
                                view = mli_View_move_up(
                                        view, -config.step_length);
                                break;
                        case 'i':
                                view = mli_View_look_up_when_possible(
                                        view, .05);
                                break;
                        case 'k':
                                view = mli_View_look_down_when_possible(
                                        view, .05);
                                break;
                        case 'l':
                                view = mli_View_look_right(view, -.05);
                                break;
                        case 'j':
                                view = mli_View_look_right(view, .05);
                                break;
                        case 'n':
                                view = mli_View_decrease_fov(view, 1.05);
                                break;
                        case 'm':
                                view = mli_View_increase_fov(view, 1.05);
                                break;
                        case 'h':
                                print_help = MLI_TRUE;
                                update_image = MLI_FALSE;
                                break;
                        case 'b':
                                super_resolution = !super_resolution;
                                break;
                        case 'c':
                                cursor.active = !cursor.active;
                                update_image = MLI_FALSE;
                                break;
                        case MLI_VIEWER_SPACE_KEY:
                                printf("Go into cursor-mode first.\n");
                                break;
                        case 'g':
                                if (print_mode ==
                                    MLI_IMAGE_PRINT_ASCII_MONOCHROME) {
                                        print_mode =
                                                MLI_IMAGE_PRINT_ASCII_ESCAPE_COLOR;
                                } else if (
                                        print_mode ==
                                        MLI_IMAGE_PRINT_ASCII_ESCAPE_COLOR) {
                                        print_mode =
                                                MLI_IMAGE_PRINT_ASCII_MONOCHROME;
                                } else {
                                        print_mode =
                                                MLI_IMAGE_PRINT_ASCII_MONOCHROME;
                                }
                                break;
                        case 'p':
                                print_scenery_info = MLI_TRUE;
                                update_image = MLI_FALSE;
                                break;

                        case '4':
                                mli_Atmosphere_decrease_altitude(
                                        &tracer_config.atmosphere, 0.9);
                                break;
                        case '5':
                                mli_Atmosphere_increase_altitude(
                                        &tracer_config.atmosphere, 1.1);
                                break;
                        case '6':
                                mli_Atmosphere_decrease_latitude(
                                        &tracer_config.atmosphere,
                                        mli_math_deg2rad(2.0));
                                break;
                        case '7':
                                mli_Atmosphere_increase_latitude(
                                        &tracer_config.atmosphere,
                                        mli_math_deg2rad(2.0));
                                break;
                        case '8':
                                mli_Atmosphere_decrease_hours(
                                        &tracer_config.atmosphere, 0.1);
                                break;
                        case '9':
                                mli_Atmosphere_increase_hours(
                                        &tracer_config.atmosphere, 0.1);
                                break;
                        case '0':
                                tracer_config.have_atmosphere =
                                        !tracer_config.have_atmosphere;
                                break;
                        case 'x':
                                gamma *= 1.05;
                                break;
                        case 'z':
                                gamma *= 0.95;
                                break;
                        case '.':
                                gain *= 1.05;
                                break;
                        case ',':
                                gain *= 0.95;
                                break;
                        default:
                                printf("Key Press unknown: %d\n", key);
                                update_image = MLI_FALSE;
                                break;
                        }
                }
        show_image:
                if (update_image) {
                        if (super_resolution) {
                                mli_camera_PinHole_render_image_with_view(
                                        view,
                                        &tracer,
                                        &img2,
                                        row_over_column_pixel_ratio,
                                        &prng);
                                mli_Image_scale_down_twice(&img2, &img);
                        } else {
                                mli_camera_PinHole_render_image_with_view(
                                        view,
                                        &tracer,
                                        &img,
                                        row_over_column_pixel_ratio,
                                        &prng);
                        }
                        chk(mli_Image_copy(&img, &img_gamma));
                        mli_Image_multiply(
                                &img_gamma, mli_Color_set(gain, gain, gain));
                        mli_Image_power(
                                &img_gamma, mli_Color_set(gamma, gamma, gamma));
                }
                mli_viewer_clear_screen();
                if (cursor.active) {
                        char symbols[1];
                        uint64_t rows[1];
                        uint64_t cols[1];
                        const uint64_t num_symbols = 1u;
                        symbols[0] = 'X';
                        rows[0] = cursor.row;
                        cols[0] = cursor.col;
                        mli_Image_print_chars(
                                &img_gamma,
                                symbols,
                                rows,
                                cols,
                                num_symbols,
                                print_mode);
                        {
                                struct mli_camera_PinHole pin_hole_camera =
                                        mli_camera_PinHole_set(
                                                view.field_of_view,
                                                &img,
                                                row_over_column_pixel_ratio);

                                struct mli_HomTraComp camera2root_comp =
                                        mli_View_to_HomTraComp(view);
                                struct mli_HomTra camera2root =
                                        mli_HomTraComp_from_compact(
                                                camera2root_comp);
                                struct mli_Ray probing_ray_wrt_camera;
                                struct mli_Ray probing_ray_wrt_root;

                                probing_ray_wrt_camera =
                                        mli_camera_PinHole_ray_at_row_col(
                                                &pin_hole_camera,
                                                &img,
                                                cursor.row,
                                                cursor.col);
                                probing_ray_wrt_root = mli_HomTraComp_ray(
                                        &camera2root, probing_ray_wrt_camera);

                                probing_intersection =
                                        mli_IntersectionSurfaceNormal_init();

                                has_probing_intersection =
                                        mli_raytracing_query_intersection_with_surface_normal(
                                                scenery,
                                                probing_ray_wrt_root,
                                                &probing_intersection);
                        }
                } else {
                        mli_Image_print(&img_gamma, print_mode);
                }
                mli_viewer_print_info_line(
                        view, cursor, tracer_config, gamma, gain);
                if (cursor.active) {
                        printf("Intersection: ");
                        if (has_probing_intersection) {

                                printf("(% 5d;% 5d,% 5d,% 5d)"
                                       "/"
                                       "(id;ref,obj,face), "
                                       "dist % 6.2fm, "
                                       "pos [% -.2e, % -.2e, % -.2e]m, "
                                       "normal [% -.3f, % -.3f, % -.3f], ",
                                       scenery->geometry.robject_ids
                                               [probing_intersection.geometry_id
                                                        .robj],
                                       probing_intersection.geometry_id.robj,
                                       scenery->geometry.robjects
                                               [probing_intersection.geometry_id
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
                        mli_viewer_print_help();
                }
                if (print_scenery_info) {
                        mli_viewer_clear_screen();
                        mli_Scenery_info_fprint(stdout, scenery);
                }
        }

        mli_ColorMaterials_free(&color_materials);
        mli_Image_free(&img);
        mli_Image_free(&img2);
        mli_Image_free(&img_gamma);
        return CHK_SUCCESS;
chk_error:
        mli_ColorMaterials_free(&color_materials);
        mli_Image_free(&img);
        mli_Image_free(&img2);
        mli_Image_free(&img_gamma);
        return CHK_FAIL;
}
