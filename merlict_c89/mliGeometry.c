/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry.h"
#include "mli_debug.h"

struct mliGeometry mliGeometry_init(void)
{
        struct mliGeometry scn;
        scn.num_objects = 0u;
        scn.objects = NULL;

        scn.num_robjects = 0u;
        scn.robjects = NULL;
        scn.robject_ids = NULL;

        scn.robject_boundary_layers = NULL;
        scn.robject2root = NULL;
        return scn;
}

void mliGeometry_free(struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_objects; i++) {
                mliObject_free(&(geometry->objects[i]));
        }
        free(geometry->objects);

        free(geometry->robjects);
        free(geometry->robject_ids);
        free(geometry->robject_boundary_layers);
        free(geometry->robject2root);

        (*geometry) = mliGeometry_init();
}

int mliGeometry_malloc(
        struct mliGeometry *geometry,
        const uint32_t num_objects,
        const uint32_t num_robjects)
{
        uint32_t i;
        mliGeometry_free(geometry);

        geometry->num_objects = num_objects;
        mli_malloc(geometry->objects, struct mliObject, geometry->num_objects);
        for (i = 0; i < geometry->num_objects; i++) {
                geometry->objects[i] = mliObject_init();
        }

        geometry->num_robjects = num_robjects;
        mli_malloc(geometry->robjects, uint32_t, geometry->num_robjects);
        mli_malloc(geometry->robject_ids, uint32_t, geometry->num_robjects);
        mli_malloc(
                geometry->robject_boundary_layers,
                struct mliBoundaryLayer,
                geometry->num_robjects);
        mli_malloc(
                geometry->robject2root,
                struct mliHomTraComp,
                geometry->num_robjects);

        return 1;
error:
        mliGeometry_free(geometry);
        return 0;
}

void mliGeometry_info_fprint(FILE *f, const struct mliGeometry *geometry)
{
        uint32_t rob;
        fprintf(f, "__mliGeometry__ [num obj: %u, num obj-refs: %u]\n",
                geometry->num_objects, geometry->num_robjects);
        fprintf(f,
                " rob | obj | id  | translation(xyz) | rotation(xyz;w)  |\n");
        fprintf(f,
                "-----|-----|-----|------------------|------------------|\n");
        for (rob = 0; rob < geometry->num_robjects; rob++) {
                fprintf(f,
                        "% 4d |% 4d |% 4d |"
                        "(%.1f, %.1f, %.1f)|"
                        "(%.1f, %.1f, %.1f; %.1f)|"
                        "\n",
                        rob,
                        geometry->robjects[rob],
                        geometry->robject_ids[rob],
                        geometry->robject2root[rob].translation.x,
                        geometry->robject2root[rob].translation.y,
                        geometry->robject2root[rob].translation.z,
                        geometry->robject2root[rob].rotation.x,
                        geometry->robject2root[rob].rotation.y,
                        geometry->robject2root[rob].rotation.z,
                        geometry->robject2root[rob].rotation.w);
        }
}
