/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef mliStr_H_
#define mliStr_H_

struct mliStr {
        char *c_str;
        uint64_t capacity;
        uint64_t length;
};
struct mliStr mliStr_init(void);
void mliStr_free(struct mliStr *str);
int mliStr_malloc(struct mliStr *str, const uint64_t capacity);
int mliStr_push_back_char(struct mliStr *str, const char c);
int mliStr_push_back_c_str(struct mliStr *str, const char *s);
int mliStr_malloc_from_path(struct mliStr *str, const char *path);
int mliStr_convert_line_break_CRLF_CR_to_LF(
        struct mliStr *dst,
        const struct mliStr *src);

#endif