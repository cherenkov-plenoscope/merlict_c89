/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MERLICT_C89_VIWER_MLIVR_VIEWER_H_
#define MERLICT_C89_VIWER_MLIVR_VIEWER_H_

#include "../merlict_c89/mliImage.h"
#include "../merlict_c89/mliView.h"
#include "../merlict_c89/mliScenery.h"

#include "mlivrCursor.h"
#include "mlivrConfig.h"

#define MLIVR_ESCAPE_KEY 27
#define MLIVR_SPACE_KEY 32

void mlivr_clear_screen(void);

void mlivr_print_help(void);

void mlivr_print_info_line(
        const struct mliView view,
        const struct mlivrCursor cursor,
        const struct mliTracerCongig tracer_config);

void mlivr_timestamp_now_19chars(char *buffer);

int mlivr_truncate_8bit(const int key);

int _mlivr_export_image(
        const struct mliScenery *scenery,
        const struct mlivrConfig config,
        const struct mliView view,
        struct mliMT19937 *prng,
        const struct mliTracerCongig *tracer_config,
        const double object_distance,
        const char *path);

int mlivr_run_interactive_viewer(
        const struct mliScenery *scenery,
        const struct mlivrConfig config);
int mlivr_run_interactive_viewer_try_non_canonical_stdin(
        const struct mliScenery *scenery,
        const struct mlivrConfig config);
#endif
