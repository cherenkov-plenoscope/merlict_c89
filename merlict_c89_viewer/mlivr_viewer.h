/* Copyright 2019 Sebastian Achim Mueller                                     */
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <time.h>

#include "../merlict_c89/mliImage.h"
#include "../merlict_c89/mliCamera.h"
#include "../merlict_c89/mliScenery.h"
#include "../merlict_c89/mliOcTree.h"

#include "mlivrCursor.h"
#include "mlivrConfig.h"

#define MLIVR_ESCAPE_KEY 27
#define MLIVR_SPACE_KEY 32

void mlivr_clear_screen(void);

void mlivr_print_help(void);

void mlivr_print_info_line(
        const struct mliCamera camera,
        const struct mlivrCursor cursor);

void mlivr_timestamp_now_19chars(char *buffer);

void mlivr_restore_stdin_buffer(struct termios *old_terminal);

int mlivr_truncate_8bit(const int key);

int _mlivr_export_image(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mlivrConfig config,
        const struct mliCamera camera,
        const char *path);

int mlivr_run_interactive_viewer(
        const struct mliScenery *scenery,
        const struct mliOcTree *octree,
        const struct mlivrConfig config);
