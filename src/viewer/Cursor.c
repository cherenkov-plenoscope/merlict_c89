/* Copyright 2019 Sebastian Achim Mueller                                     */
#include "Cursor.h"

void mli_viewer_Cursor_move_up(struct mli_viewer_Cursor *cursor)
{
        if (cursor->row != 0)
                cursor->row -= 1;
}

void mli_viewer_Cursor_move_down(struct mli_viewer_Cursor *cursor)
{
        if (cursor->row + 1u < cursor->num_rows)
                cursor->row += 1;
}

void mli_viewer_Cursor_move_right(struct mli_viewer_Cursor *cursor)
{
        if (cursor->col != 0)
                cursor->col -= 1;
}

void mli_viewer_Cursor_move_left(struct mli_viewer_Cursor *cursor)
{
        if (cursor->col + 1u < cursor->num_cols)
                cursor->col += 1;
}
