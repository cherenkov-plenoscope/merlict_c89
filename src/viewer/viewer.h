/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MLI_VIEWER_VIEWER_H_
#define MLI_VIEWER_VIEWER_H_

#include "../scenery/scenery.h"
#include "../prng/prng.h"
#include "../view/view.h"
#include "../pathtracer/pathtracer.h"
#include "Cursor.h"
#include "Config.h"

#define MLI_VIEWER_ESCAPE_KEY 27
#define MLI_VIEWER_SPACE_KEY 32

void mli_viewer_clear_screen(void);

void mli_viewer_print_help(void);

void mli_viewer_print_info_line(
        const struct mli_View view,
        const struct mli_viewer_Cursor cursor,
        const struct mli_pathtracer_Config tracer_config,
        const double gamma);

void mli_viewer_timestamp_now_19chars(char *buffer);

int mli_viewer_export_image(
        const struct mli_PathTracer *tracer,
        const struct mli_viewer_Config config,
        const struct mli_View view,
        struct mli_Prng *prng,
        const double object_distance,
        const double gamma,
        const char *path);

int mli_viewer_run_interactive_viewer(
        const struct mli_Scenery *scenery,
        const struct mli_viewer_Config config);
int mli_viewer_run_interactive_viewer_try_non_canonical_stdin(
        const struct mli_Scenery *scenery,
        const struct mli_viewer_Config config);
#endif
