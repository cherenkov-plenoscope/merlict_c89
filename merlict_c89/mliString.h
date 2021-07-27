/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISTRING_H_
#define MERLICT_C89_MLISTRING_H_

struct mliDynStr {
        char *c_str;
        uint64_t capacity;
        uint64_t length;
};
struct mliDynStr mliDynStr_init(void);
void mliDynStr_free(struct mliDynStr *str);
int mliDynStr_malloc(struct mliDynStr *str, const uint64_t capacity);
int mliDynStr_push_back(struct mliDynStr *str, const char *s);
int mliDynStr_malloc_from_path(struct mliDynStr *str, const char *path);
int mliDynStr_convert_line_break_CRLF_CR_to_LF(
        struct mliDynStr *dst,
        const struct mliDynStr *src);

#endif
