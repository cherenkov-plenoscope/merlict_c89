/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry.h"
#include "../object/object_valid.h"
#include "../chk/chk.h"

void mli_Geometry_init_objects(struct mli_Geometry *self)
{
        self->num_objects = 0u;
        self->objects = NULL;
        self->object_names = NULL;
}

void mli_Geometry_init_references(struct mli_Geometry *self)
{
        self->num_robjects = 0u;
        self->robjects = NULL;
        self->robject_ids = NULL;
        self->robject2root = NULL;
}

struct mli_Geometry mli_Geometry_init(void)
{
        struct mli_Geometry out;
        mli_Geometry_init_objects(&out);
        mli_Geometry_init_references(&out);
        return out;
}

void mli_Geometry_free_objects(struct mli_Geometry *self)
{
        uint32_t i;
        for (i = 0; i < self->num_objects; i++) {
                mli_Object_free(&(self->objects[i]));
                mli_String_free(&(self->object_names[i]));
        }
        free(self->objects);
        free(self->object_names);
        mli_Geometry_init_objects(self);
}

void mli_Geometry_free_references(struct mli_Geometry *self)
{
        free(self->robjects);
        free(self->robject_ids);
        free(self->robject2root);
        mli_Geometry_init_references(self);
}

void mli_Geometry_free(struct mli_Geometry *self)
{
        mli_Geometry_free_objects(self);
        mli_Geometry_free_references(self);
        (*self) = mli_Geometry_init();
}

int mli_Geometry_malloc_objects(
        struct mli_Geometry *self,
        const uint32_t num_objects)
{
        uint32_t i;
        self->num_objects = num_objects;
        chk_malloc(self->objects, struct mli_Object, self->num_objects);
        chk_malloc(self->object_names, struct mli_String, self->num_objects);
        for (i = 0; i < self->num_objects; i++) {
                self->objects[i] = mli_Object_init();
                self->object_names[i] = mli_String_init();
        }
        return CHK_SUCCESS;
chk_error:
        mli_Geometry_free_objects(self);
        return CHK_FAIL;
}

int mli_Geometry_malloc_references(
        struct mli_Geometry *self,
        const uint32_t num_robjects)
{
        self->num_robjects = num_robjects;
        chk_malloc(self->robjects, uint32_t, self->num_robjects);
        chk_malloc(self->robject_ids, uint32_t, self->num_robjects);
        chk_malloc(
                self->robject2root, struct mli_HomTraComp, self->num_robjects);
        return CHK_SUCCESS;
chk_error:
        mli_Geometry_free_references(self);
        return CHK_FAIL;
}

int mli_Geometry_malloc(
        struct mli_Geometry *self,
        const uint32_t num_objects,
        const uint32_t num_robjects)
{
        mli_Geometry_free(self);
        chk(mli_Geometry_malloc_objects(self, num_objects));
        chk(mli_Geometry_malloc_references(self, num_robjects));
        return CHK_SUCCESS;
chk_error:
        mli_Geometry_free(self);
        return CHK_FAIL;
}

void mli_Geometry_info_fprint(FILE *f, const struct mli_Geometry *self)
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

        for (i = 0; i < self->num_objects; i++) {
                fprintf(f, "    ");
                fprintf(f, "%5d ", i);
                fprintf(f, "%24s ", self->object_names[i].array);
                fprintf(f, "%8d ", self->objects[i].num_vertices);
                fprintf(f, "%8d ", self->objects[i].num_vertex_normals);
                fprintf(f, "%8d ", self->objects[i].num_faces);
                fprintf(f, "%8d ", self->objects[i].num_materials);
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

        for (rob = 0; rob < self->num_robjects; rob++) {
                fprintf(f, "    ");
                fprintf(f, "%6d ", rob);
                fprintf(f, "%6d ", self->robjects[rob]);
                fprintf(f, "%6d ", self->robject_ids[rob]);
                fprintf(f, "% 6.1f,", self->robject2root[rob].translation.x);
                fprintf(f, "% 6.1f,", self->robject2root[rob].translation.y);
                fprintf(f, "% 6.1f ", self->robject2root[rob].translation.z);
                fprintf(f, " ");
                fprintf(f, "% 6.1f,", self->robject2root[rob].rotation.x);
                fprintf(f, "% 6.1f,", self->robject2root[rob].rotation.y);
                fprintf(f, "% 6.1f;", self->robject2root[rob].rotation.z);
                fprintf(f, "% 6.1f ", self->robject2root[rob].rotation.w);
                fprintf(f, "\n");
        }
}

int mli_Geometry_warn_objects(const struct mli_Geometry *self)
{
        uint32_t o;
        for (o = 0; o < self->num_objects; o++) {
                uint32_t v, vn, mtl;
                chk_msg(mli_Object_num_unused(
                                &(self->objects)[o], &v, &vn, &mtl),
                        "Can't estimate num unused v/vn/mtl in object.");
                if (v > 0 || vn > 0 || mtl > 0) {
                        fprintf(stderr,
                                "[WARNING] Object '%s' at [%u] ",
                                self->object_names[o].array,
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

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
