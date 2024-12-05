/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MLI_VIEWER_VIEWER_H_
#define MLI_VIEWER_VIEWER_H_

#include "../mli/mliImage.h"
#include "../mli/mliScenery.h"
#include "../mli/mliTracer.h"
#include "../mtl/prng.h"
#include "../mli/mliView.h"
#include "Cursor.h"
#include "Config.h"

#define MLIVR_ESCAPE_KEY 27
#define MLIVR_SPACE_KEY 32

void mlivr_clear_screen(void);

void mlivr_print_help(void);

void mlivr_print_info_line(
        const struct mliView view,
        const struct mlivrCursor cursor,
        const struct mliTracerConfig tracer_config);

void mlivr_timestamp_now_19chars(char *buffer);

int mlivr_export_image(
        const struct mliTracer *tracer,
        const struct mlivrConfig config,
        const struct mliView view,
        struct mtl_Prng *prng,
        const double object_distance,
        const char *path);

int mlivr_run_interactive_viewer(
        const struct mliScenery *scenery,
        const struct mlivrConfig config);
int mlivr_run_interactive_viewer_try_non_canonical_stdin(
        const struct mliScenery *scenery,
        const struct mlivrConfig config);
#endif
