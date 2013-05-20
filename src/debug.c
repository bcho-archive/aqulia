#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

void ERROR(const char *msg)
{
    fprintf(stderr, msg);
    abort();
}

void DEBUG(const char *msg)
{
    fprintf(stderr, msg);
}
