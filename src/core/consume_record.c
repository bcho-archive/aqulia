#include "consume_record.h"
#include "csv.h"
#include "sort.h"
#include "../debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_STR "consumed,received,sum,balance,consume_type,transcation,pos"

static inline void prepare_header(struct csv_header *header)
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

static inline struct csv_header *create_header()
{
    char buf[] = HEADER_STR;
    struct csv_header *header;

    header = csv_read_header_from_string(buf);
    prepare_header(header);
    return header;
}

static inline void assign_str(struct csv_row *row, char *str)
{
    row->svalue = malloc(sizeof(char) * (strlen(str) + 1));
    if (row == NULL)
        ERROR("malloc");
    strcpy(row->svalue, str);
}

static inline void assign_row(struct csv_row *row, struct consume_record *record)
{
    assign_str(csv_find_row(row, "consumed"), record->consumed);
    assign_str(csv_find_row(row, "received"), record->received);
    csv_find_row(row, "sum")->dvalue = record->sum;
    csv_find_row(row, "balance")->dvalue = record->balance;
    csv_find_row(row, "transcation")->ivalue = record->pos->transcation;
    assign_str(csv_find_row(row, "pos"), record->pos->name);
}

struct consume_record *consume_record_read(char *fname)
{
    struct consume_record *head, *prev, *p;
    FILE *stream;
    struct csv_header *header;
    struct csv_row *row;

    if ((stream = fopen(fname, "r")) == NULL)
        ERROR("open %s", fname);

    header = csv_read_header(stream);
    prepare_header(header);

    head = NULL;
    while ((row = csv_read_row(stream, header)) != NULL) {
        p = malloc(sizeof(struct consume_record));
        if (p == NULL)
            ERROR("malloc");
        p->pos = malloc(sizeof(struct pos_info));
        if (p->pos == NULL)
            ERROR("malloc");

        p->next = NULL;
        strcpy(p->consumed, csv_find_row(row, "consumed")->svalue);
        strcpy(p->received, csv_find_row(row, "received")->svalue);
        p->sum = csv_find_row(row, "sum")->dvalue;
        p->balance = csv_find_row(row, "balance")->dvalue;
        p->pos->transcation = csv_find_row(row, "transcation")->ivalue;
        strcpy(p->pos->name, csv_find_row(row, "pos")->svalue);

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

static void *next(void *cur) {
    return ((struct consume_record *) cur)->next;
}

static void set_next(void *node, void *value)
{
    ((struct consume_record *) node)->next = value;    
}

static int cmp(void *a, void *b)
{
    return strcmp(((struct consume_record *) a)->consumed,
                  ((struct consume_record *) b)->consumed);
}

void consume_record_save(char *fname, struct consume_record *record)
{
    struct csv_header *header;
    struct csv_row *row;
    FILE *stream;

    if ((stream = fopen(fname, "w")) == NULL)
        ERROR("write %s", fname);

    header = create_header();
    csv_write_header(stream, header);

    sort((void **) &record, next, set_next, cmp);
    for (;record != NULL;record = record->next) {
        row = csv_create_row(header);
        assign_row(row, record);
        csv_write_row(stream, row);
        csv_destory_row(row);
    }

    csv_destory_header(header);
    fclose(stream);
}

void consume_record_destory(struct consume_record *record)
{
    struct consume_record *p;

    while (record != NULL) {
        p = record->next;
        free(record->pos);
        free(record);
        record = p;
    }
}
