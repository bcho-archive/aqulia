#include "personal_info.h"
#include "csv.h"
#include "../debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void prepare_header(struct csv_header *header)
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

static inline struct csv_header *create_header()
{
    char buf[] = "cardno,expire,balance,state,faculty";
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

static inline void assign_row(struct csv_row *row, struct personal_info *info)
{
    csv_find_row(row, "cardno")->ivalue = info->cardno;
    assign_str(csv_find_row(row, "expire"), info->expire);
    csv_find_row(row, "balance")->dvalue = info->balance;
    csv_find_row(row, "state")->ivalue = info->state;
    assign_str(csv_find_row(row, "faculty"), info->faculty);
}

struct personal_info *personal_read(char *fname)
{
    struct personal_info *info;
    FILE *stream;
    struct csv_header *header;
    struct csv_row *row;

    if ((stream = fopen(fname, "r")) == NULL)
        ERROR("open %s", fname);

    info = malloc(sizeof(struct personal_info));
    if (info == NULL)
        ERROR("malloc");

    header = csv_read_header(stream);
    prepare_header(header);

    row = csv_read_row(stream, header);
    info->cardno = csv_find_row(row, "cardno")->ivalue;
    strcpy(info->expire, csv_find_row(row, "expire")->svalue);
    info->balance = csv_find_row(row, "balance")->dvalue;
    info->state = csv_find_row(row, "state")->ivalue;
    strcpy(info->faculty, csv_find_row(row, "faculty")->svalue);

    csv_destory_row(row);
    csv_destory_header(header);
    fclose(stream);

    return info;
}

void personal_save(char *fname, struct personal_info *info)
{
    struct csv_header *header;
    struct csv_row *row;
    FILE *stream;

    if ((stream = fopen(fname, "w")) == NULL)
        ERROR("write %s", fname);

    header = create_header();
    row = csv_create_row(header);
    assign_row(row, info);

    csv_write_header(stream, header);
    csv_write_row(stream, row);

    csv_destory_row(row);
    csv_destory_header(header);
    fclose(stream);
}

void personal_destory(struct personal_info *info)
{
    if (info != NULL)
        free(info);
}
