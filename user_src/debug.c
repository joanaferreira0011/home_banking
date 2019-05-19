#include "debug.h"
#include <stddef.h>
void __debug_log_str_array(char* header, char **array) {
    __debug_log_str(header);
    for (int i = 0; array[i] != NULL; ++i) {
        //fprintf(stderr, "Index %d: %s\n", i, array[i]);
    }
}
