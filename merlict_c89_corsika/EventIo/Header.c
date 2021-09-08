/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#include "../../merlict_c89_corsika/EventIo/Header.h"

#define EXPECTED_SYNC -736130505

struct _FirstFour {
        int32_t sync;
        int32_t type;
        int32_t id;
        int32_t length;
};
struct _FirstFour _FirstFour_zeros(void)
{
        struct _FirstFour ff;
        ff.sync = 0;
        ff.type = 0;
        ff.id = 0;
        ff.length = 0;
        return ff;
}

struct _TypeInfo {
        int32_t type;
        int32_t version;
        int user;
        int extended;
};
struct _TypeInfo _TypeInfo_zeros(void)
{
        struct _TypeInfo ti;
        ti.type = 0;
        ti.version = 0;
        ti.user = 0;
        ti.extended = 0;
        return ti;
}
struct _TypeInfo _TypeInfo_init(const int32_t _type)
{
        struct _TypeInfo info;
        info.type = _type & 0xffff;
        info.version = (_type & 0xfff00000) >> 20;
        info.user = (int)(_type & (1 << 16));
        info.extended = (int)(_type & (1 << 17));
        return info;
}

struct _LengthInfo {
        int only_sub_objects;
        int32_t length;
};
struct _LengthInfo _LengthInfo_zeros(void)
{
        struct _LengthInfo li;
        li.only_sub_objects = 0;
        li.length = 0;
        return li;
}
struct _LengthInfo _LengthInfo_init(const int32_t _length)
{
        struct _LengthInfo info;
        info.only_sub_objects = (int)(_length & 1 << 30);
        /* bit 31 of length is reserved */
        info.length = _length & 0x3fffffff;
        return info;
}

int64_t _extend_length(
        const int32_t extended,
        const struct _LengthInfo length_info)
{
        int64_t ext, len;
        ext = extended;
        ext &= 0xfff;
        len = length_info.length;
        len &= (ext << 12);
        return len;
}

int mliEventIoHeader_read(
        struct mliEventIoHeader *header,
        FILE *f,
        int level)
{
        struct _FirstFour first_four = _FirstFour_zeros();
        struct _TypeInfo type_info = _TypeInfo_zeros();
        struct _LengthInfo length_info = _LengthInfo_zeros();
        /* sub level headers do not have the 'sync' field. */

        if (level == MLI_EVENTIO_TOP_LEVEL) {
                mli_fread(&first_four.sync, sizeof(first_four.sync), 1, f);
                header->is_sync = EXPECTED_SYNC == first_four.sync;
        } else {
                mli_check(
                        level == MLI_EVENTIO_SUB_LEVEL,
                        "Level must be either 'top' or 'sub'.");
                header->is_sync = 1;
        }
        mli_fread(&first_four.type, sizeof(first_four.type), 1, f);
        mli_fread(&first_four.id, sizeof(first_four.id), 1, f);
        mli_fread(&first_four.length, sizeof(first_four.length), 1, f);

        type_info = _TypeInfo_init(first_four.type);
        length_info = _LengthInfo_init(first_four.length);

        mli_check(
                header->is_sync,
                "Expected EventIo-Header to be in sync, but its not.");

        header->id = first_four.id;

        header->type = type_info.type;
        header->version = type_info.version;
        header->user = type_info.user;
        header->extended = type_info.extended;
        header->only_sub_objects = length_info.only_sub_objects;

        if (!type_info.extended) {
                header->length = length_info.length;
        } else {
                int32_t extended;
                mli_fread(&extended, sizeof(int32_t), 1, f);
                header->length = _extend_length(extended, length_info);
        }

        return 1;
error:
        return 0;
}

void mliEventIoHeader_fprint(const struct mliEventIoHeader head, FILE *f)
{
        fprintf(f, "h.is_sync %d\n", head.is_sync);
        fprintf(f, "h.type %d\n", head.type);
        fprintf(f, "h.version %d\n", head.version);
        fprintf(f, "h.user %d\n", head.user);
        fprintf(f, "h.extended %d\n", head.extended);
        fprintf(f, "h.only_sub_objects %d\n", head.only_sub_objects);
        fprintf(f, "h.length %lu\n", head.length);
        fprintf(f, "h.id %d\n", head.id);
}
