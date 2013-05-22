#include "account.h"
#include "../debug.h"

#include <stdio.h>

E_ACCOUNT_READ_TYPE account_freeze(struct account *account)
{
    E_ACCOUNT_READ_TYPE state;
    FILE *stream;

    if ((state = account_validate(account)) == E_FREEZE)
        return state;

    if ((stream = fopen(account->freeze_file, "w")) == NULL)
        ERROR("open %s", account->freeze_file);

    fprintf(stream, "1");
    fclose(stream);
    
    account->info->state = CARD_FREEZED;
    account_save(account);

    return E_FREEZE;
}

E_ACCOUNT_READ_TYPE account_unfreeze(struct account *account)
{
    E_ACCOUNT_READ_TYPE state;
    FILE *stream;

    if ((state = account_validate(account)) != E_FREEZE)
        return state;

    if ((stream = fopen(account->freeze_file, "w")) == NULL)
        ERROR("open %s", account->freeze_file);
    fprintf(stream, "0");
    fclose(stream);

    account->info->state = CARD_USABLE;
    account_save(account);

    return E_OK;
}
