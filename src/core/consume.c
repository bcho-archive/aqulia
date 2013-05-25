#include "account.h"

#include <stdlib.h>

E_ACCOUNT_CONSUME_TYPE
account_consume(struct account *account,
                const char *consumed, const char *received,
                double sum, CONSUME_TYPE_T consume_type, const char *pos)
{
    struct consume_record *new;
    double balance;

    if (account_validate(account) == E_FREEZE)
        return E_CONSUME_FREEZED;

    if (consume_type == CONSUME_POS) {
        balance = account->balance - sum;
        if (balance < 0 || account->balance <= 0)
            return E_CONSUME_CANTPAY;
    } else if (consume_type == CONSUME_RECHARGE) {
        balance = account->balance + sum;
    }

    account->balance = balance;
    new = consume_record_create(consumed, received, sum, balance,
                                consume_type, pos, account->record);
    new->next = account->record->next;
    account->record->next = new;
    account_save(account);

    return E_CONSUME_OK;
}

E_ACCOUNT_CONSUME_TYPE
account_consume_delete(struct account *account, struct consume_record *record)
{
    struct consume_record *prev, *undo;

    if (account_validate(account) == E_FREEZE)
        return E_CONSUME_FREEZED;

    for (prev = account->record;
         prev != NULL && prev->next != NULL && prev->next != record;
         prev = prev->next)
        ;

    if (prev == NULL || prev->next == NULL && record != account->record)
        return E_CONSUME_NOTFOUND;

    if (record != account->record)
        prev->next = record->next;
    else
        account->record = record->next;
    account_save(account);

    undo = consume_record_read(account->fconsume_undo);
    record->next = undo;
    undo = record;
    consume_record_save(account->fconsume_undo, &undo);
    consume_record_destory(undo);

    return E_CONSUME_OK;
}
