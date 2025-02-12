/* Copyright 2019 Sebastian Achim Mueller                                     */
#ifndef MLI_VIEWER_VIEWER_H_
#define MLI_VIEWER_VIEWER_H_

#include "../chk/chk.h"
#include "../scenery/scenery.h"
#include "../prng/prng.h"
#include "../view/view.h"
#include "../pathtracer/pathtracer.h"
#include "Cursor.h"
#include "Config.h"

#define mli_key_code int

enum mli_viewer_key_codes {
        MLI_VIEWER_ESCAPE_KEY = 27,
        MLI_VIEWER_SPACE_KEY = 32
};

mli_key_code mli_viewer_get_key(void);

void mli_viewer_clear_screen(void);

void mli_viewer_print_help(void);

void mli_viewer_print_info_line(
        const struct mli_View view,
        const struct mli_viewer_Cursor cursor,
        const struct mli_pathtracer_Config tracer_config,
        const double gamma,
        const double gain);

void mli_viewer_timestamp_now_19chars(char *buffer);

chk_rc mli_viewer_export_image(
        const struct mli_PathTracer *tracer,
        const struct mli_viewer_Config config,
        const struct mli_View view,
        struct mli_Prng *prng,
        const double object_distance,
        const double gamma,
        const double gain,
        const char *path);

chk_rc mli_viewer_run_interactive_viewer(
        const struct mli_Scenery *scenery,
        const struct mli_viewer_Config config);
chk_rc mli_viewer_run_interactive_viewer_try_non_canonical_stdin(
        const struct mli_Scenery *scenery,
        const struct mli_viewer_Config config);
#endif
