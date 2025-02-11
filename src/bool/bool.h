/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_BOOL_BOOL_H_
#define MLI_BOOL_BOOL_H_

#define mli_bool int

enum mli_bool_states { MLI_FALSE = 0, MLI_TRUE = 1 };

char mli_bool_to_char(const mli_bool self);

#endif
