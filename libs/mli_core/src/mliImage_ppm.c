/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliImage_ppm.h"
#include "../../chk_debug/src/chk_debug.h"

int mliImage_malloc_fread(struct mliImage *img, FILE *f)
{
        char line[1024];
        uint32_t num_commen_lines = 0;
        uint32_t num_cols;
        uint32_t num_rows;
        uint32_t col;
        uint32_t row;
        chk_msg(fgets(line, 1024, f), "Can't read header-line.") chk_msg(
                strcmp(line, "P6\n") == 0, "Expected starts with 'P6'.");
        while (1) {
                chk_msg(num_commen_lines < 1024, "Expected < 1024 lines.");
                chk_msg(fgets(line, 1024, f), "Can't read header-line.");
                if (line[0] == '#') {
                        num_commen_lines += 1u;
                } else {
                        break;
                }
        }
        num_cols = atoi(line);
        chk_msg(fgets(line, 1024, f), "Can't read header-line.");
        num_rows = atoi(line);
        chk_msg(fgets(line, 1024, f), "Can't read header-line.");
        chk_msg(strcmp(line, "255\n") == 0, "Expected 8bit range '255'.");
        chk_mem(mliImage_malloc(img, num_cols, num_rows));
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        uint8_t r, g, b;
                        struct mliColor color;
                        chk_fread(&r, sizeof(uint8_t), 1u, f);
                        chk_fread(&g, sizeof(uint8_t), 1u, f);
                        chk_fread(&b, sizeof(uint8_t), 1u, f);
                        color.r = (float)r;
                        color.g = (float)g;
                        color.b = (float)b;
                        mliImage_set(img, col, row, color);
                }
        }
        return 1;
error:
        mliImage_free(img);
        return 0;
}

int mliImage_malloc_from_path(struct mliImage *img, const char *path)
{
        FILE *f;
        f = fopen(path, "rb");
        chk_msgf(f, ("Can't open path '%s'.", path));
        chk_msg(mliImage_malloc_fread(img, f), "Can't read ppm from file.");
        chk_msg(!feof(f), "Unexpected end-of-file.");
        chk_msg(!ferror(f), "File error.");
        fclose(f);
        return 1;
error:
        mliImage_free(img);
        fclose(f);
        return 0;
}

int mliImage_fwrite(const struct mliImage *img, FILE *f)
{

        uint32_t col;
        uint32_t row;
        chk(fprintf(f, "P6\n"));
        chk(fprintf(f, "# merlict_c89\n"));
        chk(
                fprintf(f,
                        "# MLI_VERSION %d.%d.%d\n",
                        MLI_VERSION_MAYOR,
                        MLI_VERSION_MINOR,
                        MLI_VERSION_PATCH));
        chk(fprintf(f, "%d\n", img->num_cols));
        chk(fprintf(f, "%d\n", img->num_rows));
        chk(fprintf(f, "255\n"));
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        struct mliColor color = mliImage_at(img, col, row);
                        struct mliColor out = mliColor_truncate_to_uint8(color);
                        uint8_t r = (uint8_t)out.r;
                        uint8_t g = (uint8_t)out.g;
                        uint8_t b = (uint8_t)out.b;
                        chk_fwrite(&r, sizeof(uint8_t), 1u, f);
                        chk_fwrite(&g, sizeof(uint8_t), 1u, f);
                        chk_fwrite(&b, sizeof(uint8_t), 1u, f);
                }
        }
        return 1;
error:
        return 0;
}

int mliImage_write_to_path(const struct mliImage *img, const char *path)
{
        FILE *f;
        f = fopen(path, "wb");
        chk_msgf(f, ("Can't open path '%s'.", path));
        chk_msg(mliImage_fwrite(img, f), "Can't write ppm to file.");
        chk_msg(!feof(f), "Unexpected end-of-file.");
        chk_msg(!ferror(f), "File error.");
        fclose(f);
        return 1;
error:
        fclose(f);
        return 0;
}
