/**
 * Copyright (c) 2017 rxi
 * Copyright (c) 2019 Sebastian A. Mueller
 *                    Max-Planck-Institute for nuclear-physics, Heidelberg
 */

#include "mliTar.h"
#include <string.h>
#include "mli_debug.h"

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
        uint64_t i;
        struct mliTarHeader h;
        h.mode = 0;
        h.owner = 0;
        h.size = 0;
        h.mtime = 0;
        h.type = 0;
        for (i = 0; i < MLITAR_NAME_LENGTH; i++) {
                h.name[i] = '\0';
                h.linkname[i] = '\0';
        }
        return h;
}

/* write */

int _mliTar_twrite(struct mliTar *tar, const void *data, const uint64_t size)
{
        int64_t res = fwrite(data, 1, size, tar->stream);
        mli_check(res >= 0, "Failed writing to tar.");
        mli_check((uint64_t)res == size, "Failed writing to tar.");
        tar->pos += size;
        return 1;
error:
        return 0;
}

/* read */

int _mliTar_tread(struct mliTar *tar, void *data, const uint64_t size)
{
        int64_t res = fread(data, 1, size, tar->stream);
        mli_check(res >= 0, "Failed reading from tar.");
        mli_check((uint64_t)res == size, "Failed reading from tar.");
        tar->pos += size;
        return 1;
error:
        return 0;
}

/* seek */

int _mliTar_seek(struct mliTar *tar, const uint64_t offset)
{
        mli_check(0 == fseek(tar->stream, offset, SEEK_CUR), "Failed to seek");
        tar->pos += offset;
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
                mli_check(
                        _mliTar_twrite(tar, &nul, 1), "Failed to write nulls");
        }
        return 1;
error:
        return 0;
}

int _mliTar_raw_to_header(
        struct mliTarHeader *h,
        const struct _mliTarRawHeader *rh)
{
        uint64_t chksum1, chksum2;

        /* Build and compare checksum */
        chksum1 = _mliTar_checksum(rh);
        mli_check(mli_string_to_uint(&chksum2, rh->checksum, 8u),
                "bad checksum string.");
        mli_check(chksum1 == chksum2, "Bad checksum.");
        /* Load raw header into header */
        mli_check(mli_string_to_uint(&h->mode, rh->mode, 8u), "bad mode");
        mli_check(mli_string_to_uint(&h->owner, rh->owner, 8u), "bad owner");
        mli_check(mli_string_to_uint(&h->size, rh->size, 8u), "bad size");
        mli_check(mli_string_to_uint(&h->mtime, rh->mtime, 8u), "bad mtime");
        h->type = rh->type;
        sprintf(h->name, "%s", rh->name);
        sprintf(h->linkname, "%s", rh->linkname);

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
        mli_check(mli_uint_to_string(h->mode, rh->mode, sizeof(rh->mode), 8u),
                "bad mode");
        mli_check(mli_uint_to_string(h->owner, rh->owner, sizeof(rh->owner), 8u),
                "bad owner");
        mli_check(mli_uint_to_string(h->size, rh->size, sizeof(rh->size), 8u),
                "bad size");
        mli_check(mli_uint_to_string(h->mtime, rh->mtime, sizeof(rh->mtime), 8u),
                "bad mtime");
        rh->type = h->type ? h->type : MLITAR_TREG;
        sprintf(rh->name, "%s", h->name);
        sprintf(rh->linkname, "%s", h->linkname);

        /* Calculate and write checksum */
        chksum = _mliTar_checksum(rh);
        mli_check(
                mli_uint_to_string(
                        chksum, rh->checksum, sizeof(rh->checksum), 8u),
                "bad checksum");
        rh->checksum[7] = ' ';

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
        mli_check(tar->stream, "Failed to open tar-file.");

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

        mli_check(
                _mliTar_tread(tar, &rh, sizeof(rh)),
                "Failed to read raw header");

        if (mliTar_raw_header_is_null(&rh)) {
                return 0;
        }

        mli_check(_mliTar_raw_to_header(h, &rh), "Failed to parse raw header.");
        tar->remaining_data = h->size;
        return 1;
error:
        return 0;
}

int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size)
{
        mli_check(
                tar->remaining_data >= size,
                "Expect size to be read >= remaining_data");
        mli_check(
                _mliTar_tread(tar, ptr, size), "Failed to read payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                const uint64_t next_record = _mliTar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;

                mli_check(
                        _mliTar_seek(tar, padding_size),
                        "Failed to read padding block to reach nect record.");
        }

        return 1;
error:
        return 0;
}

int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h)
{
        struct _mliTarRawHeader rh;
        _mliTar_make_raw_header(&rh, h);
        tar->remaining_data = h->size;
        mli_check(
                _mliTar_twrite(tar, &rh, sizeof(rh)),
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
        sprintf(h.name, "%s", name);
        h.size = size;
        h.type = MLITAR_TREG;
        h.mode = 0664;
        /* Write header */
        mli_check(mliTar_write_header(tar, &h), "Failed to write file-header.");
        return 1;
error:
        return 0;
}

int mliTar_write_dir_header(struct mliTar *tar, const char *name)
{
        struct mliTarHeader h = mliTarHeader_init();
        sprintf(h.name, "%s", name);
        h.type = MLITAR_TDIR;
        h.mode = 0775;
        mli_check(mliTar_write_header(tar, &h), "Failed to write dir-header.");
        return 1;
error:
        return 0;
}

int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size)
{
        mli_check(
                tar->remaining_data >= size,
                "Expect tar->remaining_data >= size to be written.");
        mli_check(
                _mliTar_twrite(tar, data, size),
                "Failed to write payload-data.");
        tar->remaining_data -= size;

        if (tar->remaining_data == 0) {
                const uint64_t next_record = _mliTar_round_up(tar->pos, 512);
                const uint64_t padding_size = next_record - tar->pos;
                mli_check(
                        _mliTar_write_null_bytes(tar, padding_size),
                        "Failed to write padding zeros.");
        }
        return 1;
error:
        return 0;
}

int mliTar_finalize(struct mliTar *tar)
{
        mli_check(
                _mliTar_write_null_bytes(
                        tar, sizeof(struct _mliTarRawHeader) * 2),
                "Failed to write two final null records.");
        return 1;
error:
        return 0;
}
