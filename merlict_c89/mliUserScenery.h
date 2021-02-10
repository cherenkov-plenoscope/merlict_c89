/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIUSERSCENERY_H_
#define MERLICT_C89_MLIUSERSCENERY_H_

#include "mliMaterials.h"
#include "mliFrame.h"
#include "mliDynMap.h"
#include "mliArchive.h"

struct mliNameMap {
        struct mliDynMap objects;
        struct mliDynMap functions;
        struct mliDynMap colors;
        struct mliDynMap media;
        struct mliDynMap surfaces;
        struct mliDynMap boundary_layers;
};
struct mliNameMap mliNameMap_init(void);
int mliNameMap_malloc(struct mliNameMap *namemap);
void mliNameMap_free(struct mliNameMap *namemap);


int __mliMaterialsCapacity_from_materials_json(
        struct mliMaterialsCapacity *rescap,
        const struct mliJson *json);

void __mli_strip_key(const char *filename, char *key);

#endif
