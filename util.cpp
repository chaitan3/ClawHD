
#include "util.h"
#include <stdint.h>

int f_read_integer (ifstream *file, int offset) {
    uint32_t val;
    file -> seekg (offset);
    file -> read ((char *) &val, sizeof (val));
    return val;
}
