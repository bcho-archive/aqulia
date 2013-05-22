#include "account.h"
#include "../debug.h"

#include <string.h>

E_ACCOUNT_READ_TYPE account_query_by_date(struct account *account, char *date,
                                          struct consume_record **begin,
                                          struct consume_record **end)
{
    E_ACCOUNT_READ_TYPE ret;

    if ((ret = account_sort_by_date(account, 0)) != E_OK)
        return ret;

    for (*begin = account->record;
         *begin != NULL && strcmp(date, (*begin)->consumed) != 0;
         *begin = (*begin)->next)
        ;
    for (*end = *begin;
         *end != NULL && strcmp(date, (*end)->consumed) == 0;
         *end = (*end)->next);
        ;

    return ret;
}

E_ACCOUNT_READ_TYPE account_query_by_date_range(struct account *account,
                                                char *lower, char *upper,
                                                struct consume_record **begin,
                                                struct consume_record **end)
{
    E_ACCOUNT_READ_TYPE ret;

    if ((ret = account_sort_by_date(account, 1)) != E_OK)
        return ret;

    for (*begin = account->record;
         *begin != NULL && strcmp(lower, (*begin)->consumed) > 0;
         *begin = (*begin)->next)
        ;
    for (*end = *begin;
         *end != NULL && strcmp(upper, (*end)->consumed) >= 0;
         *end = (*end)->next)
        ;

    return ret;
}

E_ACCOUNT_READ_TYPE account_query_by_sum(struct account *account, double sum,
                                         struct consume_record **begin,
                                         struct consume_record **end)
{
    E_ACCOUNT_READ_TYPE ret;

    if ((ret = account_sort_by_sum(account, 0)) != E_OK)
        return ret;

    for (*begin = account->record;
         *begin != NULL && sum > (*begin)->sum;
         *begin = (*begin)->next)
        ;
    for (*end = (*begin);
         *end != NULL && sum <= (*end)->sum;
         *end = (*end)->next)
        ;
    
    return ret;
}
