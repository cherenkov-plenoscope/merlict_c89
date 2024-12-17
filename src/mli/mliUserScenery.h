/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIUSERSCENERY_H_
#define MLIUSERSCENERY_H_

#include "../map/map.h"
#include "mliResourceNameMap.h"

struct mli_Materials;
struct mli_Archive;
int mli_Materials_malloc_form_archive(
        struct mli_Materials *materials,
        struct mli_ResourceNameMap *names,
        const struct mli_Archive *archive);
struct mli_Geometry;
int mli_set_geometry_objects_and_names_from_archive(
        struct mli_Geometry *geometry,
        struct mli_Map *object_names,
        const struct mli_Archive *archive);

struct mli_Object;
struct mli_Frame;
int mli_Frame_tree_from_Archive(
        struct mli_Frame *root,
        const struct mli_Archive *archive,
        const struct mli_Map *object_names,
        const struct mli_Object *objects,
        const struct mli_Map *boundary_layer_names);
#endif
