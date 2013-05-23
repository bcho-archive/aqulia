#include "consume_record.h"
#include "../utils/csv.h"
#include "../utils/debug.h"
#include "../utils/misc.h"
#include "../utils/link_sort.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define DEFAULT_HEADER "consumed,received,sum,balance,consume_type,transcation,pos"

/* sorting helpers */
static void *
next(void *cur) {
    return ((struct consume_record *) cur)->next;
}

static void
set_next(void *node, void *value)
{
    ((struct consume_record *) node)->next = value;    
}

static int
cmp(void *a, void *b)
{
    struct consume_record c, d;
    int ret;

    /* TODO FIXME
     * strange behaviour when sorting
     */
    c = *((struct consume_record *) a);
    d = *((struct consume_record *) b);
    ret = strcmp(c.consumed, d.consumed);
    if (ret == 0)
        if (c.balance > d.balance)
            return -1;
        if (c.balance < d.balance)
            return 1;
    return ret;
}

/* declare csv col type */
inline static void
prepare_header(struct csv_header *header)
{
    struct csv_header *p;

    p = csv_find_header(header, "sum");
    p->type = CSV_DOUBLE;
    sprintf(p->fmt, "%%.2f");
    p = csv_find_header(header, "balance");
    p->type = CSV_DOUBLE;
    sprintf(p->fmt, "%%.2f");
    p = csv_find_header(header, "consume_type");
    p->type = CSV_INT;
    sprintf(p->fmt, "%%d");
    p = csv_find_header(header, "transcation");
    p->type = CSV_INT;
    sprintf(p->fmt, "%%d");
}

/* convert a record into a row */
inline static void
prepare_row(struct csv_row *row, struct consume_record *record)
{
    csv_find_row(row, "consumed")->svalue = strdup(record->consumed);
    csv_find_row(row, "received")->svalue = strdup(record->consumed);
    csv_find_row(row, "sum")->dvalue = record->sum;
    csv_find_row(row, "balance")->dvalue = record->balance;
    csv_find_row(row, "consume_type")->ivalue = record->type;
    csv_find_row(row, "transcation")->ivalue = record->pos->transcation;
    csv_find_row(row, "pos")->svalue = strdup(record->pos->name);
}

struct consume_record *
consume_record_read(char *fname)
{
    struct consume_record *head, *prev, *p;
    FILE *stream;
    struct csv_header *header;
    struct csv_row *row;

    touch_file(fname);
    if ((stream = fopen(fname, "r")) == NULL)
        ERROR("read consume record: %s", fname);

    header = csv_read_header(stream);
    if (header == NULL)
        header = csv_read_header_from_string(DEFAULT_HEADER);
    prepare_header(header);

    head = NULL;
    while ((row = csv_read_row(stream, header)) != NULL) {
        if ((p = malloc(sizeof(struct consume_record))) == NULL)
            ERROR("malloc");

        p->next = NULL;
        p->consumed = strdup(csv_find_row(row, "consumed")->svalue);
        p->received = strdup(csv_find_row(row, "received")->svalue);
        p->sum = csv_find_row(row, "sum")->dvalue;
        p->balance = csv_find_row(row, "balance")->dvalue;
        p->type = csv_find_row(row, "consume_type")->ivalue;
        if ((p->pos = malloc(sizeof(struct POS))) == NULL)
            ERROR("malloc");
        p->pos->name = strdup(csv_find_row(row, "pos")->svalue);
        p->pos->transcation = csv_find_row(row, "transcation")->ivalue;

        /* create link list */
        if (head == NULL)
            head = p;
        else
            prev->next = p;
        prev = p;

        csv_destory_row(row);
    }
    csv_destory_header(header);

    return head;
}

void
consume_record_save(char *fname, struct consume_record **rec)
{
    struct consume_record *copy;
    struct csv_header *header;
    struct csv_row *row;
    FILE *stream;

    if ((stream = fopen(fname, "w")) == NULL)
        ERROR("write consume record: %s", fname);

    header = csv_read_header_from_string(DEFAULT_HEADER);
    prepare_header(header);
    csv_write_header(stream, header);

    /* sort by date & balance before saving to file */
    link_sort((void **) rec, next, set_next, cmp, 0);
    for (copy = *rec;copy != NULL;copy = copy->next) {
        row = csv_create_row(header);
        prepare_row(row, copy);
        csv_write_row(stream, row);
        csv_destory_row(row);
    }
    csv_destory_header(header);
    fclose(stream);
}

void
consume_record_destory(struct consume_record *record)
{
    struct consume_record *prev;

    while (record != NULL) {
        prev = record->next;
        if (record->pos != NULL) {
            free(record->pos->name);
            free(record->pos);
        }
        free(record->consumed);
        free(record->received);
        free(record);

        record = prev;
    }
}

struct consume_record *
consume_record_create(const char *consumed, const char *received,
                      double sum, double balance,
                      CONSUME_TYPE_T type, const char *pos_name,
                      struct consume_record *prev_record)
{
    struct consume_record *new;
    struct POS *pos, *p;

    if ((new = malloc(sizeof(struct consume_record))) == NULL)
        ERROR("malloc");
    new->next = NULL;
    new->sum = sum;
    new->balance = balance;
    new->consumed = strdup(consumed);
    new->received = strdup((received == NULL) ? consumed : received);
    new->type = type;
    if ((new->pos = malloc(sizeof(struct POS))) == NULL)
        ERROR("malloc");
    pos = POS_get(prev_record);
    p = POS_query_by_name(pos_name, pos);
    if (p == NULL)
        p = POS_create(pos_name, 0);
    else
        p = POS_dup(p);
    p->transcation++;
    new->pos = p;
    POS_destory(pos);

    return new;
}
