/* Copyright 2016 Sebastian A. Mueller, Dominik Neise */
#ifndef MERLICT_C89_CORSIKA_EVENTIOHEADER_H_
#define MERLICT_C89_CORSIKA_EVENTIOHEADER_H_

#include <stdint.h>

#define MLI_EVENTIO_TOP_LEVEL 1
#define MLI_EVENTIO_SUB_LEVEL 0

struct mliEventIoHeader {
        int is_sync;
        int32_t type;
        int32_t version;
        int user;
        int extended;
        int only_sub_objects;
        uint64_t length;
        int32_t id;
};
int mliEventIoHeader_read(
        struct mliEventIoHeader *header,
        FILE *f,
        int level);
void mliEventIoHeader_fprint(const struct mliEventIoHeader head, FILE *f);
#endif
