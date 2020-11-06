/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_ARCHIVE_H_
#define MERLICT_C89_ARCHIVE_H_

#include "mli_debug.h"
#include "mliTar.h"
#include "mli_json.h"
#include "mliObject_wavefront.h"
#include "mliFunc_comma_seperated_values.h"
#include "mliDynMap.h"
#include "mli_string_to.h"


struct mliArc {
        struct mliJson geometry;
        struct mliJson materials;

        struct mliDynMap objects;
        struct mliDynMap functions;
};

struct mliArc mliArc_init(void);

void mliArc_free(struct mliArc *arc);
int mliArc_malloc_from_tar(struct mliArc *arc, const char *path);

#endif
