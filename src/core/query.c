#include "account.h"

#include <string.h>

E_ACCOUNT_ACCESS_TYPE
account_query_by_date(struct account *account, char *date,
                      struct consume_record **begin_return,
                      struct consume_record **end_return)
{
    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *b, *e;

    if ((state = account_sort_by_date(account, 1)) != E_OK)
        return state;

    for (b = account->record;
         b != NULL && strcmp(date, b->consumed) != 0;
         b = b->next)
        ;
    for (e = b;
         e != NULL && strcmp(date, e->consumed) == 0;
         e = e->next)
        ;
    *begin_return = b;
    *end_return = e;

    return state;
}

E_ACCOUNT_ACCESS_TYPE
account_query_by_date_range(struct account *account,
                            char *lower, char *upper,
                            struct consume_record **begin_return,
                            struct consume_record **end_return)
{
    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *b, *e;

    if ((state = account_sort_by_date(account, 1)) != E_OK)
        return state;

    for (b = account->record;
         b != NULL && strcmp(lower, b->consumed) > 0;
         b = b->next)
        ;
    for (e = b;
         e != NULL && strcmp(upper, e->consumed) >= 0;
         e = e->next)
        ;
    *begin_return = b;
    *end_return = e;

    return state;
}

E_ACCOUNT_ACCESS_TYPE
account_query_by_sum(struct account *account, double sum,
                     struct consume_record **begin_return,
                     struct consume_record **end_return)
{
    E_ACCOUNT_ACCESS_TYPE state;
    struct consume_record *b, *e;

    if ((state = account_sort_by_sum(account, 0)) != E_OK)
        return state;

    for (b = account->record;
         b != NULL && sum > b->sum;
         b = b->next)
        ;
    for (e = b;
         e != NULL && sum <= e->sum;
         e = e->next)
        ;
    *begin_return = b;
    *end_return = e;

    return state;
}
