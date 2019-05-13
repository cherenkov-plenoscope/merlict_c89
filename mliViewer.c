/* Copyright 2019 Sebastian Achim Mueller                                     */
/* Compile with: gcc mliViewer.c -o view -std=c89 -lm -Wall -pedantic              */
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
    printf("merlict c89 by Sebastian Achim Mueller\n");
    printf("\n");
    printf("_Position__________________   _Orientation_______________\n");
    printf(" move forward........[ w ]     look up.............[ i ]\n");
    printf(" move backward.......[ s ]     look down...........[ k ]\n");
    printf(" move left...........[ a ]     look left...........[ j ]\n");
    printf(" move right..........[ d ]     look right..........[ l ]\n");
    printf(" move up.............[ q ]\n");
    printf(" move down...........[ e ]\n");
    printf("\n");
    printf("_Field_of_View_____________\n");
    printf(" increace............[ n ]\n");
    printf(" decreace............[ m ]\n");
    printf("\n");
    printf(" print help..........[ h ]\n");
    printf(" exit ...............[ESC]\n");
    printf("\n");
    printf("[  space key  ] full resolution.\n");
}

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
    uint64_t num_screenshots = 0;
    char timestamp[1024];
    mliScenery scenery;
    mliOcTree octree;
    mliCamera camera;
    mliImage img;
    int update_image = 1;
    time_stamp(timestamp);
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <scenery-path>\n", argv[0]);
        goto restore_terminal_and_exit;
    }
    if (!mliScenery_read_from_path(&scenery, argv[1])) {
        fprintf(stderr, "Can not open '%s'\n", argv[1]);
        goto restore_terminal_and_exit;
    }

    octree = mliOcTree_from_scenery(&scenery);

    camera.position.x = -20.;
    camera.position.y = 0.;
    camera.position.z = 0.;
    camera.rotation.x = 0.;
    camera.rotation.y = -mli_deg2rad(90.);
    camera.rotation.z = 0.;
    camera.field_of_view = mli_deg2rad(80.);

    mliImage_init(&img, 128u, 72u);

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
            case MLI_SPACE_KEY:
                {
                    char path[1024];
                    mliImage full;
                    sprintf(path, "%s_%06lu.ppm", timestamp, num_screenshots++);
                    mliImage_init(&full, 1920u, 1080);
                    mliCamera_render_image(&camera, &scenery, &octree, &full);
                    mliImage_write_to_ppm(&full, path);
                    mliImage_free(&full);
                }
                update_image = 0;
            break;
        }
        if (update_image) {
            mliCamera_render_image(&camera, &scenery, &octree, &img);
            clear_screen();
            mliImage_print(&img);
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
                mli_rad2deg(camera.field_of_view));
        }
    }

    mliImage_free(&img);
    mliOcTree_free(&octree);
    mliScenery_free(&scenery);

    restore_terminal_and_exit:
    tcsetattr(STDIN_FILENO, TCSANOW, &old_terminal);
    return EXIT_SUCCESS;
}
