#include "account.h"
#include "../utils/misc.h"
#include "../utils/debug.h"
#include "../utils/str.h"

#include <stdio.h>
#include <string.h>

#define LINE_MAX 1024

inline static int
validate_freeze(const char *fname)
{
    FILE *stream;
    char line[LINE_MAX];

    touch_file((char *) fname);
    if ((stream = fopen(fname, "r")) == NULL)
        return 0;

    fgets(line, LINE_MAX, stream);
    trim(line);
    if (strcmp(line, "1") == 0)
        return 1;
    return 0;
}

E_ACCOUNT_ACCESS_TYPE
account_validate(struct account *account)
{
    if (!validate_freeze(account->ffreeze) && account->state == CARD_USABLE)
        return E_OK;
    else
        return E_FREEZE;
}
