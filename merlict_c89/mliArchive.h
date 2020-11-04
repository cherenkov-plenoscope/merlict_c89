/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_ARCHIVE_H_
#define MERLICT_C89_ARCHIVE_H_

#include "mli_debug.h"
#include "mliTar.h"
#include "mli_json.h"
#include "mliObject_wavefront.h"
#include "mliFunc_comma_seperated_values.h"
#include "mliMap.h"

struct mliMapItem {
        char key[128];
        void *value;
};

MLIDYNARRAY_DEFINITON(mli, MapItem, struct mliMapItem)

struct mliMapItem mliMapItem_init(void);

int mliDynMapItem_has(const struct mliDynMapItem *map, const char *key);

int mliDynMapItem_malloc_insert(
        struct mliDynMapItem *map,
        const char *key,
        void *value);

int mliDynMapItem_get(
        const struct mliDynMapItem *map,
        const char *key,
        void **value);


struct mliArc {
        struct mliDynMapItem objects;
        struct mliDynMapItem functions;
};

struct mliArc mliArc_init(void);

void mliArc_free(struct mliArc *arc);
int mliArc_malloc_from_tar(struct mliArc *arc, const char *path);




#endif
