/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MLI_VIEWER_VIEWER_H_
#define MLI_VIEWER_VIEWER_H_

#include "../mli/mliScenery.h"
#include "../mli/mliTracer.h"
#include "../prng/prng.h"
#include "../view/view.h"
#include "Cursor.h"
#include "Config.h"

#define MLI_VIEWER_ESCAPE_KEY 27
#define MLI_VIEWER_SPACE_KEY 32

void mli_viewer_clear_screen(void);

void mli_viewer_print_help(void);

void mli_viewer_print_info_line(
        const struct mli_View view,
        const struct mli_viewer_Cursor cursor,
        const struct mliTracerConfig tracer_config);

void mli_viewer_timestamp_now_19chars(char *buffer);

int mli_viewer_export_image(
        const struct mliTracer *tracer,
        const struct mli_viewer_Config config,
        const struct mli_View view,
        struct mli_Prng *prng,
        const double object_distance,
        const char *path);

int mli_viewer_run_interactive_viewer(
        const struct mliScenery *scenery,
        const struct mli_viewer_Config config);
int mli_viewer_run_interactive_viewer_try_non_canonical_stdin(
        const struct mliScenery *scenery,
        const struct mli_viewer_Config config);
#endif
