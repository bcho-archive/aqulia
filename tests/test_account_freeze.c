#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/account.h"

int main(int argc, char *argv[])
{
    int cardno;
    struct account *account;

    if (argc < 2)
        return 1;

    cardno = atoi(argv[argc - 1]);
    if (account_read(cardno, &account) == E_OK) {
        printf("account ok\n");
        account_freeze(account);
        printf("%d\n", account_validate(account) == E_FREEZE);
        account_freeze(account);
    } else {
        printf("account freezed\n");
        account_unfreeze(account);
        printf("%d\n", account_validate(account) == E_OK);
    }

    return 0;
}
