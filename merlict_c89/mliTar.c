/**
 * Copyright (c) 2017 rxi
 * Copyright (c) 2019 Sebastian A. Mueller
 *                    Max-Planck-Institute for nuclear-physics, Heidelberg
 */

#include "mliTar.h"
#include <string.h>
#include "chk_debug.h"
#include "mli_cstr_and_numbers.h"

struct _mliTarRawHeader {
        char name[MLITAR_NAME_LENGTH];
        char mode[8];
        char owner[8];
        char group[8];
        char size[12];
        char mtime[12];
        char checksum[8];
        char type;
        char linkname[MLITAR_NAME_LENGTH];
        char _padding[255];
};

struct mliTar mliTar_init(void)
{
        struct mliTar out;
        out.stream = NULL;
        out.pos = 0u;
        out.remaining_data = 0u;
        return out;
}

struct mliTarHeader mliTarHeader_init(void)
{
        struct mliTarHeader h;
        h.mode = 0;
        h.owner = 0;
        h.size = 0;
        h.mtime = 0;
        h.type = 0;
        memset(h.name, '\0', sizeof(h.name));
        memset(h.linkname, '\0', sizeof(h.linkname));
        return h;
}

/* write */

int _mliTar_twrite(struct mliTar *tar, const void *data, const uint64_t size)
{
        int64_t res = fwrite(data, 1, size, tar->stream);
        chk_msg(res >= 0, "Failed writing to tar.");
        chk_msg((uint64_t)res == size, "Failed writing to tar.");
        tar->pos += size;
        return 1;
error:
        return 0;
}

/* read */

int _mliTar_tread(struct mliTar *tar, void *data, const uint64_t size)
{
        int64_t res = fread(data, 1, size, tar->stream);
        chk_msg(res >= 0, "Failed reading from tar.");
        chk_msg((uint64_t)res == size, "Failed reading from tar.");
        tar->pos += size;
        return 1;
error:
        return 0;
}

/* close */

int mliTar_close(struct mliTar *tar)
{
        fclose(tar->stream);
        (*tar) = mliTar_init();
        return 1;
}

uint64_t _mliTar_round_up(uint64_t n, uint64_t incr)
{
        return n + (incr - n % incr) % incr;
}

uint64_t _mliTar_checksum(const struct _mliTarRawHeader *rh)
{
        uint64_t i;
        unsigned char *p = (unsigned char *)rh;
        uint64_t res = 256;
        for (i = 0; i < offsetof(struct _mliTarRawHeader, checksum); i++) {
                res += p[i];
        }
        for (i = offsetof(struct _mliTarRawHeader, type); i < sizeof(*rh);
             i++) {
                res += p[i];
        }
        return res;
}

int _mliTar_write_null_bytes(struct mliTar *tar, uint64_t n)
{
        uint64_t i;
        char nul = '\0';
        for (i = 0; i < n; i++) {
                chk_msg(_mliTar_twrite(tar, &nul, 1), "Failed to write nulls");
        }
        return 1;
error:
        return 0;
}

int mliTar_field_to_uint(
        uint64_t *out,
        const char *field,
        const uint64_t field_size)
{
        char buff[MLITAR_NAME_LENGTH] = {'\0'};
        chk(field_size < MLITAR_NAME_LENGTH);
        memcpy(buff, field, field_size);

        /* Take care of historic 'space' (32 decimal) termination */
        /* Convert all 'space' terminations to '\0' terminations. */

        if (buff[field_size - 1] == 32) {
                buff[field_size - 1] = 0;
        }
        if (buff[field_size - 2] == 32) {
                buff[field_size - 2] = 0;
        }

        chk(mli_cstr_to_uint64(out, buff, MLITAR_OCTAL));
        return 1;
error:
        return 0;
}

int _mliTar_raw_to_header(
        struct mliTarHeader *h,
        const struct _mliTarRawHeader *rh)
{
        uint64_t chksum_actual, chksum_expected;
        chksum_actual = _mliTar_checksum(rh);

        /* Build and compare checksum */
        chk_msg(mliTar_field_to_uint(
                        &chksum_expected, rh->checksum, sizeof(rh->checksum)),
                "bad checksum string.");
        chk_msg(chksum_actual == chksum_expected, "bad checksum.");

        /* Load raw header into header */
        chk_msg(mliTar_field_to_uint(&h->mode, rh->mode, sizeof(rh->mode)),
                "bad mode");
        chk_msg(mliTar_field_to_uint(&h->owner, rh->owner, sizeof(rh->owner)),
                "bad owner");
        if (rh->size[0] == -128) {
                chk_msg(mliTar_field12_to_uint64_2001star_base256(
                                rh->size, &h->size),
                        "bad size, mode: base-256");
        } else {
                chk_msg(mliTar_field_to_uint(
                                &h->size, rh->size, sizeof(rh->size)),
                        "bad size, mode: base-octal");
        }
        chk_msg(mliTar_field_to_uint(&h->mtime, rh->mtime, sizeof(rh->mtime)),
                "bad mtime");
        h->type = rh->type;
        memcpy(h->name, rh->name, sizeof(h->name));
        memcpy(h->linkname, rh->linkname, sizeof(h->linkname));

        return 1;
error:
        return 0;
}

int mliTar_uint_to_field(
        const uint64_t val,
        char *field,
        const uint64_t fieldsize)
{
        chk(mli_uint64_to_cstr(
                val, field, fieldsize, MLITAR_OCTAL, fieldsize - 1));
        return 1;
error:
        return 0;
}

int mliTar_uint64_to_field12_2001star_base256(uint64_t val, char *field)
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
        return 1;
error:
        return 0;
}

int mliTar_field12_to_uint64_2001star_base256(const char *field, uint64_t *val)
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
        return 1;
error:
        return 0;
}

int _mliTar_make_raw_header(
        struct _mliTarRawHeader *rh,
        const struct mliTarHeader *h)
{
        uint64_t chksum;

        /* Load header into raw header */
        memset(rh, 0, sizeof(*rh));
        chk_msg(mliTar_uint_to_field(h->mode, rh->mode, sizeof(rh->mode)),
                "bad mode");
        chk_msg(mliTar_uint_to_field(h->owner, rh->owner, sizeof(rh->owner)),
                "bad owner");
        if (h->size >= MLITAR_MAX_FILESIZE_OCTAL) {
                chk_msg(mliTar_uint64_to_field12_2001star_base256(
                                h->size, rh->size),
                        "bad size, mode: base-256");
        } else {
                chk_msg(mliTar_uint_to_field(
                                h->size, rh->size, sizeof(rh->size)),
                        "bad size, mode: base-octal");
        }
        chk_msg(mliTar_uint_to_field(h->mtime, rh->mtime, sizeof(rh->mtime)),
                "bad mtime");
        rh->type = h->type ? h->type : MLITAR_TREG;
        memcpy(rh->name, h->name, sizeof(rh->name));
        memcpy(rh->linkname, h->linkname, sizeof(rh->linkname));

        /* Calculate and write checksum */
        chksum = _mliTar_checksum(rh);
        chk_msg(mli_uint64_to_cstr(
                        chksum,
                        rh->checksum,
                        sizeof(rh->checksum),
                        MLITAR_OCTAL,
                        sizeof(rh->checksum) - 2),
                "bad checksum");

        rh->checksum[sizeof(rh->checksum) - 1] = 32;

        chk_msg(rh->checksum[sizeof(rh->checksum) - 2] == 0,
                "Second last char in checksum must be '\\0', i.e. 0(decimal).");
        chk_msg(rh->checksum[sizeof(rh->checksum) - 1] == 32,
                "Last char in checksum must be ' ', i.e. 32(decimal).");

        return 1;
error:
        return 0;
}

int mliTar_open(struct mliTar *tar, const char *filename, const char *mode)
{
        *tar = mliTar_init();

        /* Assure mode is always binary */
        if (strchr(mode, 'r'))
                mode = "rb";
        if (strchr(mode, 'w'))
                mode = "wb";
        if (strchr(mode, 'a'))
                mode = "ab";

        tar->stream = fopen(filename, mode);
        chk_msg(tar->stream, "Failed to open tar-file.");

        return 1;
error:
        return 0;
}

int mliTar_raw_header_is_null(const struct _mliTarRawHeader *rh)
{
        uint64_t i = 0u;
        unsigned char *p = (unsigned char *)rh;
        for (i = 0; i < sizeof(struct _mliTarRawHeader); i++) {
                if (p[i] != '\0') {
                        return 0;
                }
        }
        return 1;
}

int mliTar_read_header(struct mliTar *tar, struct mliTarHeader *h)
{
        struct _mliTarRawHeader rh;

        chk_msg(_mliTar_tread(tar, &rh, sizeof(rh)),
                "Failed to read raw header");

        if (mliTar_raw_header_is_null(&rh)) {
                return 0;
        }

        chk_msg(_mliTar_raw_to_header(h, &rh), "Failed to parse raw header.");
        tar->remaining_data = h->size;
        return 1;
error:
        return 0;
}

int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size)
{
        chk_msg(tar->remaining_data >= size,
                "Expect size to be read >= remaining_data");
        chk_msg(_mliTar_tread(tar, ptr, size), "Failed to read payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                uint64_t i;
                const uint64_t next_record = _mliTar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                char padding;

                for (i = 0; i < padding_size; i++) {
                        chk_msg(_mliTar_tread(tar, &padding, 1),
                                "Failed to read padding-block "
                                "to reach next record.");
                }
        }

        return 1;
error:
        return 0;
}

int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h)
{
        struct _mliTarRawHeader rh;
        chk_msg(_mliTar_make_raw_header(&rh, h), "Failed to make raw-header");
        tar->remaining_data = h->size;
        chk_msg(_mliTar_twrite(tar, &rh, sizeof(rh)),
                "Failed to write header.");
        return 1;
error:
        return 0;
}

int mliTar_write_file_header(
        struct mliTar *tar,
        const char *name,
        uint64_t size)
{
        struct mliTarHeader h = mliTarHeader_init();
        chk_msg(strlen(name) < sizeof(h.name), "Filename is too long.");
        memcpy(h.name, name, strlen(name));
        h.size = size;
        h.type = MLITAR_TREG;
        h.mode = 0664;
        /* Write header */
        chk_msg(mliTar_write_header(tar, &h), "Failed to write file-header.");
        return 1;
error:
        return 0;
}

int mliTar_write_dir_header(struct mliTar *tar, const char *name)
{
        struct mliTarHeader h = mliTarHeader_init();
        chk_msg(strlen(name) < sizeof(h.name), "Dirname is too long.");
        memcpy(h.name, name, strlen(name));
        h.type = MLITAR_TDIR;
        h.mode = 0775;
        chk_msg(mliTar_write_header(tar, &h), "Failed to write dir-header.");
        return 1;
error:
        return 0;
}

int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size)
{
        chk_msg(tar->remaining_data >= size,
                "Expect tar->remaining_data >= size to be written.");
        chk_msg(_mliTar_twrite(tar, data, size),
                "Failed to write payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                const uint64_t next_record = _mliTar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                chk_msg(_mliTar_write_null_bytes(tar, padding_size),
                        "Failed to write padding zeros.");
        }
        return 1;
error:
        return 0;
}

int mliTar_finalize(struct mliTar *tar)
{
        chk_msg(_mliTar_write_null_bytes(
                        tar, sizeof(struct _mliTarRawHeader) * 2),
                "Failed to write two final null records.");
        return 1;
error:
        return 0;
}
