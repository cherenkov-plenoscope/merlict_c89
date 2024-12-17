/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_minimal_object.h"
#include "../chk/chk.h"
#include "../color/color.h"
#include "../vec/vec_random.h"
#include "../prng/prng.h"
#include "../cstr/cstr.h"
#include "../object/object_wavefront.h"
#include "mliScenery_valid.h"

int mli_Surface_malloc_random_phong(
        struct mli_Surface *srf,
        struct mli_Prng *prng)
{
        struct mli_prng_UniformRange uniform_range;
        struct mli_Color color;

        mli_Surface_free(srf);
        uniform_range.start = 0.0;
        uniform_range.range = 1.0;
        color = mli_Color_set(
                mli_prng_draw_uniform(uniform_range, prng),
                mli_prng_draw_uniform(uniform_range, prng),
                mli_prng_draw_uniform(uniform_range, prng));

        /* r: 600nm
         * g: 550nm
         * b: 450nm
         */

        srf->material = MLI_SURFACE_PHONG;

        /* no specular reflection */
        chk(mli_Func_malloc(&srf->specular_reflection, 2));
        srf->specular_reflection.x[0] = 200e-9;
        srf->specular_reflection.y[0] = 0.0;
        srf->specular_reflection.x[1] = 1200e-9;
        srf->specular_reflection.y[1] = 0.0;

        /* only diffuse reflection */
        chk(mli_Func_malloc(&srf->diffuse_reflection, 11));
        srf->diffuse_reflection.x[0] = 200e-9;
        srf->diffuse_reflection.y[0] = 0.0;

        /* blue peak */
        srf->diffuse_reflection.x[1] = 400e-9;
        srf->diffuse_reflection.y[1] = 0.0;
        srf->diffuse_reflection.x[2] = 450e-9;
        srf->diffuse_reflection.y[2] = color.b;
        srf->diffuse_reflection.x[3] = 500e-9;
        srf->diffuse_reflection.y[3] = 0.0;

        /* green peak */
        srf->diffuse_reflection.x[4] = 501e-9;
        srf->diffuse_reflection.y[4] = 0.0;
        srf->diffuse_reflection.x[5] = 550e-9;
        srf->diffuse_reflection.y[5] = color.g;
        srf->diffuse_reflection.x[6] = 575e-9;
        srf->diffuse_reflection.y[6] = 0.0;

        /* red peak */
        srf->diffuse_reflection.x[7] = 576e-9;
        srf->diffuse_reflection.y[7] = 0.0;
        srf->diffuse_reflection.x[8] = 600e-9;
        srf->diffuse_reflection.y[8] = color.r;
        srf->diffuse_reflection.x[9] = 650e-9;
        srf->diffuse_reflection.y[9] = 0.0;

        srf->diffuse_reflection.x[10] = 1200e-9;
        srf->diffuse_reflection.y[10] = 0.0;

        return 1;
chk_error:
        return 0;
}

int mliScenery_malloc_minimal_from_wavefront(
        struct mliScenery *scenery,
        const char *path)
{
        uint32_t i, total_num_boundary_layers;
        struct mli_Prng prng = mli_Prng_init_MT19937(1u);
        struct mli_IO str = mli_IO_init();
        struct mli_MaterialsCapacity mtlcap = mli_MaterialsCapacity_init();
        struct mli_String _path = mli_String_init();
        struct mli_String _mode = mli_String_init();

        mliScenery_free(scenery);

        chk_msg(mli_Geometry_malloc(&scenery->geometry, 1u, 1u),
                "Failed to malloc geometry.");

        /* set object */
        chk(mli_String_from_cstr(&_path, path));
        chk(mli_String_from_cstr(&_mode, "r"));
        chk_msg(mli_IO_open_file(&str, &_path, &_mode), "Failed to open file.");
        chk_msg(mli_Object_malloc_from_wavefront(
                        &scenery->geometry.objects[0], &str),
                "Failed to malloc wavefront-object from string.");
        mli_IO_close(&str);

        mli_String_free(&_path);
        mli_String_free(&_mode);

        chk(mli_String_from_cstr(
                &scenery->geometry.object_names[0], "default-object"));

        /* set reference */
        scenery->geometry.robjects[0] = 0u;
        scenery->geometry.robject_ids[0] = 0u;
        scenery->geometry.robject2root[0] = mli_HomTraComp_set(
                mli_Vec_init(0.0, 0.0, 0.0),
                mli_Quaternion_set_tait_bryan(0.0, 0.0, 0.0));

        /* materials */
        total_num_boundary_layers = scenery->geometry.objects[0].num_materials;

        mtlcap.num_media = 1u;
        mtlcap.num_boundary_layers = total_num_boundary_layers;
        mtlcap.num_surfaces = total_num_boundary_layers;

        chk_msg(mli_Materials_malloc(&scenery->materials, mtlcap),
                "Failed to malloc materials.");

        chk(mli_String_from_cstr(
                &scenery->materials.medium_names[0], "vacuum"));

        chk(mli_Func_malloc(&scenery->materials.media[0].refraction, 2));
        scenery->materials.media[0].refraction.x[0] = 200e-9;
        scenery->materials.media[0].refraction.x[1] = 1200e-9;
        scenery->materials.media[0].refraction.y[0] = 1.0;
        scenery->materials.media[0].refraction.y[1] = 1.0;

        chk(mli_Func_malloc(&scenery->materials.media[0].absorbtion, 2));
        scenery->materials.media[0].absorbtion.x[0] = 200e-9;
        scenery->materials.media[0].absorbtion.x[1] = 1200e-9;
        scenery->materials.media[0].absorbtion.y[0] = 0.0;
        scenery->materials.media[0].absorbtion.y[1] = 0.0;

        for (i = 0u; i < total_num_boundary_layers; i++) {
                chk_msg(mli_Surface_malloc_random_phong(
                                &scenery->materials.surfaces[i], &prng),
                        "Can't draw random phong surface.");

                chk(mli_String_from_cstr_fromat(
                        &scenery->materials.surface_names[i],
                        "surface_%06u",
                        i));

                scenery->materials.boundary_layers[i].inner.medium = 0u;
                scenery->materials.boundary_layers[i].outer.medium = 0u;
                scenery->materials.boundary_layers[i].inner.surface = i;
                scenery->materials.boundary_layers[i].outer.surface = i;

                chk(mli_String_from_cstr_fromat(
                        &scenery->materials.boundary_layer_names[i],
                        "boundary_layer_%06u",
                        i));
        }

        chk_msg(mli_GeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        scenery->geometry.num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        /* set map */
        for (i = 0u; i < total_num_boundary_layers; i++) {
                mli_GeometryToMaterialMap_set(&scenery->geomap, 0u, i, i);
        }

        chk_msg(mli_Accelerator_malloc_from_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mliScenery_valid(scenery), "Expected scenery to be valid.");
        return 1;
chk_error:
        return 0;
}
