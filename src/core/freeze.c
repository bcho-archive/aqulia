#include "account.h"
#include "../utils/debug.h"

#include <stdio.h>

E_ACCOUNT_ACCESS_TYPE
account_freeze(struct account *account)
{
    E_ACCOUNT_ACCESS_TYPE state;
    FILE *stream;

    if ((state = account_validate(account)) == E_FREEZE)
        return state;

    if ((stream = fopen(account->ffreeze, "w")) == NULL)
        ERROR("write freeze: %s", account->ffreeze);
    fprintf(stream, "1");
    fclose(stream);

    account->state = CARD_FREEZED;
    account_save_force(account);

    return E_FREEZE;
}

E_ACCOUNT_ACCESS_TYPE
account_unfreeze(struct account *account)
{
    E_ACCOUNT_ACCESS_TYPE state;
    FILE *stream;

    if ((state = account_validate(account)) != E_FREEZE)
        return state;

    if ((stream = fopen(account->ffreeze, "w")) == NULL)
        ERROR("write freeze: %s", account->ffreeze);
    fprintf(stream, "0");
    fclose(stream);

    account->state = CARD_USABLE;
    account_save(account);

    return E_OK;
}
