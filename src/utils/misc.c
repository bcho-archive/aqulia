#include "misc.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

void
touch_file(char *fname)
{
    FILE *stream;

    stream = fopen(fname, "a");
    if (stream != NULL)
        fclose(stream);
    else
        DEBUG("touch error: %s", fname);
}
