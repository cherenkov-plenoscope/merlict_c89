/**
 * Copyright (c) 2017 rxi
 * Copyright (c) 2019 Sebastian A. Mueller
 *                    Max-Planck-Institute for nuclear-physics, Heidelberg
 */

#include "tar.h"
#include <string.h>
#include <stddef.h>
#include "../cstr/cstr_numbers.h"

/*                             basics                                         */
/* ========================================================================== */

uint64_t mli_Tar_round_up(uint64_t n, uint64_t incr)
{
        return n + (incr - n % incr) % incr;
}

chk_rc mli_Tar_field_to_uint(
        uint64_t *out,
        const char *field,
        const uint64_t fieldsize)
{
        char buff[MLI_TAR_NAME_LENGTH] = {'\0'};
        chk(fieldsize < MLI_TAR_NAME_LENGTH);
        memcpy(buff, field, fieldsize);

        /* Take care of historic 'space' (32 decimal) termination */
        /* Convert all 'space' terminations to '\0' terminations. */

        if (buff[fieldsize - 1] == 32) {
                buff[fieldsize - 1] = 0;
        }
        if (buff[fieldsize - 2] == 32) {
                buff[fieldsize - 2] = 0;
        }

        chk(mli_cstr_to_uint64(out, buff, MLI_TAR_OCTAL));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_uint_to_field(
        const uint64_t val,
        char *field,
        const uint64_t fieldsize)
{
        chk(mli_cstr_print_uint64(
                val, field, fieldsize, MLI_TAR_OCTAL, fieldsize - 1));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_uint64_to_field12_2001star_base256(uint64_t val, char *field)
{
        uint8_t tmp[12];
        int64_t i = 0;
        for (i = 11; i > 0; i--) {
                tmp[i] = (uint8_t)(val % 256u);
                val = val / 256u;
        }

        chk_msg(val == 0u, "Expected value to be less than 256**11.");
        /* set highest bit in leftmost byte to 1 */
        tmp[0] = (uint8_t)128u;

        memcpy(field, tmp, 12);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_field12_to_uint64_2001star_base256(
        const char *field,
        uint64_t *val)
{
        uint8_t tmp[12];
        uint64_t i = 0u;
        const uint64_t powers[] = {
                0x100000000000000,
                0x1000000000000,
                0x10000000000,
                0x100000000,
                0x1000000,
                0x10000,
                0x100,
                0x1,
        };

        memcpy(tmp, field, 12);
        chk_msg(tmp[0] == 128u,
                "Expected field[0] == 128, indicating 256-base, 2001star.");
        chk_msg(tmp[1] == 0u,
                "Expected field[1] == 0, 256**10 exceeds uint64.");
        chk_msg(tmp[2] == 0u,
                "Expected field[2] == 0, 256**09 exceeds uint64.");
        chk_msg(tmp[3] == 0u,
                "Expected field[3] == 0, 256**08 exceeds uint64.");

        (*val) = 0u;
        for (i = 4; i < 12; i++) {
                (*val) = (*val) + powers[i - 4] * (uint64_t)tmp[i];
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

/*                               raw header                                   */
/* ========================================================================== */

uint64_t mli_TarRawHeader_checksum(const struct mli_TarRawHeader *rh)
{
        uint64_t i;
        unsigned char *p = (unsigned char *)rh;
        uint64_t res = 256;
        for (i = 0; i < offsetof(struct mli_TarRawHeader, checksum); i++) {
                res += p[i];
        }
        for (i = offsetof(struct mli_TarRawHeader, type); i < sizeof(*rh);
             i++) {
                res += p[i];
        }
        return res;
}

mli_bool mli_TarRawHeader_is_null(const struct mli_TarRawHeader *rh)
{
        uint64_t i = 0u;
        unsigned char *p = (unsigned char *)rh;
        for (i = 0; i < sizeof(struct mli_TarRawHeader); i++) {
                if (p[i] != '\0') {
                        return MLI_FALSE;
                }
        }
        return MLI_TRUE;
}

chk_rc mli_TarRawHeader_from_header(
        struct mli_TarRawHeader *rh,
        const struct mli_TarHeader *h)
{
        uint64_t chksum;

        /* Load header into raw header */
        memset(rh, 0, sizeof(*rh));
        chk_msg(mli_Tar_uint_to_field(h->mode, rh->mode, sizeof(rh->mode)),
                "bad mode");
        chk_msg(mli_Tar_uint_to_field(h->owner, rh->owner, sizeof(rh->owner)),
                "bad owner");
        if (h->size >= MLI_TAR_MAX_FILESIZE_OCTAL) {
                chk_msg(mli_Tar_uint64_to_field12_2001star_base256(
                                h->size, rh->size),
                        "bad size, mode: base-256");
        } else {
                chk_msg(mli_Tar_uint_to_field(
                                h->size, rh->size, sizeof(rh->size)),
                        "bad size, mode: base-octal");
        }
        chk_msg(mli_Tar_uint_to_field(h->mtime, rh->mtime, sizeof(rh->mtime)),
                "bad mtime");
        if (mli_Tar_is_known_file_type(h->type)) {
                rh->type = h->type;
        } else {
                rh->type = MLI_TAR_NORMAL_FILE;
        }
        memcpy(rh->name, h->name, sizeof(rh->name));
        memcpy(rh->linkname, h->linkname, sizeof(rh->linkname));

        /* Calculate and write checksum */
        chksum = mli_TarRawHeader_checksum(rh);
        chk_msg(mli_cstr_print_uint64(
                        chksum,
                        rh->checksum,
                        sizeof(rh->checksum),
                        MLI_TAR_OCTAL,
                        sizeof(rh->checksum) - 2),
                "bad checksum");

        rh->checksum[sizeof(rh->checksum) - 1] = 32;

        chk_msg(rh->checksum[sizeof(rh->checksum) - 2] == 0,
                "Second last char in checksum must be '\\0', i.e. 0(decimal).");
        chk_msg(rh->checksum[sizeof(rh->checksum) - 1] == 32,
                "Last char in checksum must be ' ', i.e. 32(decimal).");

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

/*                                  header                                    */
/* ========================================================================== */

struct mli_TarHeader mli_TarHeader_init(void)
{
        struct mli_TarHeader h;
        h.mode = 0;
        h.owner = 0;
        h.size = 0;
        h.mtime = 0;
        h.type = 0;
        memset(h.name, '\0', sizeof(h.name));
        memset(h.linkname, '\0', sizeof(h.linkname));
        return h;
}

chk_rc mli_TarHeader_set_directory(struct mli_TarHeader *h, const char *name)
{
        (*h) = mli_TarHeader_init();
        chk_msg(strlen(name) < sizeof(h->name), "Dirname is too long.");
        memcpy(h->name, name, strlen(name));
        h->type = MLI_TAR_DIRECTORY;
        h->mode = 0775;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_TarHeader_set_normal_file(
        struct mli_TarHeader *h,
        const char *name,
        const uint64_t size)
{
        (*h) = mli_TarHeader_init();
        chk_msg(strlen(name) < sizeof(h->name), "Filename is too long.");
        memcpy(h->name, name, strlen(name));
        h->size = size;
        h->type = MLI_TAR_NORMAL_FILE;
        h->mode = 0664;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_TarHeader_from_raw(
        struct mli_TarHeader *h,
        const struct mli_TarRawHeader *rh)
{
        uint64_t chksum_actual, chksum_expected;
        chksum_actual = mli_TarRawHeader_checksum(rh);

        /* Build and compare checksum */
        chk_msg(mli_Tar_field_to_uint(
                        &chksum_expected, rh->checksum, sizeof(rh->checksum)),
                "bad checksum string.");
        chk_msg(chksum_actual == chksum_expected, "bad checksum.");

        /* Load raw header into header */
        chk_msg(mli_Tar_field_to_uint(&h->mode, rh->mode, sizeof(rh->mode)),
                "bad mode");
        chk_msg(mli_Tar_field_to_uint(&h->owner, rh->owner, sizeof(rh->owner)),
                "bad owner");
        if (rh->size[0] == -128) {
                chk_msg(mli_Tar_field12_to_uint64_2001star_base256(
                                rh->size, &h->size),
                        "bad size, mode: base-256");
        } else {
                chk_msg(mli_Tar_field_to_uint(
                                &h->size, rh->size, sizeof(rh->size)),
                        "bad size, mode: base-octal");
        }
        chk_msg(mli_Tar_field_to_uint(&h->mtime, rh->mtime, sizeof(rh->mtime)),
                "bad mtime");
        h->type = rh->type;
        memcpy(h->name, rh->name, sizeof(h->name));
        memcpy(h->linkname, rh->linkname, sizeof(h->linkname));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

/* tar */
/* === */

struct mli_Tar mli_Tar_init(void)
{
        struct mli_Tar out;
        out.stream = NULL;
        out.pos = 0u;
        out.remaining_data = 0u;
        return out;
}

/*                                 read                                       */
/* ========================================================================== */

chk_rc mli_Tar_read_begin(struct mli_Tar *tar, struct mli_IO *stream)
{
        chk_msg(tar->stream == NULL,
                "Can't begin reading tar. "
                "tar is either still open or not initialized.");
        (*tar) = mli_Tar_init();
        tar->stream = stream;
        chk_msg(tar->stream, "Can't begin reading tar. Tar->stream is NULL.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_tread(struct mli_Tar *tar, void *data, const uint64_t size)
{
        int64_t res = mli_IO_read(data, 1, size, tar->stream);
        chk_msg(res >= 0, "Failed reading from tar.");
        chk_msg((uint64_t)res == size, "Failed reading from tar.");
        tar->pos += size;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_read_header(struct mli_Tar *tar, struct mli_TarHeader *h)
{
        struct mli_TarRawHeader rh;

        chk_msg(mli_Tar_tread(tar, &rh, sizeof(rh)),
                "Failed to read raw header");

        if (mli_TarRawHeader_is_null(&rh)) {
                (*h) = mli_TarHeader_init();
                return CHK_FAIL;
        }

        chk_msg(mli_TarHeader_from_raw(h, &rh), "Failed to parse raw header.");
        tar->remaining_data = h->size;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_read_data(struct mli_Tar *tar, void *ptr, uint64_t size)
{
        chk_msg(tar->remaining_data >= size,
                "Expect size to be read >= remaining_data");
        chk_msg(mli_Tar_tread(tar, ptr, size), "Failed to read payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                uint64_t i;
                const uint64_t next_record = mli_Tar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                char padding;

                for (i = 0; i < padding_size; i++) {
                        chk_msg(mli_Tar_tread(tar, &padding, 1),
                                "Failed to read padding-block "
                                "to reach next record.");
                }
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_read_finalize(struct mli_Tar *tar)
{
        struct mli_TarHeader h = mli_TarHeader_init();
        chk_msg(mli_Tar_read_header(tar, &h) == 0,
                "Failed to read the 2nd final block of zeros.");
        chk(h.mode == 0);
        chk(h.owner == 0);
        chk(h.size == 0);
        chk(h.mtime == 0);
        chk(h.type == 0);
        chk(h.name[0] == '\0');
        chk(h.linkname[0] == '\0');
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

/*                                  write                                     */
/* ========================================================================== */

chk_rc mli_Tar_write_begin(struct mli_Tar *tar, struct mli_IO *stream)
{
        chk_msg(tar->stream == NULL,
                "Can't begin writing tar. "
                "tar is either still open or not initialized.");
        (*tar) = mli_Tar_init();
        tar->stream = stream;
        chk_msg(tar->stream, "Can't begin writing tar. Tar->stream is NULL.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_twrite(
        struct mli_Tar *tar,
        const void *data,
        const uint64_t size)
{
        int64_t res = mli_IO_write(data, 1, size, tar->stream);
        chk_msg(res >= 0, "Failed writing to tar.");
        chk_msg((uint64_t)res == size, "Failed writing to tar.");
        tar->pos += size;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_write_header(struct mli_Tar *tar, const struct mli_TarHeader *h)
{
        struct mli_TarRawHeader rh;
        chk_msg(mli_TarRawHeader_from_header(&rh, h),
                "Failed to make raw-header");
        tar->remaining_data = h->size;
        chk_msg(mli_Tar_twrite(tar, &rh, sizeof(rh)),
                "Failed to write header.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_write_null_bytes(struct mli_Tar *tar, uint64_t n)
{
        uint64_t i;
        char nul = '\0';
        for (i = 0; i < n; i++) {
                chk_msg(mli_Tar_twrite(tar, &nul, 1), "Failed to write nulls");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_write_data(struct mli_Tar *tar, const void *data, uint64_t size)
{
        chk_msg(tar->remaining_data >= size,
                "Expect tar->remaining_data >= size to be written.");
        chk_msg(mli_Tar_twrite(tar, data, size),
                "Failed to write payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                const uint64_t next_record = mli_Tar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                chk_msg(mli_Tar_write_null_bytes(tar, padding_size),
                        "Failed to write padding zeros.");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Tar_write_finalize(struct mli_Tar *tar)
{
        chk_msg(mli_Tar_write_null_bytes(
                        tar, sizeof(struct mli_TarRawHeader) * 2),
                "Failed to write two final null records.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

mli_bool mli_Tar_is_known_file_type(const int file_type)
{
        mli_bool outcome = MLI_FALSE;
        switch (file_type) {
        case MLI_TAR_NORMAL_FILE:
        case MLI_TAR_HARD_LINK:
        case MLI_TAR_SYMBOLIC_LINK:
        case MLI_TAR_CHARACTER_SPECIAL:
        case MLI_TAR_BLOCK_SPECIAL:
        case MLI_TAR_DIRECTORY:
        case MLI_TAR_FIFO:
                outcome = MLI_TRUE;
        };
        return outcome;
}
