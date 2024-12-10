/* Copyright 2018-2024 Sebastian Achim Mueller */

#include "args.h"
#include "../chk/chk.h"

MLI_ARRAY_IMPLEMENTATION_FREE(
        mli_ArrayString,
        struct mli_String,
        mli_String_free)

int mli_ArrayString_from_argc_argv(
        struct mli_ArrayString *self,
        int argc,
        char *argv[])
{
        int i;
        mli_ArrayString_free(self);
        chk_msg(argc >= 0, "Expected 'argc' >= 0.");
        chk_msg(mli_ArrayString_malloc(self, argc), "Failed to malloc Argv.");
        for (i = 0; i < argc; i++) {
                struct mli_String *field = &self->array[i];
                chk_msg(mli_String_from_cstr(field, argv[i]),
                        "Failed to malloc string in Argv.");
        }

        return 1;
chk_error:
        mli_ArrayString_free(self);
        return 0;
}
