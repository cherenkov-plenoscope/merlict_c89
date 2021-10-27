/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIUSERSCENERY_H_
#define MLIUSERSCENERY_H_

#include "mliMaterials.h"
#include "mliFrame.h"
#include "mliDynMap.h"
#include "mliArchive.h"

struct mliNameMap {
        struct mliDynMap objects;
        struct mliDynMap media;
        struct mliDynMap surfaces;
        struct mliDynMap boundary_layers;
};
struct mliNameMap mliNameMap_init(void);
int mliNameMap_malloc(struct mliNameMap *namemap);
void mliNameMap_free(struct mliNameMap *namemap);

#endif
