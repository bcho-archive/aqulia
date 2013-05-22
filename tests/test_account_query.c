#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "test.h"
#include "../src/core/account.h"

#define SEP printf("---------------------\n")

void print_records(struct consume_record *record);
void print_nrecords(struct consume_record *begin, struct consume_record *end);

int main(int argc, char *argv[])
{
    int cardno;
    struct account *account;
    struct consume_record *b, *e;

    if (argc < 2)
        return 1;

    cardno = atoi(argv[argc - 1]);
    if (account_read(cardno, &account) == E_OK) {
        account_query_by_date(account, "20130621", &b, &e);
        print_nrecords(b, e);
        SEP;
        account_query_by_date(account, "20131221", &b, &e);
        print_nrecords(b, e);
        printf("date");
        SEP;

        account_query_by_date_range(account, "20130621", "20130731", &b, &e);
        print_nrecords(b, e);
        SEP;
        account_query_by_date_range(account, "20130621", "20131231", &b, &e);
        print_nrecords(b, e);
        printf("date range");
        SEP;

        account_query_by_sum(account, 15.00, &b, &e);
        print_nrecords(b, e);
        SEP;
        account_query_by_sum(account, 150.00, &b, &e);
        print_nrecords(b, e);
        SEP;
        account_query_by_sum(account, 1.00, &b, &e);
        print_nrecords(b, e);
        printf("sum");
        SEP;

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

void print_nrecords(struct consume_record *begin, struct consume_record *end)
{
    for (;begin != NULL && begin != end;begin = begin->next)
        PRT(begin);
}
