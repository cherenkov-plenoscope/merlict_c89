/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VIEWER_CURSOR_H_
#define MLI_VIEWER_CURSOR_H_

#include <stdint.h>

struct mlivrCursor {
        int64_t active;
        uint64_t col;
        uint64_t row;
        uint64_t num_cols;
        uint64_t num_rows;
};

void mlivrCursor_move_up(struct mlivrCursor *cursor);
void mlivrCursor_move_down(struct mlivrCursor *cursor);
void mlivrCursor_move_right(struct mlivrCursor *cursor);
void mlivrCursor_move_left(struct mlivrCursor *cursor);

#endif
