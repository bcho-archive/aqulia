#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/account.h"
#include "../src/debug.h"

int main(int argc, char *argv[])
{
    int cardno;
    struct account *account;
    struct consume_record *b, *e;

    if (argc < 2)
        return 1;

    cardno = atoi(argv[argc - 1]);
    if (account_read(cardno, &account) == E_FREEZE)
        account_unfreeze(account);

    /* delete */
    if (account_query_by_date(account, "20130621", &b, &e) == E_OK) {
        printf("record: %s\n", b->consumed);
        account_consume_delete(account, b);
        printf("record: %s\n", b->consumed);
    }

    /* consume */
    if (account_consume(account, "20130822", "20130822", account->info->balance + 0.1,
                        CONSUME_POS, "test machine") == E_CONSUME_CANTPAY)
        printf("too expensive\n");
    if (account_consume(account, "20130823", "20130823", account->info->balance - 0.1,
                        CONSUME_POS, "test machine") == E_CONSUME_OK)
        printf("bought\n");
    DEBUG("%s %.2lf", account->record->next->received, account->record->next->balance);
    if (account_consume(account, "20130826", "20130826", account->info->balance,
                        CONSUME_POS, "test machine") == E_CONSUME_OK)
        printf("bought\n");
    DEBUG("%s %.2lf", account->record->next->received, account->record->next->balance);

    account_destory(account);

    return 0;
}
