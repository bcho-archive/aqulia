#include "account.h"
#include "../utils/link_sort.h"

#include <string.h>

/* sorting helpers */
inline static void *
next(void *cur)
{
    return ((struct consume_record *) cur)->next;
}

inline static void
set_next(void *cur, void *value)
{
    ((struct consume_record *) cur)->next = value;
}

inline static
int cmp_date(void *a, void *b)
{
    /*
    return strcmp(((struct consume_record *) a)->consumed,
                  ((struct consume_record *) b)->consumed);
                  */
    struct consume_record c, d;
    int ret;

    /* TODO FIXME
     * When casting a/b into struct consume_record, 
     * the address will be changed to something else.
     */
    c = *((struct consume_record *) a);
    d = *((struct consume_record *) b);
    ret = strcmp(c.consumed, d.consumed);
    if (ret == 0) {
        if (c.balance > d.balance)
            ret = 1;
        else if (c.balance < d.balance)
            ret = -1;
    }
    return ret;
}

E_ACCOUNT_ACCESS_TYPE
account_sort_by_date(struct account *account, int reverse)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_validate(account)) != E_OK)
        return state;

    link_sort((void **) &account->record, next, set_next, cmp_date, reverse);
    return state;
}

inline static int cmp_sum(void *a, void *b)
{
    if (((struct consume_record *) a)->sum >\
        ((struct consume_record *) b)->sum)
        return 1;
    else if (((struct consume_record *) a)->sum <\
             ((struct consume_record *) b)->sum)
        return -1;
    return 0;
}

E_ACCOUNT_ACCESS_TYPE
account_sort_by_sum(struct account *account, int reverse)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_validate(account)) != E_OK)
        return state;

    link_sort((void **) &account->record, next, set_next, cmp_sum, reverse);
    return state;
}

inline static int cmp_transcation(void *a, void *b)
{
    return ((struct consume_record *) a)->pos->transcation -\
           ((struct consume_record *) b)->pos->transcation;
}

E_ACCOUNT_ACCESS_TYPE
account_sort_by_transcation(struct account *account, int reverse)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_validate(account)) != E_OK)
        return state;

    link_sort((void **) &account->record, next, set_next, cmp_transcation, reverse);
    return state;
}
