/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIUSERSCENERY_H_
#define MLIUSERSCENERY_H_

#include "../map/map.h"
#include "mliResourceNameMap.h"

struct mliMaterials;
struct mliArchive;
int mliMaterials_malloc_form_archive(
        struct mliMaterials *materials,
        struct mli_ResourceNameMap *names,
        const struct mliArchive *archive);
struct mliGeometry;
int mli_set_geometry_objects_and_names_from_archive(
        struct mliGeometry *geometry,
        struct mliDynMap *object_names,
        const struct mliArchive *archive);

struct mli_Object;
struct mliFrame;
int mli_check_malloc_root_frame_from_Archive(
        struct mliFrame *root,
        const struct mliArchive *archive,
        const struct mliDynMap *object_names,
        const struct mli_Object *objects,
        const struct mliDynMap *boundary_layer_names);
#endif
