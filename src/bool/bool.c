#include "bool.h"

char mli_bool_to_char(const mli_bool self)
{
        if (self == MLI_FALSE) {
                return 'F';
        } else {
                return 'T';
        }
}
