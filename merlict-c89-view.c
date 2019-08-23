/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with:                                                              */
/* gcc merlict-c89-view.c -o merlict-c89-view -std=c89 -lm -Wall -pedantic    */
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>     /* STDIN_FILENO */
#include <assert.h>
#include <time.h>

#include "merlict_c89.h"

#define MLI_ESCAPE_KEY 27
#define MLI_SPACE_KEY 32

void clear_screen() {
    printf("\033[2J\n");}

void print_help() {
    clear_screen();
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
    printf(" print help..........[ h ]\n");
    printf(" exit ...............[ESC]\n");
    printf("\n");
    printf("[  space key  ] full resolution.\n");}

void print_info_line(const mliCamera camera) {
    printf(
        "Press 'h' for help. "
        "Pos.: [%.2f, %.2f, %.2f]m, "
        "Rot.: [%.2f, %.2f %.2f]deg, "
        "FoV.: %.2fdeg.\n",
        camera.position.x,
        camera.position.y,
        camera.position.z,
        mli_rad2deg(camera.rotation.x),
        mli_rad2deg(camera.rotation.y),
        mli_rad2deg(camera.rotation.z),
        mli_rad2deg(camera.field_of_view));}

void time_stamp(char* buffer) {
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

struct termios mli_disable_terminal_stdin_buffer() {
    /* Thanks to Edwin Buck */
    struct termios old_terminal, new_terminal;
    tcgetattr(STDIN_FILENO, &old_terminal);
    new_terminal = old_terminal;
    new_terminal.c_lflag &= ~(ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_terminal);
    return old_terminal;}

int key_truncate_8bit(const int key) {
    if (key == -1)
        return key;
    else
        return key & 255;}

int main(int argc, char *argv[]) {
    struct termios old_terminal = mli_disable_terminal_stdin_buffer();
    int key;
    const uint64_t num_cols = 128u;
    const uint64_t num_rows = 72u;
    int super_resolution = 0;
    uint64_t num_screenshots = 0;
    char timestamp[1024];
    mliScenery scenery = mliScenery_init();
    mliOcTree octree;
    mliCamera camera;
    mliImage img = mliImage_init();
    mliImage img2 = mliImage_init();
    int update_image = 1;
    time_stamp(timestamp);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <scenery-path>\n", argv[0]);
        goto error;
    }
    if (mli_string_ends_with(argv[1], ".json")) {
        mliJson json = mliJson_init();
        mliUserScenery uscn = mliUserScenery_init();
        mli_check(
            mliJson_malloc_from_file(&json, argv[1]),
            "Failed to parse json-scenery.");
        mli_check(
            mliUserScenery_malloc_from_json(&uscn, &json),
            "Failed to parse json-scenery.");
        mli_check(
            mliScenery_malloc_from_mliUserScenery(&scenery, &uscn),
            "Failed to translate mliUserScenery to mliScenery.")
    } else {
        if (!mliScenery_read_from_path(&scenery, argv[1])) {
            fprintf(stderr, "Can not open '%s'\n", argv[1]);
            goto error;
        }
    }

    octree = mliOcTree_from_scenery(&scenery);

    camera.position.x = -20.;
    camera.position.y = 0.;
    camera.position.z = 0.;
    camera.rotation.x = 0.;
    camera.rotation.y = -mli_deg2rad(90.);
    camera.rotation.z = 0.;
    camera.field_of_view = mli_deg2rad(80.);

    mli_check_mem(mliImage_malloc(&img, num_cols, num_rows));
    mli_check_mem(mliImage_malloc(&img2, num_cols*2u, num_rows*2u));

    goto show_image;

    while((key = key_truncate_8bit(getchar())) != MLI_ESCAPE_KEY) {
        update_image = 1;
        switch (key) {
            case 'w':
                camera = mliCamera_move_forward(camera, 1);
                break;
            case 's':
                camera = mliCamera_move_forward(camera, -1);
                break;
            case 'a':
                camera = mliCamera_move_right(camera, -1.);
                break;
            case 'd':
                camera = mliCamera_move_right(camera, 1.);
                break;
            case 'q':
                camera = mliCamera_move_up(camera, 1.);
                break;
            case 'e':
                camera = mliCamera_move_up(camera, -1.);
                break;
            case 'i':
                camera = mliCamera_look_up_when_possible(camera, .05);
                break;
            case 'k':
                camera = mliCamera_look_down_when_possible(camera, .05);
                break;
            case 'l':
                camera = mliCamera_look_right(camera, -.05);
                break;
            case 'j':
                camera = mliCamera_look_right(camera, .05);
                break;

            case 'n':
                camera = mliCamera_decrease_fov(camera, 1.05);
                break;
            case 'm':
                camera = mliCamera_increase_fov(camera, 1.05);
                break;
            case 'h':
                print_help();
                update_image = 0;
                break;
            case 'b':
                super_resolution = !super_resolution;
                break;
            case MLI_SPACE_KEY:
                {
                    char path[1024];
                    mliImage full = mliImage_init();
                    sprintf(path, "%s_%06lu.ppm", timestamp, num_screenshots++);
                    mli_check_mem(
                        mliImage_malloc(&full, 1920u, 1080u));
                    mliCamera_render_image(&camera, &scenery, &octree, &full);
                    mliImage_write_to_ppm(&full, path);
                    mliImage_free(&full);
                }
                update_image = 0;
            break;
        }
        show_image:
        if (update_image) {
            if (super_resolution) {
                mliCamera_render_image(&camera, &scenery, &octree, &img2);
                mliImage_scale_down_twice(&img2, &img);
            } else {
                mliCamera_render_image(&camera, &scenery, &octree, &img);
            }
            clear_screen();
            mliImage_print(&img);
            print_info_line(camera);
        }
    }

    mliImage_free(&img);
    mliImage_free(&img2);
    mliOcTree_free(&octree);
    mliScenery_free(&scenery);

    return EXIT_SUCCESS;
error:
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return EXIT_FAILURE;
}
