/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry.h"
#include "mli_debug.h"

void _mliGeometry_init_objects(struct mliGeometry *geometry)
{
        geometry->num_objects = 0u;
        geometry->objects = NULL;
        geometry->object_names = NULL;
}

void _mliGeometry_init_references(struct mliGeometry *geometry)
{
        geometry->num_robjects = 0u;
        geometry->robjects = NULL;
        geometry->robject_ids = NULL;

        geometry->robject_boundary_layers = NULL;
        geometry->robject2root = NULL;
}

struct mliGeometry mliGeometry_init(void)
{
        struct mliGeometry geometry;
        _mliGeometry_init_objects(&geometry);
        _mliGeometry_init_references(&geometry);
        return geometry;
}

void _mliGeometry_free_objects(struct mliGeometry *geometry)
{
        uint32_t i;
        for (i = 0; i < geometry->num_objects; i++) {
                mliObject_free(&(geometry->objects[i]));
        }
        free(geometry->objects);
        free(geometry->object_names);
        _mliGeometry_init_objects(geometry);
}

void _mliGeometry_free_references(struct mliGeometry *geometry)
{
        free(geometry->robjects);
        free(geometry->robject_ids);
        free(geometry->robject_boundary_layers);
        free(geometry->robject2root);
        _mliGeometry_init_references(geometry);
}

void mliGeometry_free(struct mliGeometry *geometry)
{
        _mliGeometry_free_objects(geometry);
        _mliGeometry_free_references(geometry);
        (*geometry) = mliGeometry_init();
}

int _mliGeometry_malloc_objects(
        struct mliGeometry *geometry,
        const uint32_t num_objects)
{
        uint32_t i;
        geometry->num_objects = num_objects;
        mli_malloc(geometry->objects, struct mliObject, geometry->num_objects);
        mli_malloc(geometry->object_names, struct mliName, geometry->num_objects);
        for (i = 0; i < geometry->num_objects; i++) {
                geometry->objects[i] = mliObject_init();
                geometry->object_names[i] = mliName_init();
        }
        return 1;
error:
        _mliGeometry_free_objects(geometry);
        return 0;
}

int _mliGeometry_malloc_references(
        struct mliGeometry *geometry,
        const uint32_t num_robjects)
{
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
        _mliGeometry_free_references(geometry);
        return 0;
}

int mliGeometry_malloc(
        struct mliGeometry *geometry,
        const uint32_t num_objects,
        const uint32_t num_robjects)
{
        mliGeometry_free(geometry);
        mli_c(_mliGeometry_malloc_objects(geometry, num_objects));
        mli_c(_mliGeometry_malloc_references(geometry, num_robjects));
        return 1;
error:
        mliGeometry_free(geometry);
        return 0;
}

void mliGeometry_info_fprint(FILE *f, const struct mliGeometry *geometry)
{
        uint32_t rob, i;
        fprintf(f, "Geometry:\n");
        fprintf(f, "%*sobjects:\n", 4, "");

        fprintf(f, "%*s obj  name                                 #v    #vn     #f   #mtl\n", 8, "");
        fprintf(f, "%*s------------------------------------------------------------------\n", 8, "");
        for (i = 0; i < geometry->num_objects; i++) {
                fprintf(f, "%*s% 4d  ", 8, "", i);
                fprintf(f, "%-32s  ", geometry->object_names[i].c_str);
                fprintf(f, "% 5d  % 5d  % 5d  % 5d",
                        geometry->objects[i].num_vertices,
                        geometry->objects[i].num_vertex_normals,
                        geometry->objects[i].num_faces,
                        geometry->objects[i].num_materials);
                fprintf(f, "\n");
        }
        fprintf(f, "\n");
        fprintf(f, "%*sobject-references:\n", 4, "");
        fprintf(f,
                "%*s ref   obj    id   translation(xyz)/m     rot. quarternion(xyz;w)\n", 8, "");
        fprintf(f, "%*s------------------------------------------------------------"
                "-----\n", 8, "");
        for (rob = 0; rob < geometry->num_robjects; rob++) {
                fprintf(f,
                        "%*s% 4d  % 4d  % 4d  "
                        "(% 5.1f, % 5.1f, % 5.1f)  "
                        "(% 1.1f, % 1.1f, % 1.1f; % 1.1f) "
                        "\n",
                        8,
                        "",
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
        fprintf(f, "\n");
}
