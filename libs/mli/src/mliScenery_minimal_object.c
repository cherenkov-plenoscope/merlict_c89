/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_minimal_object.h"
#include "../../chk/src/chk.h"
#include "mliColor.h"
#include "mli_random.h"
#include "../../mtl/src/prng.h"
#include "../../mtl/src/cstr.h"
#include "mliObject_wavefront.h"
#include "mliScenery_valid.h"

int mliSurface_malloc_random_phong(
        struct mliSurface *srf,
        struct mtl_Prng *prng)
{
        struct mtl_prng_UniformRange uniform_range;
        struct mliColor color;

        mliSurface_free(srf);
        uniform_range.start = 0.0;
        uniform_range.range = 1.0;
        color = mliColor_set(
                mtl_prng_draw_uniform(uniform_range, prng),
                mtl_prng_draw_uniform(uniform_range, prng),
                mtl_prng_draw_uniform(uniform_range, prng));

        /* r: 600nm
         * g: 550nm
         * b: 450nm
         */

        srf->material = MLI_MATERIAL_PHONG;

        /* no specular reflection */
        chk(mliFunc_malloc(&srf->specular_reflection, 2));
        srf->specular_reflection.x[0] = 200e-9;
        srf->specular_reflection.y[0] = 0.0;
        srf->specular_reflection.x[1] = 1200e-9;
        srf->specular_reflection.y[1] = 0.0;

        /* only diffuse reflection */
        chk(mliFunc_malloc(&srf->diffuse_reflection, 11));
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
        struct mtl_Prng prng = mtl_Prng_init_MT19937(1u);
        struct mtl_IO str = mtl_IO_init();
        struct mliMaterialsCapacity mtlcap = mliMaterialsCapacity_init();

        mliScenery_free(scenery);

        chk_msg(mliGeometry_malloc(&scenery->geometry, 1u, 1u),
                "Failed to malloc geometry.");

        /* set object */
        chk_msg(mtl_IO_write_from_path(&str, path), "Failed to read file.");
        chk_msg(mtl_cstr_assert_only_NUL_LF_TAB_controls((char *)str.cstr),
                "Expected object-wavefront file to be free of "
                "control characters, except [NUL, TAB, LF].");
        chk_msg(mliObject_malloc_from_wavefront(
                        &scenery->geometry.objects[0], (char *)str.cstr),
                "Failed to malloc wavefront-object from string.");
        mtl_IO_free(&str);
        sprintf(scenery->geometry.object_names[0].cstr, "default-object");

        /* set reference */
        scenery->geometry.robjects[0] = 0u;
        scenery->geometry.robject_ids[0] = 0u;
        scenery->geometry.robject2root[0] = mliHomTraComp_set(
                mliVec_init(0.0, 0.0, 0.0),
                mliQuaternion_set_tait_bryan(0.0, 0.0, 0.0));

        /* materials */
        total_num_boundary_layers = scenery->geometry.objects[0].num_materials;

        mtlcap.num_media = 1u;
        mtlcap.num_boundary_layers = total_num_boundary_layers;
        mtlcap.num_surfaces = total_num_boundary_layers;

        chk_msg(mliMaterials_malloc(&scenery->materials, mtlcap),
                "Failed to malloc materials.");

        sprintf(scenery->materials.medium_names[0].cstr, "vacuum");

        chk(mliFunc_malloc(&scenery->materials.media[0].refraction, 2));
        scenery->materials.media[0].refraction.x[0] = 200e-9;
        scenery->materials.media[0].refraction.x[1] = 1200e-9;
        scenery->materials.media[0].refraction.y[0] = 1.0;
        scenery->materials.media[0].refraction.y[1] = 1.0;

        chk(mliFunc_malloc(&scenery->materials.media[0].absorbtion, 2));
        scenery->materials.media[0].absorbtion.x[0] = 200e-9;
        scenery->materials.media[0].absorbtion.x[1] = 1200e-9;
        scenery->materials.media[0].absorbtion.y[0] = 0.0;
        scenery->materials.media[0].absorbtion.y[1] = 0.0;

        for (i = 0u; i < total_num_boundary_layers; i++) {
                chk_msg(mliSurface_malloc_random_phong(
                                &scenery->materials.surfaces[i], &prng),
                        "Can't draw random phong surface.");

                sprintf(scenery->materials.surface_names[i].cstr,
                        "surface_%06u",
                        i);

                scenery->materials.boundary_layers[i].inner.medium = 0u;
                scenery->materials.boundary_layers[i].outer.medium = 0u;
                scenery->materials.boundary_layers[i].inner.surface = i;
                scenery->materials.boundary_layers[i].outer.surface = i;
                sprintf(scenery->materials.boundary_layer_names[i].cstr,
                        "boundary_layer_%06u",
                        i);
        }

        chk_msg(mliGeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        scenery->geometry.num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        /* set map */
        for (i = 0u; i < total_num_boundary_layers; i++) {
                mliGeometryToMaterialMap_set(&scenery->geomap, 0u, i, i);
        }

        chk_msg(mliAccelerator_malloc_from_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mliScenery_valid(scenery), "Expected scenery to be valid.");
        return 1;
chk_error:
        return 0;
}
