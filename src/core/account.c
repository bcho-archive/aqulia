#include "account.h"
#include "../utils/csv.h"
#include "../utils/debug.h"
#include "../utils/misc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_HEADER "cardno,expire,balance,state,faculty"

/* declare csv col type */
inline static void
prepare_header(struct csv_header *header)
{
    struct csv_header *p;

    p = csv_find_header(header, "cardno");
    p->type = CSV_INT;
    sprintf(p->fmt, "%%d");
    p = csv_find_header(header, "balance");
    p->type = CSV_DOUBLE;
    sprintf(p->fmt, "%%.2f");
    p = csv_find_header(header, "state");
    p->type = CSV_INT;
    sprintf(p->fmt, "%%d");
}

/* convert a account into a row */
inline static void
prepare_row(struct csv_row *row, struct account *account)
{
    csv_find_row(row, "cardno")->ivalue = account->cardno;
    csv_find_row(row, "expire")->svalue = strdup(account->expire);
    csv_find_row(row, "balance")->dvalue = account->balance;
    csv_find_row(row, "state")->ivalue = account->state;
    csv_find_row(row, "faculty")->svalue = strdup(account->faculty);
}

inline static void
personal_read(char *fname, struct account *account)
{
    struct csv_header *header;
    struct csv_row *row;
    FILE *stream;

    touch_file(fname);
    if ((stream = fopen(fname, "r")) == NULL)
        ERROR("read personal %s", fname);

    header = csv_read_header(stream);
    if (header == NULL)
        header = csv_read_header_from_string(DEFAULT_HEADER);
    prepare_header(header);

    row = csv_read_row(stream, header);
    if (row == NULL)
        return;

    account->cardno = csv_find_row(row, "cardno")->ivalue;
    account->expire = strdup(csv_find_row(row, "expire")->svalue);
    account->balance = csv_find_row(row, "balance")->dvalue;
    account->state = csv_find_row(row, "state")->ivalue;
    account->faculty = strdup(csv_find_row(row, "faculty")->svalue);

    csv_destory_row(row);
    csv_destory_header(header);
    fclose(stream);
}

inline static void
personal_save(char *fname, struct account *account)
{
    struct csv_header *header;
    struct csv_row *row;
    FILE *stream;

    if ((stream = fopen(fname, "w")) == NULL)
        ERROR("write personal: %s", fname);

    header = csv_read_header_from_string(DEFAULT_HEADER);
    prepare_header(header);
    row = csv_create_row(header);
    prepare_row(row, account);

    csv_write_header(stream, header);
    csv_write_row(stream, row);

    csv_destory_row(row);
    csv_destory_header(header);
    fclose(stream);
}

E_ACCOUNT_ACCESS_TYPE
account_read(int cardno, struct account **account_return)
{
    struct account *account;
    char base[50];

    if ((account = malloc(sizeof(struct account))) == NULL)
        ERROR("malloc");
    /* TODO portable path building */
    sprintf(base, "%d/"FPERSONAL, cardno);
    account->fpersonal = strdup(base);
    sprintf(base, "%d/"FFREEZE, cardno);
    account->ffreeze = strdup(base);
    sprintf(base, "%d/"FCONSUME_RECORD, cardno);
    account->fconsume_record = strdup(base);
    sprintf(base, "%d/"FCONSUME_UNDO, cardno);
    account->fconsume_undo = strdup(base);

    /* read personal infomation */
    personal_read(account->fpersonal, account);
    /* read consume record */
    account->record = consume_record_read(account->fconsume_record);
    *account_return = account;

    return account_validate(account);
}

E_ACCOUNT_ACCESS_TYPE
account_save(struct account *account)
{
    E_ACCOUNT_ACCESS_TYPE state;

    if ((state = account_validate(account)) != E_OK)
        return state;

    consume_record_save(account->fconsume_record, &account->record);
    personal_save(account->fpersonal, account);
    return state;
}

void 
account_save_force(struct account *account)
{
    consume_record_save(account->fconsume_record, &account->record);
    personal_save(account->fpersonal, account);
}

void
account_destory(struct account *account)
{
    if (account != NULL) {
        free(account->fpersonal);
        free(account->ffreeze);
        free(account->fconsume_record);
        free(account->fconsume_undo);
        free(account->expire);
        free(account->faculty);
        consume_record_destory(account->record);
    }
}
