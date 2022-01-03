/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliImage_ppm.h"

int mliImage_malloc_ppm_from_file(struct mliImage *img, FILE *fin)
{
        char line[1024];
        uint32_t num_commen_lines = 0;
        uint32_t num_cols;
        uint32_t num_rows;
        uint32_t col;
        uint32_t row;
        chk_msg(fgets(line, 1024, fin), "Can not read header-line.") chk_msg(
                strcmp(line, "P6\n") == 0, "Expected starts with 'P6'.");
        while (1) {
                chk_msg(num_commen_lines < 1024, "Expected < 1024 lines.");
                chk_msg(fgets(line, 1024, fin), "Can not read header-line.");
                if (line[0] == '#') {
                        num_commen_lines += 1u;
                } else {
                        break;
                }
        }
        num_cols = atoi(line);
        chk_msg(fgets(line, 1024, fin), "Can not read header-line.");
        num_rows = atoi(line);
        chk_msg(fgets(line, 1024, fin), "Can not read header-line.");
        chk_msg(strcmp(line, "255\n") == 0, "Expected 8bit range '255'.");
        chk_mem(mliImage_malloc(img, num_cols, num_rows));
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        uint8_t r, g, b;
                        struct mliColor color;
                        chk_fread(&r, sizeof(uint8_t), 1u, fin);
                        chk_fread(&g, sizeof(uint8_t), 1u, fin);
                        chk_fread(&b, sizeof(uint8_t), 1u, fin);
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

int mliImage_malloc_ppm_from_path(struct mliImage *img, const char *path)
{
        FILE *fin;
        fin = fopen(path, "rb");
        chk_msg(fin, "Can not open ppm.");
        chk_msg(mliImage_malloc_ppm_from_file(img, fin),
                "Can't read ppm from file.");
        chk_msg(!feof(fin), "Unexpected end-of-file.");
        chk_msg(!ferror(fin), "File error.");
        fclose(fin);
        return 1;
error:
        mliImage_free(img);
        fclose(fin);
        return 0;
}

int mliImage_write_ppm_to_file(const struct mliImage *img, FILE *f)
{

        uint32_t col;
        uint32_t row;
        chk(fprintf(f, "P6\n"));
        chk(fprintf(f, "# merlict_c89\n"));
        chk(fprintf(f,
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

int mliImage_write_ppm_to_path(const struct mliImage *img, const char *path)
{
        FILE *fout;
        fout = fopen(path, "w");
        chk_msg(fout, "Can not open ppm-image for writing.");
        chk_msg(mliImage_write_ppm_to_file(img, fout),
                "Can't write ppm to file.");
        chk_msg(!feof(fout), "Unexpected end-of-file.");
        chk_msg(!ferror(fout), "File error.");
        fclose(fout);
        return 1;
error:
        fclose(fout);
        return 0;
}
