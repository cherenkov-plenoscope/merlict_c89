/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry.h"
#include "../object/object_valid.h"
#include "../chk/chk.h"

void mliGeometry_init_objects(struct mliGeometry *geometry)
{
        geometry->num_objects = 0u;
        geometry->objects = NULL;
        geometry->object_names = NULL;
}

void mliGeometry_init_references(struct mliGeometry *geometry)
{
        geometry->num_robjects = 0u;
        geometry->robjects = NULL;
        geometry->robject_ids = NULL;
        geometry->robject2root = NULL;
}

struct mliGeometry mliGeometry_init(void)
{
        struct mliGeometry geometry;
        mliGeometry_init_objects(&geometry);
        mliGeometry_init_references(&geometry);
        return geometry;
}

void mliGeometry_free_objects(struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_objects; i++) {
                mli_Object_free(&(geometry->objects[i]));
                mli_String_free(&(geometry->object_names[i]));
        }
        free(geometry->objects);
        free(geometry->object_names);
        mliGeometry_init_objects(geometry);
}

void mliGeometry_free_references(struct mliGeometry *geometry)
{
        free(geometry->robjects);
        free(geometry->robject_ids);
        free(geometry->robject2root);
        mliGeometry_init_references(geometry);
}

void mliGeometry_free(struct mliGeometry *geometry)
{
        mliGeometry_free_objects(geometry);
        mliGeometry_free_references(geometry);
        (*geometry) = mliGeometry_init();
}

int mliGeometry_malloc_objects(
        struct mliGeometry *geometry,
        const uint32_t num_objects)
{
        uint32_t i;
        geometry->num_objects = num_objects;
        chk_malloc(geometry->objects, struct mli_Object, geometry->num_objects);
        chk_malloc(
                geometry->object_names,
                struct mli_String,
                geometry->num_objects);
        for (i = 0; i < geometry->num_objects; i++) {
                geometry->objects[i] = mli_Object_init();
                geometry->object_names[i] = mli_String_init();
        }
        return 1;
chk_error:
        mliGeometry_free_objects(geometry);
        return 0;
}

int mliGeometry_malloc_references(
        struct mliGeometry *geometry,
        const uint32_t num_robjects)
{
        geometry->num_robjects = num_robjects;
        chk_malloc(geometry->robjects, uint32_t, geometry->num_robjects);
        chk_malloc(geometry->robject_ids, uint32_t, geometry->num_robjects);
        chk_malloc(
                geometry->robject2root,
                struct mli_HomTraComp,
                geometry->num_robjects);
        return 1;
chk_error:
        mliGeometry_free_references(geometry);
        return 0;
}

int mliGeometry_malloc(
        struct mliGeometry *geometry,
        const uint32_t num_objects,
        const uint32_t num_robjects)
{
        mliGeometry_free(geometry);
        chk(mliGeometry_malloc_objects(geometry, num_objects));
        chk(mliGeometry_malloc_references(geometry, num_robjects));
        return 1;
chk_error:
        mliGeometry_free(geometry);
        return 0;
}

void mliGeometry_info_fprint(FILE *f, const struct mliGeometry *geometry)
{
        uint32_t rob, i;
        fprintf(f, "geometry\n");
        fprintf(f, "--------\n");
        fprintf(f, "\n");
        fprintf(f, "    objects\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%5s ", "obj");
        fprintf(f, "%24s ", "name");
        fprintf(f, "%8s ", "#v");
        fprintf(f, "%8s ", "#vn");
        fprintf(f, "%8s ", "#f");
        fprintf(f, "%8s ", "#mtl");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        for (i = 0; i < geometry->num_objects; i++) {
                fprintf(f, "    ");
                fprintf(f, "%5d ", i);
                fprintf(f, "%24s ", geometry->object_names[i].array);
                fprintf(f, "%8d ", geometry->objects[i].num_vertices);
                fprintf(f, "%8d ", geometry->objects[i].num_vertex_normals);
                fprintf(f, "%8d ", geometry->objects[i].num_faces);
                fprintf(f, "%8d ", geometry->objects[i].num_materials);
                fprintf(f, "\n");
        }
        fprintf(f, "\n");

        fprintf(f, "\n");
        fprintf(f, "    object-references\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%6s ", "ref");
        fprintf(f, "%6s ", "obj");
        fprintf(f, "%6s ", "id");
        fprintf(f, "%20s ", "translation(xyz)/m");
        fprintf(f, "%28s ", "quarternion(xyz;w)");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        for (rob = 0; rob < geometry->num_robjects; rob++) {
                fprintf(f, "    ");
                fprintf(f, "%6d ", rob);
                fprintf(f, "%6d ", geometry->robjects[rob]);
                fprintf(f, "%6d ", geometry->robject_ids[rob]);
                fprintf(f,
                        "% 6.1f,",
                        geometry->robject2root[rob].translation.x);
                fprintf(f,
                        "% 6.1f,",
                        geometry->robject2root[rob].translation.y);
                fprintf(f,
                        "% 6.1f ",
                        geometry->robject2root[rob].translation.z);
                fprintf(f, " ");
                fprintf(f, "% 6.1f,", geometry->robject2root[rob].rotation.x);
                fprintf(f, "% 6.1f,", geometry->robject2root[rob].rotation.y);
                fprintf(f, "% 6.1f;", geometry->robject2root[rob].rotation.z);
                fprintf(f, "% 6.1f ", geometry->robject2root[rob].rotation.w);
                fprintf(f, "\n");
        }
}

int mliGeometry_warn_objects(const struct mliGeometry *geometry)
{
        uint32_t o;
        for (o = 0; o < geometry->num_objects; o++) {
                uint32_t v, vn, mtl;
                chk_msg(mli_Object_num_unused(
                                &(geometry->objects)[o], &v, &vn, &mtl),
                        "Can't estimate num unused v/vn/mtl in object.");
                if (v > 0 || vn > 0 || mtl > 0) {
                        fprintf(stderr,
                                "[WARNING] Object '%s' at [%u] ",
                                geometry->object_names[o].array,
                                o);
                }
                if (v > 0) {
                        fprintf(stderr, "has %u unused vertices (v).\n", v);
                }
                if (vn > 0) {
                        fprintf(stderr,
                                "has %u unused vertex-normals (vn).\n",
                                vn);
                }
                if (mtl > 0) {
                        fprintf(stderr,
                                "has %u unused materials (mtl).\n",
                                mtl);
                }
        }

        return 1;
chk_error:
        return 0;
}
