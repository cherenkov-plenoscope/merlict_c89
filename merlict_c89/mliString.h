/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISTRING_H_
#define MERLICT_C89_MLISTRING_H_

struct mliString {
        char *c_str;
        uint64_t capacity;
};

struct mliString mliString_init(void);
int mliString_malloc(struct mliString *str, const uint64_t strlen);
void mliString_free(struct mliString *str);
int mliString_malloc_from_file(struct mliString *str, const char *path);
int mliString_convert_line_break_CRLF_CR_to_LF(
        struct mliString *dst,
        const struct mliString *src);
#endif
