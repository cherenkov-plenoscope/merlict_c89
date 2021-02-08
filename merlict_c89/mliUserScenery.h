/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIUSERSCENERY_H_
#define MERLICT_C89_MLIUSERSCENERY_H_

#include "mliSceneryResources.h"
#include "mliFrame.h"
#include "mliDynMap.h"
#include "mliArchive.h"

struct mliUserScenery {
        struct mliSceneryResources resources;
        struct mliFrame root;

        struct mliDynMap object_names;
        struct mliDynMap function_names;
        struct mliDynMap color_names;
        struct mliDynMap medium_names;
        struct mliDynMap surface_names;
};

void mliUserScenery_free(struct mliUserScenery *uscn);
struct mliUserScenery mliUserScenery_init(void);
int mliUserScenery_malloc_from_Archive(
        struct mliUserScenery *uscn,
        const struct mliArchive *arc);
int mliUserScenery_malloc_from_tar(
        struct mliUserScenery *uscn,
        const char *path);

int __mliSceneryResourcesCapacity_from_materials_json(
        struct mliSceneryResourcesCapacity *rescap,
        const struct mliJson *json);
void __mli_strip_key(const char *filename, char *key);

#endif
