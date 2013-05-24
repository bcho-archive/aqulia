#include "account.h"
#include "../utils/misc.h"
#include "../utils/debug.h"
#include "../utils/str.h"

#include <stdio.h>
#include <stdlib.h>
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

static int
account_list(int **cardno)
{
    int count, no;
    int *accounts;
    char line[19];
    FILE *stream;

    touch_file(DATA_PREFIX ACCOUNT_LIST);
    if ((stream = fopen(DATA_PREFIX ACCOUNT_LIST, "r")) == NULL)
        ERROR("open account list: %s", ACCOUNT_LIST);

    count = 0; accounts = NULL;
    while (fgets(line, 19, stream) != NULL) {
        no = atoi(line);

        if (no) {
            count++;
            accounts = realloc(accounts, sizeof(int) * count);
            accounts[count - 1] = no;
        }
    }
    *cardno = accounts;
    return count;
}

int account_exists(int cardno)
{
    int *exists;
    int i, count;

    count = account_list(&exists);
    for (i = 0;i < count;i++)
        if (exists[i] == cardno)
            return 1;
    return 0;
}
