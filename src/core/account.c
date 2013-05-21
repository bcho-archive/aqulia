#include "account.h"
#include "personal_info.h"
#include "consume_record.h"
#include "../debug.h"
#include "../utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

E_ACCOUNT_READ_TYPE account_read(int cardno, struct account **acc)
{
    char basename[50];
    struct account *account;

    account = malloc(sizeof(struct account));
    if (account == NULL)
        ERROR("malloc");
    
    sprintf(basename, "%d/"PERSONAL_INFO_FILE, cardno);
    account->personal_file = strdup(basename);
    sprintf(basename, "%d/"CONSUME_RECORD_FILE, cardno);
    account->consume_record = strdup(basename);
    sprintf(basename, "%d/"FREEZE_FILE, cardno);
    account->freeze_file = strdup(basename);
    sprintf(basename, "%d/"CONSUME_UNDO_FILE, cardno);
    account->consume_undo = strdup(basename);

    account->info = personal_read(account->personal_file);
    account->record = consume_record_read(account->consume_record);
    *acc = account;

    return account_validate(account);
}

void account_save(struct account *account)
{
    personal_save(account->personal_file, account->info);
    consume_record_save(account->consume_record, account->record);
}

void account_destory(struct account *account)
{
    if (account == NULL)
        return;
    free(account->personal_file);
    free(account->freeze_file);
    free(account->consume_record);
    free(account->consume_undo);
    personal_destory(account->info);
    consume_record_destory(account->record);
    free(account);
}
