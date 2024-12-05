/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VIEWER_CURSOR_H_
#define MLI_VIEWER_CURSOR_H_

#include <stdint.h>

struct mli_viewer_Cursor {
        int64_t active;
        uint64_t col;
        uint64_t row;
        uint64_t num_cols;
        uint64_t num_rows;
};

void mli_viewer_Cursor_move_up(struct mli_viewer_Cursor *cursor);
void mli_viewer_Cursor_move_down(struct mli_viewer_Cursor *cursor);
void mli_viewer_Cursor_move_right(struct mli_viewer_Cursor *cursor);
void mli_viewer_Cursor_move_left(struct mli_viewer_Cursor *cursor);

#endif
