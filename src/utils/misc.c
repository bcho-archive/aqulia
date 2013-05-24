#include "misc.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define isleap(year) ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))

int
date_validate(char *str)
{
    int year, month, day, date;
    
    int mdays[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
    };

    if (strlen(str) < 8)
        return 0;

    date = atoi(str);
    year = date / 10000;
    month = date % 10000 / 100;
    day = date % 100;

    if (year < 2000 || year > 2013)
        return 0;
    if (month < 0 || month > 12)
        return 0;
    if (day < 0 || day > mdays[isleap(year)][month])
        return 0;
    return 1;
}
