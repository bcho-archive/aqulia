#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/account.h"

#define SEP printf("---------------------\n")

void print_records(struct consume_record *record);

int main(int argc, char *argv[])
{
    int cardno;
    struct account *account;

    if (argc < 2)
        return 1;

    cardno = atoi(argv[argc - 1]);
    if (account_read(cardno, &account) == E_OK) {
        printf("cardno: %d\n", account->info->cardno);
        printf("balance: %.2f\n", account->info->balance);

        account_sort_by_date(account, 0);
        print_records(account->record);
        SEP;
        account_sort_by_sum(account, 0);
        print_records(account->record);
        SEP;
        account_sort_by_transcation(account, 0);
        print_records(account->record);

        account_destory(account);
    }

    return 0;
}

#define PRT(record) printf("consumed: %s received: %s\n"\
                           "sum: %.2lf balance: %.2lf\n"\
                           "type: %d\ntranscation: %d pos: %s\n\n",\
                           record->consumed, record->received, record->sum,\
                           record->balance, record->consume_type, record->pos->transcation,\
                           record->pos->name)

void print_records(struct consume_record *record)
{
    struct consume_record *p;
    
    for (p = record;p != NULL;p = p->next)
        PRT(p);
}
