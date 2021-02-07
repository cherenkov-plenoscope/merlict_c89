/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLINAME_H_
#define MERLICT_C89_MLINAME_H_

#define MLI_NAME_CAPACITY 128

struct mliName {
        char c_str[MLI_NAME_CAPACITY];
};

struct mliName mliName_init(void);
int mliName_valid(const struct mliName *name);
#endif
