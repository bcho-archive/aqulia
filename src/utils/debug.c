#include "debug.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void
DEBUG(char *fmt, ...)
{
    va_list list;

    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    fprintf(stderr, "\n");
}

void
ERROR(char *fmt, ...)
{
    va_list list;

    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    fprintf(stderr, "\n");
    abort();
}
