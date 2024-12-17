/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "accelerator.h"
#include "../object/object_AABB.h"
#include "../mli/mliGeometryAndAccelerator.h"
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_Accelerator mli_Accelerator_init(void)
{
        struct mli_Accelerator out;

        out.num_objects = 0u;
        out.object_octrees = NULL;

        out.num_robjects = 0u;
        out.robject_aabbs = NULL;

        out.scenery_octree = mli_OcTree_init();

        return out;
}

void mli_Accelerator_free(struct mli_Accelerator *self)
{
        uint32_t obj;

        mli_OcTree_free(&self->scenery_octree);

        for (obj = 0; obj < self->num_objects; obj++) {
                mli_OcTree_free(&self->object_octrees[obj]);
        }
        free(self->object_octrees);

        free(self->robject_aabbs);
        (*self) = mli_Accelerator_init();
}

int mli_Accelerator_malloc(
        struct mli_Accelerator *self,
        const uint32_t num_objects,
        const uint32_t num_robjects)
{
        uint32_t obj, rob;
        mli_Accelerator_free(self);

        self->num_objects = num_objects;
        chk_malloc(self->object_octrees, struct mli_OcTree, self->num_objects);
        for (obj = 0; obj < self->num_objects; obj++) {
                self->object_octrees[obj] = mli_OcTree_init();
        }

        self->num_robjects = num_robjects;
        chk_malloc(self->robject_aabbs, struct mli_AABB, self->num_robjects);
        for (rob = 0; rob < self->num_robjects; rob++) {
                self->robject_aabbs[rob] = mli_AABB_set(
                        mli_Vec_init(0.0, 0.0, 0.0),
                        mli_Vec_init(0.0, 0.0, 0.0));
        }

        return 1;
chk_error:
        return 0;
}

int mli_Accelerator_set_robject_aabbs(
        struct mli_Accelerator *self,
        const struct mli_Geometry *geometry)
{
        uint32_t rob;
        chk_msg(self->num_robjects == geometry->num_robjects,
                "Expected num_robjects to be equal, but its not.");

        for (rob = 0; rob < self->num_robjects; rob++) {
                const uint32_t robject = geometry->robjects[rob];
                self->robject_aabbs[rob] = mli_Object_aabb(
                        &geometry->objects[robject],
                        mli_HomTraComp_from_compact(
                                geometry->robject2root[rob]));
        }
        return 1;
chk_error:
        return 0;
}

int mli_Accelerator_set_object_octrees(
        struct mli_Accelerator *self,
        const struct mli_Geometry *geometry)
{
        uint32_t obj;
        chk_msg(self->num_objects == geometry->num_objects,
                "Expected num_objects to be equal, but its not.");

        for (obj = 0; obj < self->num_objects; obj++) {
                chk_msg(mli_OcTree_malloc_from_object_wavefront(
                                &self->object_octrees[obj],
                                &geometry->objects[obj]),
                        "Failed to setup mli_OcTree for object-wavefront.");
        }

        return 1;
chk_error:
        return 0;
}

int mli_Accelerator_malloc_from_Geometry(
        struct mli_Accelerator *self,
        const struct mli_Geometry *geometry)
{
        struct mli_AABB outermost_aabb;
        struct mli_GeometryAndAccelerator accgeo;
        accgeo.accelerator = self;
        accgeo.geometry = geometry;

        chk_msg(mli_Accelerator_malloc(
                        self, geometry->num_objects, geometry->num_robjects),
                "Failed to malloc mli_Accelerator from mli_Geometry's "
                "num_robjects");

        chk_msg(mli_Accelerator_set_robject_aabbs(self, geometry),
                "Failed to set AABBs of robjects.");

        chk_msg(mli_Accelerator_set_object_octrees(self, geometry),
                "Failed to setup object octrees.");

        outermost_aabb = mli_Accelerator_outermost_aabb(self);

        chk_msg(mli_OcTree_malloc_from_Geometry(
                        &self->scenery_octree, &accgeo, outermost_aabb),
                "Failed to set up octree across all robjects in geometry.");

        return 1;
chk_error:
        return 0;
}

void mli_Accelerator_info_fprint(FILE *f, const struct mli_Accelerator *self)
{
        uint32_t rob, i;
        fprintf(f, "accelerator\n");
        fprintf(f, "-----------\n");
        fprintf(f, "\n");
        fprintf(f, "    object-references bounding-boxes (AABB)s:\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");
        fprintf(f, "    ");
        fprintf(f, "%5s ", "ref");
        fprintf(f, "%9s ", "-x/m");
        fprintf(f, "%9s ", "-y/m");
        fprintf(f, "%9s ", "-z/m");
        fprintf(f, "%9s ", "+x/m");
        fprintf(f, "%9s ", "+y/m");
        fprintf(f, "%9s ", "+z/m");
        fprintf(f, "\n");
        fprintf(f, "    ");
        for (i = 0; i < 70; i++) {
                fprintf(f, "-");
        }
        fprintf(f, "\n");

        for (rob = 0; rob < self->num_robjects; rob++) {
                fprintf(f, "    ");
                fprintf(f, "%5d ", rob);
                fprintf(f, "%9.1f ", self->robject_aabbs[rob].lower.x);
                fprintf(f, "%9.1f ", self->robject_aabbs[rob].lower.y);
                fprintf(f, "%9.1f ", self->robject_aabbs[rob].lower.z);
                fprintf(f, "%9.1f ", self->robject_aabbs[rob].upper.x);
                fprintf(f, "%9.1f ", self->robject_aabbs[rob].upper.y);
                fprintf(f, "%9.1f ", self->robject_aabbs[rob].upper.z);
                fprintf(f, "\n");
        }
}

struct mli_AABB mli_Accelerator_outermost_aabb(
        const struct mli_Accelerator *self)
{
        uint32_t rob;
        struct mli_AABB aabb;
        if (self->num_robjects == 0) {
                aabb.lower =
                        mli_Vec_init(MLI_MATH_NAN, MLI_MATH_NAN, MLI_MATH_NAN);
                aabb.upper =
                        mli_Vec_init(MLI_MATH_NAN, MLI_MATH_NAN, MLI_MATH_NAN);
                return aabb;
        }
        aabb.lower = self->robject_aabbs[0].lower;
        aabb.upper = self->robject_aabbs[0].upper;
        for (rob = 0; rob < self->num_robjects; rob++) {
                aabb = mli_AABB_outermost(aabb, self->robject_aabbs[rob]);
        }
        return aabb;
}
