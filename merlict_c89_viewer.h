/* Copyright 2019 Sebastian Achim Mueller                                     */
#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>     /* STDIN_FILENO */
#include <assert.h>
#include <time.h>

#include "mliImage.h"
#include "mliCamera.h"
#include "mliScenery.h"
#include "mliOcTree.h"

#define MLIVR_ESCAPE_KEY 27
#define MLIVR_SPACE_KEY 32

void mlivr_clear_screen() {
    printf("\033[2J\n");}

void mlivr_print_help() {
    mlivr_clear_screen  ();
    printf("merlict-c89\n-----------\n\n");
    printf("Copyright 2019 Sebastian Achim Mueller\n");
    printf("\n");
    printf("_Position__________________   _Orientation_______________\n");
    printf(" move forward........[ w ]     look up.............[ i ]\n");
    printf(" move backward.......[ s ]     look down...........[ k ]\n");
    printf(" move left...........[ a ]     look left...........[ j ]\n");
    printf(" move right..........[ d ]     look right..........[ l ]\n");
    printf(" move up.............[ q ]\n");
    printf(" move down...........[ e ]    _Quality___________________\n");
    printf("                               super resolution....[ b ]\n");
    printf("_Field_of_View_____________\n");
    printf(" increace............[ n ]\n");
    printf(" decreace............[ m ]\n");
    printf("\n");
    printf(" Cursor..............[ c ]\n");
    printf(" print help..........[ h ]\n");
    printf(" exit ...............[ESC]\n");
    printf("\n");
    printf("[  space key  ] full resolution.\n");}

struct mlivrCursor {
    int64_t active;
    uint64_t col;
    uint64_t row;
    uint64_t num_cols;
    uint64_t num_rows;
};

void mlivr_print_info_line(
    const struct mliCamera camera,
    const struct mlivrCursor cursor)
{
    printf(
        "Press 'h' for help. "
        "Pos.: [ % -.1f, % -.1f, % -.1f]m, "
        "Rot.: [ % -.1f, % -.1f, % -.1f]deg, "
        "FoV.: %.2fdeg",
        camera.position.x,
        camera.position.y,
        camera.position.z,
        mli_rad2deg(camera.rotation.x),
        mli_rad2deg(camera.rotation.y),
        mli_rad2deg(camera.rotation.z),
        mli_rad2deg(camera.field_of_view));
    if (cursor.active) {
        printf(", Cursor [% 2ld,% 2ld]pix", cursor.col, cursor.row);
    }
    printf(".\n");}

void mlivr_timestamp_now_19chars(char* buffer) {
    time_t now = time(0);
    struct tm * nowtm;
    nowtm = localtime(&now);
    sprintf(buffer, "%04u-%02u-%02u_%02u-%02u-%02u",
        nowtm->tm_year + 1900,
        nowtm->tm_mon + 1,
        nowtm->tm_mday,
        nowtm->tm_hour,
        nowtm->tm_min,
        nowtm->tm_sec);}

struct termios mlivr_disable_stdin_buffer() {
    /* Thanks to Edwin Buck */
    struct termios old_terminal, new_terminal;
    tcgetattr(STDIN_FILENO, &old_terminal);
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
    return old_terminal;}

int mlivr_truncate_8bit(const int key) {
    if (key == -1)
        return key;
    else
        return key & 255;}

struct mlivrConfig{
    uint64_t preview_num_cols;
    uint64_t preview_num_rows;
    uint64_t export_num_cols;
    uint64_t export_num_rows;
    struct mliCamera camera;
};

struct mlivrConfig mlivrConfig_default() {
    struct mlivrConfig cfg;
    cfg.preview_num_cols = 128u;
    cfg.preview_num_rows = 72u;

    cfg.export_num_cols = 1920u;
    cfg.export_num_rows = 1080u;

    cfg.camera.position.x = -20.;
    cfg.camera.position.y = 0.;
    cfg.camera.position.z = 0.;
    cfg.camera.rotation.x = 0.;
    cfg.camera.rotation.y = -mli_deg2rad(90.);
    cfg.camera.rotation.z = 0.;
    cfg.camera.field_of_view = mli_deg2rad(80.);
    return cfg;}

void _mlivr_mv_cursor_up(struct mlivrCursor *cursor) {
    if (cursor->row != 0) {cursor->row -= 2;}}

void _mlivr_mv_cursor_down(struct mlivrCursor *cursor) {
    if (cursor->row + 2u < cursor->num_rows) {cursor->row += 2;}}

void _mlivr_mv_cursor_right(struct mlivrCursor *cursor) {
    if (cursor->col != 0) {cursor->col -= 1;}}

void _mlivr_mv_cursor_left(struct mlivrCursor *cursor) {
    if (cursor->col + 1u < cursor->num_cols) {cursor->col += 1;}}

int mlivr_run_interactive_viewer(
    const struct mliScenery *scenery,
    const struct mliOcTree *octree,
    const struct mlivrConfig config)
{
    struct termios old_terminal = mlivr_disable_stdin_buffer();

    int key;
    int super_resolution = 0;
    struct mlivrCursor cursor;
    uint64_t num_screenshots = 0;
    char timestamp[20];
    struct mliCamera camera = config.camera;
    struct mliImage img = mliImage_init();
    struct mliImage img2 = mliImage_init();
    int update_image = 1;
    int print_help = 0;
    struct mliRay probing_ray;
    int has_probing_intersection;
    mliIntersection probing_intersection;

    mlivr_timestamp_now_19chars(timestamp);
    img.num_cols = config.preview_num_cols;
    img.num_rows = config.preview_num_rows;
    img2.num_cols = config.preview_num_cols*2u;
    img2.num_rows = config.preview_num_rows*2u;
    mli_check_mem(mliImage_malloc(&img));
    mli_check_mem(mliImage_malloc(&img2));

    cursor.active = 0;
    cursor.col = config.preview_num_cols/2;
    cursor.row = config.preview_num_rows/2;
    cursor.num_cols = config.preview_num_cols;
    cursor.num_rows = config.preview_num_rows;
    goto show_image;

    while((key = mlivr_truncate_8bit(getchar())) != MLIVR_ESCAPE_KEY) {
        update_image = 1;
        print_help = 0;
        if (cursor.active) {
            update_image = 0;
            super_resolution = 0;
            switch (key) {
                case 'i': _mlivr_mv_cursor_up(&cursor); break;
                case 'k': _mlivr_mv_cursor_down(&cursor); break;
                case 'l': _mlivr_mv_cursor_left(&cursor); break;
                case 'j': _mlivr_mv_cursor_right(&cursor); break;
                case 'c': cursor.active = !cursor.active; break;
                case 'h': print_help = 1; break;
                default: printf("Key Press unknown: %d\n", key); break;
            }
        } else {
            switch (key) {
                case 'w': camera = mliCamera_move_forward(camera, 1); break;
                case 's': camera = mliCamera_move_forward(camera, -1); break;
                case 'a': camera = mliCamera_move_right(camera, -1.); break;
                case 'd': camera = mliCamera_move_right(camera, 1.); break;
                case 'q': camera = mliCamera_move_up(camera, 1.); break;
                case 'e': camera = mliCamera_move_up(camera, -1.); break;
                case 'i':
                    camera = mliCamera_look_up_when_possible(camera, .05);
                    break;
                case 'k':
                    camera = mliCamera_look_down_when_possible(camera, .05);
                    break;
                case 'l': camera = mliCamera_look_right(camera, -.05); break;
                case 'j': camera = mliCamera_look_right(camera, .05); break;
                case 'n': camera = mliCamera_decrease_fov(camera, 1.05); break;
                case 'm': camera = mliCamera_increase_fov(camera, 1.05); break;
                case 'h': print_help = 1; update_image = 0; break;
                case 'b': super_resolution = !super_resolution; break;
                case 'c':
                    cursor.active = !cursor.active;
                    update_image = 0;
                    break;
                case MLIVR_SPACE_KEY:
                    {
                        char path[1024];
                        struct mliImage full = mliImage_init();
                        sprintf(
                            path,
                            "%s_%06lu.ppm",
                            timestamp,
                            num_screenshots++);
                        full.num_cols = config.export_num_cols;
                        full.num_rows = config.export_num_rows;
                        mli_check_mem(mliImage_malloc(&full));
                        mliCamera_render_image(&camera, scenery, octree, &full);
                        mliImage_write_to_ppm(&full, path);
                        mliImage_free(&full);
                    }
                    update_image = 0;
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
                mliCamera_render_image(&camera, scenery, octree, &img2);
                mliImage_scale_down_twice(&img2, &img);
            } else {
                mliCamera_render_image(&camera, scenery, octree, &img);
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
            mliImage_print_chars(&img, symbols, rows, cols, num_symbols);
            {
                struct mliCameraSensor sensor;
                mliCameraSensor_init(&sensor, &camera, &img);
                probing_ray = mliCamera_ray_at_row_col(
                    &camera,
                    &sensor,
                    &img,
                    cursor.row,
                    cursor.col);
                has_probing_intersection = mli_first_casual_intersection(
                    scenery,
                    octree,
                    probing_ray,
                    &probing_intersection);
            }
        } else {
            mliImage_print(&img);
        }
        mlivr_print_info_line(camera, cursor);
        if (cursor.active) {
            if (has_probing_intersection) {
                char type_string[1024];
                const mliIndex object_index = __mliScenery_resolve_index(
                    scenery,
                    probing_intersection.object_idx);
                mli_c(mli_type_to_string(object_index.type, type_string));
                printf(
                    "Obj % 6ld, %-16s, "
                    "dist % 3.1fm, "
                    "normal [% -.2f,% -.2f,% -.2f], "
                    "surf %d.\n",
                    probing_intersection.object_idx,
                    type_string,
                    probing_intersection.distance_of_ray,
                    probing_intersection.surface_normal.x,
                    probing_intersection.surface_normal.y,
                    probing_intersection.surface_normal.z,
                    probing_intersection.from_outside_to_inside);
            } else {
                printf("No intersection.\n");
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
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return 1;
error:
    mliImage_free(&img);
    mliImage_free(&img2);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return 0;
}
