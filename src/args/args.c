/* Copyright 2018-2024 Sebastian Achim Mueller */

#include "args.h"
#include "../chk/chk.h"

chk_rc mli_StringVector_from_argc_argv(
        struct mli_StringVector *self,
        int argc,
        char *argv[])
{
        int i;
        mli_StringVector_free(self);
        chk_msg(argc >= 0, "Expected 'argc' >= 0.");
        chk_msg(mli_StringVector_malloc(self, argc), "Failed to malloc Argv.");
        for (i = 0; i < argc; i++) {
                struct mli_String *field = NULL;
                chk_msg(mli_StringVector_push_back(self, mli_String_init()),
                        "");
                field = &self->array[i];
                chk_msg(mli_String_from_cstr(field, argv[i]),
                        "Failed to malloc string in Argv.");
        }

        return CHK_SUCCESS;
chk_error:
        mli_StringVector_free(self);
        return CHK_FAIL;
}
