#include "account.h"
#include "../utils.h"

#include <stdio.h>
#include <string.h>

#define MAXLINE 1024

inline static int validate_freeze(const char *file)
{
    FILE *stream;
    char line[MAXLINE];

    if ((stream = fopen(file, "r")) == NULL)
        return 1;

    fgets(line, MAXLINE, stream);
    trim(line);
    if (strcmp(line, "1") == 0)
        return 0;
    return 1;
}

E_ACCOUNT_READ_TYPE account_validate(struct account *account)
{
    if (validate_freeze(account->freeze_file))
        return E_OK;
    else
        return E_FREEZE;
}
