/* Copyright 2019 Sebastian Achim Mueller                                     */
#include "viewer.h"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>
#include "../chk/chk.h"
#include "../mtl/math.h"
#include "../mli/mliIntersectionSurfaceNormal.h"
#include "../mli/mliImage.h"
#include "../mli/mliImage_print.h"
#include "../mli/mliImage_ppm.h"
#include "../mli/mliPinHoleCamera.h"
#include "../mli/mliApertureCamera.h"
#include "../mli/mli_ray_scenery_query.h"
#include "../mli/mli_version.h"
#include "toggle_stdin.h"

void mlivr_clear_screen(void)
{
        uint64_t n = 20;
        while (n) {
                putchar('\n');
                n--;
        }
}

void mlivr_print_help(void)
{
        mlivr_clear_screen();
        mli_logo_fprint(stdout);
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
        printf("    increace          [  m  ]     super sampling    [  b  ]\n");
        printf("    decreace          [  n  ]     color/monochrome  [  g  ]\n");
        printf("\n");
        printf("  Atmosphere                    Sun\n");
        printf("    on/off            [  0  ]     later daytime     [  9  ]\n");
        printf("    - altitude        [  4  ]     earlier daytime   [  8  ]\n");
        printf("    + altitude        [  5  ]     + latitude        [  7  ]\n");
        printf("                                  - latitude        [  6  ]\n");
        printf("\n");
        mli_authors_and_affiliations_fprint(stdout);
}

void mlivr_print_info_line(
        const struct mliView view,
        const struct mlivrCursor cursor,
        const struct mliTracerConfig tracer_config)
{
        printf("Help 'h', "
               "Cam: "
               "pos[% -.2e, % -.2e, % -.2e]m, "
               "rot[% -.1f, % -.1f, % -.1f]deg, "
               "fov %.2fdeg, ",
               view.position.x,
               view.position.y,
               view.position.z,
               mtl_math_rad2deg(view.rotation.x),
               mtl_math_rad2deg(view.rotation.y),
               mtl_math_rad2deg(view.rotation.z),
               mtl_math_rad2deg(view.field_of_view));
        printf("Sun: lat % 3.0fdeg, %02d:%02dh, alt % 3.1fkm",
               mtl_math_rad2deg(tracer_config.atmosphere.sunLatitude),
               (int)(tracer_config.atmosphere.sunHourAngle),
               (int)(tracer_config.atmosphere.sunHourAngle * 60) % 60,
               tracer_config.atmosphere.altitude * 1e-3);
        if (cursor.active) {
                printf(", Cursor[%3ld, %3ld]pix", cursor.col, cursor.row);
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

int mlivr_get_key(void)
{
        /* Waits for keystroke and returns ascii-code.
         */
        const int key = getchar();
        if (key == EOF) {
                /* In case of EOF, return EOF */
                return key;
        } else {
                /* On some systems we have to truncate. I dont know why. */
                return key & 255;
        }
}

int mlivr_export_image(
        const struct mliTracer *tracer,
        const struct mlivrConfig config,
        const struct mliView view,
        struct mtl_Prng *prng,
        const double object_distance,
        const char *path)
{
        struct mliImage full = mliImage_init();
        struct mliHomTraComp camera2root_comp;
        struct mliApertureCamera apcam = mliApertureCamera_init();

        const double image_ratio =
                ((double)config.export_num_cols /
                 (double)config.export_num_rows);
        chk_mem(mliImage_malloc(
                &full, config.export_num_cols, config.export_num_rows));
        camera2root_comp = mliView_to_HomTraComp(view);
        apcam.focal_length =
                mliApertureCamera_focal_length_given_field_of_view_and_sensor_width(
                        view.field_of_view,
                        config.aperture_camera_image_sensor_width);
        apcam.aperture_radius = 0.5 * (apcam.focal_length /
                                       config.aperture_camera_f_stop_ratio);
        apcam.image_sensor_distance =
                mli_thin_lens_get_image_given_focal_and_object(
                        apcam.focal_length, object_distance);
        apcam.image_sensor_width_x = config.aperture_camera_image_sensor_width;
        apcam.image_sensor_width_y = apcam.image_sensor_width_x / image_ratio;
        mliApertureCamera_render_image(
                apcam, camera2root_comp, tracer, &full, prng);
        chk_msg(mliImage_write_to_path(&full, path), "Failed to write ppm.");
        mliImage_free(&full);
        return 1;
chk_error:
        return 0;
}

int mlivr_run_interactive_viewer_try_non_canonical_stdin(
        const struct mliScenery *scenery,
        const struct mlivrConfig config)
{
#ifdef HAVE_TERMIOS_H
        struct termios old_terminal = mlivr_non_canonical_stdin();
#endif
        int rc = mlivr_run_interactive_viewer(scenery, config);

#ifdef HAVE_TERMIOS_H
        mlivr_restore_stdin(&old_terminal);
#endif
        return rc;
}

int mlivr_run_interactive_viewer(
        const struct mliScenery *scenery,
        const struct mlivrConfig config)
{
        struct mtl_Prng prng = mtl_Prng_init_MT19937(config.random_seed);
        struct mliTracerConfig tracer_config = mliTracerConfig_init();
        struct mliTracer tracer = mliTracer_init();
        struct mliColorObserver color_observer = mliColorObserver_init();
        struct mliColorMaterials color_materials = mliColorMaterials_init();
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
        int print_scenery_info = 0;
        int has_probing_intersection = 0;
        struct mliIntersectionSurfaceNormal probing_intersection;

        chk_msg(mliColorObserver_malloc_cie1931(&color_observer),
                "Can't malloc color observer.");
        chk_msg(mliColorMaterials_malloc_from_Materials(
                        &color_materials, &scenery->materials, &color_observer),
                "Can't malloc color materials from scenery materials.");
        mliColorObserver_free(&color_observer);

        tracer.scenery = scenery;
        tracer.config = &tracer_config;
        tracer.scenery_color_materials = &color_materials;

        mlivr_timestamp_now_19chars(timestamp);
        chk_mem(mliImage_malloc(
                &img, config.preview_num_cols, config.preview_num_rows));
        chk_mem(mliImage_malloc(
                &img2,
                config.preview_num_cols * 2u,
                config.preview_num_rows * 2u));

        cursor.active = 0;
        cursor.col = config.preview_num_cols / 2;
        cursor.row = config.preview_num_rows / 2;
        cursor.num_cols = config.preview_num_cols;
        cursor.num_rows = config.preview_num_rows;
        goto show_image;

        while ((key = mlivr_get_key()) != MLIVR_ESCAPE_KEY) {
                update_image = 1;
                print_help = 0;
                print_scenery_info = 0;
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
                                chk(mlivr_export_image(
                                        &tracer,
                                        config,
                                        view,
                                        &prng,
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
                        case 'p':
                                print_scenery_info = 1;
                                update_image = 0;
                                break;

                        case '4':
                                mliAtmosphere_decrease_altitude(
                                        &tracer_config.atmosphere, 0.9);
                                break;
                        case '5':
                                mliAtmosphere_increase_altitude(
                                        &tracer_config.atmosphere, 1.1);
                                break;
                        case '6':
                                mliAtmosphere_decrease_latitude(
                                        &tracer_config.atmosphere,
                                        mtl_math_deg2rad(2.0));
                                break;
                        case '7':
                                mliAtmosphere_increase_latitude(
                                        &tracer_config.atmosphere,
                                        mtl_math_deg2rad(2.0));
                                break;
                        case '8':
                                mliAtmosphere_decrease_hours(
                                        &tracer_config.atmosphere, 0.1);
                                break;
                        case '9':
                                mliAtmosphere_increase_hours(
                                        &tracer_config.atmosphere, 0.1);
                                break;
                        case '0':
                                tracer_config.have_atmosphere =
                                        !tracer_config.have_atmosphere;
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
                                        &tracer,
                                        &img2,
                                        row_over_column_pixel_ratio,
                                        &prng);
                                mliImage_scale_down_twice(&img2, &img);
                        } else {
                                mliPinHoleCamera_render_image_with_view(
                                        view,
                                        &tracer,
                                        &img,
                                        row_over_column_pixel_ratio,
                                        &prng);
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

                                probing_intersection =
                                        mliIntersectionSurfaceNormal_init();

                                has_probing_intersection =
                                        mli_query_intersection_with_surface_normal(
                                                scenery,
                                                probing_ray_wrt_root,
                                                &probing_intersection);
                        }
                } else {
                        mliImage_print(&img, print_mode);
                }
                mlivr_print_info_line(view, cursor, tracer_config);
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
                        mlivr_print_help();
                }
                if (print_scenery_info) {
                        mlivr_clear_screen();
                        mliScenery_info_fprint(stdout, scenery);
                }
        }

        mliColorMaterials_free(&color_materials);
        mliImage_free(&img);
        mliImage_free(&img2);
        return 1;
chk_error:
        mliImage_free(&img);
        mliImage_free(&img2);
        return 0;
}
