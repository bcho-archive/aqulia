#include "account.h"
#include "../debug.h"

#include <stdlib.h>
#include <assert.h>

E_ACCOUNT_CONSUME_TYPE account_consume(struct account *account,
                                       const char *consumed, const char *received,
                                       double sum, consume_type_t consume_type,
                                       const char *pos)
{
    struct consume_record *record;
    double balance;

    /* validate */
    if (account_validate(account) == E_FREEZE)
        return E_CONSUME_FREEZED;
    balance = account->info->balance - sum;
    if (balance < 0 || account->info->balance <= 0)
        return E_CONSUME_CANTPAY;

    account->info->balance = balance;
    record = consume_record_create(consumed, received, sum, balance,
                                   consume_type, pos, account->record);
    record->next = account->record;
    account->record = record;
    account_save(account);

    return E_CONSUME_OK;
}

E_ACCOUNT_CONSUME_TYPE account_consume_delete(struct account *account,
                                              struct consume_record *record)
{
    struct consume_record *prev, *undo;

    if (account_validate(account) == E_FREEZE)
        return E_CONSUME_FREEZED;

    /* remove from consume history */
    for (prev = account->record;
         prev != NULL && prev->next != NULL && prev->next != record;
         prev = prev->next)
        ;

    if (prev == NULL || prev->next == NULL)
        return E_CONSUME_NOTFOUND;
    prev->next = record->next;
    account_save(account);

    /* save to undo */
    undo = consume_record_read(account->consume_undo);
    record->next = undo;
    undo = record;
    consume_record_save(account->consume_undo, &undo);
    consume_record_destory(undo);

    return E_CONSUME_OK;
}
