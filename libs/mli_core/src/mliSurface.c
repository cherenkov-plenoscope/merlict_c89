/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface.h"
#include "chk.h"

struct mliSurface mliSurface_init(void)
{
        struct mliSurface surface;
        surface.material = MLI_MATERIAL_PHONG;
        surface.specular_reflection = mliFunc_init();
        surface.diffuse_reflection = mliFunc_init();
        surface.color = mliColor_set(0.0, 0.0, 0.0);
        return surface;
}

void mliSurface_free(struct mliSurface *surface)
{
        mliFunc_free(&surface->specular_reflection);
        mliFunc_free(&surface->diffuse_reflection);
        (*surface) = mliSurface_init();
}

int mliSurface_malloc(
        struct mliSurface *surface,
        const uint32_t num_points_specular_reflection,
        const uint32_t num_points_diffuse_reflection)
{
        mliSurface_free(surface);
        chk(mliFunc_malloc(
                &surface->specular_reflection, num_points_specular_reflection));
        chk(mliFunc_malloc(
                &surface->diffuse_reflection, num_points_diffuse_reflection));
        return 1;
chk_error:
        return 0;
}

int mliSurface_equal(const struct mliSurface *a, const struct mliSurface *b)
{
        if (a->material != b->material)
                return 0;
        if (!mliFunc_equal(a->specular_reflection, b->specular_reflection))
                return 0;
        if (!mliFunc_equal(a->diffuse_reflection, b->diffuse_reflection))
                return 0;
        if (!mliColor_equal(a->color, b->color))
                return 0;
        return 1;
}

int mli_material_type_to_string(const uint32_t type, char *s)
{
        switch (type) {
        case MLI_MATERIAL_PHONG:
                sprintf(s, "Phong");
                break;
        case MLI_MATERIAL_TRANSPARENT:
                sprintf(s, "transparent");
                break;
        default:
                chk_bad("material-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_material_type_from_string(const char *s, uint32_t *id)
{
        if (0 == strcmp(s, "Phong")) {
                (*id) = MLI_MATERIAL_PHONG;
                return 1;
        } else if (0 == strcmp(s, "transparent")) {
                (*id) = MLI_MATERIAL_TRANSPARENT;
                return 1;
        } else {
                chk_bad("material-type-string is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mliSurface_fwrite(const struct mliSurface *srf, FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliSurface"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_fwrite(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mliFunc_fwrite(&srf->specular_reflection, f));
        chk(mliFunc_fwrite(&srf->diffuse_reflection, f));
        chk_fwrite(&srf->color, sizeof(struct mliColor), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mliSurface_malloc_fread(struct mliSurface *srf, FILE *f)
{
        struct mliMagicId magic;
        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk_msg(mliMagicId_has_word(&magic, "mliSurface"),
                "Expected MagicID 'mliSurface'.");
        mliMagicId_warn_version(&magic);

        chk_fread(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mliFunc_malloc_fread(&srf->specular_reflection, f));
        chk(mliFunc_malloc_fread(&srf->diffuse_reflection, f));
        chk_fread(&srf->color, sizeof(struct mliColor), 1u, f);

        return 1;
chk_error:
        return 0;
}
