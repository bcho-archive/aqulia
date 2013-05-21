#include "account.h"

E_ACCOUNT_READ_TYPE account_query_by_date(struct account *account, char *date,
                                          struct consume_record **record)
{
    if (account_validate(account) == E_FREEZE)
        return E_FREEZE;
}
