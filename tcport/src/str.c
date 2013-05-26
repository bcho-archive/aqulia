#include "str.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

char *
trim(char *str)
{
    char *left, *right, *p;

    if (str == NULL)
        return NULL;

    left = str;
    right = str + strlen(str) - 1;

    while (isspace(*left))
        left++;
    while (isspace(*right))
        right--;

    if (left == str) {
        *(right + 1) = '\0';
    } else {                    /* shift the block forward */
        p = str;
        while (left <= right)
            *p++ = *left++;

        *p = '\0';
    }

    return str;
}

char *
split(char *str, char delim)
{
    char *p;

    p = strchr(str, delim);

    if (p != NULL) {
     *p = '\0';
     return p + 1;
    }
    return NULL;
}
