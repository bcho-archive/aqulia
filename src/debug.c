#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void ERROR(const char *fmt, ...)
{
    va_list list;

    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
    abort();
}

void DEBUG(const char *fmt, ...)
{
    va_list list;

    va_start(list, fmt);
    vfprintf(stderr, fmt, list);
    va_end(list);
}
