/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_VIWER_MLIVR_CURSOR_H_
#define MERLICT_C89_VIWER_MLIVR_CURSOR_H_

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
