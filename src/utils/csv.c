#include "csv.h"
#include "str.h"
#include "debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 1024
#define DELIMITER ','

struct csv_header *
csv_read_header_from_string(char *s)
{
    char *str, *value, *buf, *dummy;
    struct csv_header *p, *prev, *head;

    buf = strdup(s);
    dummy = buf;
    head = NULL;
    trim(buf);
    for (str = buf, value = buf;str != NULL;value = str) {
        str = split(value, DELIMITER);
        if (value != NULL && strlen(value)) {
            trim(value);
            p = malloc(sizeof(struct csv_header));
            if (p == NULL)
                ERROR("malloc");
            p->name = malloc(sizeof(char) * (strlen(value) + 1));
            if (p->name == NULL)
                ERROR("malloc");
            strcpy(p->name, value);
            p->type = CSV_STRING;
            sprintf(p->fmt, "%%s");
            p->next = NULL;
            if (head == NULL)
                head = p;
            else
                prev->next = p;
            prev = p;
        }
    }
    free(dummy);

    return head;
}

struct csv_header *
csv_read_header(FILE *stream)
{
    char buf[LINE_MAX];

    if (fgets(buf, LINE_MAX, stream) == NULL)
        return NULL;

    return csv_read_header_from_string(buf);
}

struct csv_row *
csv_read_row(FILE *stream, struct csv_header *header)
{
    char *str, *value;
    char buf[LINE_MAX];
    struct csv_row *p, *head;
    struct csv_header *h;

    if (header == NULL)
        return NULL;

    if (fgets(buf, LINE_MAX, stream) == NULL)
        return NULL;

    head = csv_create_row(header);
    trim(buf);
    h = header;
    for (str = buf, value = buf;str != NULL;value = str, h = h->next) {
        str = split(value, DELIMITER);
        if (value != NULL) {
            trim(value);
            p = csv_find_row(head, h->name);
            if (p == NULL)
                ERROR("csv read row: %s", h->name);

            switch (h->type) {
                case CSV_INT:
                    p->ivalue = atoi(value);
                    break;
                case CSV_LONG:
                    p->lvalue = atol(value);
                    break;
                case CSV_DOUBLE:
                    p->dvalue = atof(value);
                    break;
                case CSV_STRING:
                default:
                    p->svalue = strdup(value);
                    if (p->svalue == NULL)
                        ERROR("malloc");
                    break;
            }
        }
    }

    return head;
}

/* create an empty row basic on header */
struct csv_row *
csv_create_row(struct csv_header *header)
{
    struct csv_row *head, *p, *prev;

    for (head = NULL;header != NULL;header = header->next) {
        p = malloc(sizeof(struct csv_row));
        if (p == NULL)
            ERROR("malloc");
        p->next = NULL;
        p->name = malloc(sizeof(char) * strlen(header->name));
        if (p->name == NULL)
            ERROR("malloc");
        strcpy(p->name, header->name);
        strcpy(p->fmt, header->fmt);
        p->type = header->type;

        if (head == NULL)
            head = p;
        else
            prev->next = p;
        prev = p;
    }

    return head;
}

void
csv_write_header(FILE *stream, struct csv_header *header)
{
    for (;header != NULL && header->next != NULL;header = header->next)
        fprintf(stream, "%s%c", header->name, DELIMITER);
    if (header != NULL)
        fprintf(stream, "%s\n", header->name);
}

static void
_write_field(FILE *stream, struct csv_row *row)
{
   switch (row->type) {
        case CSV_INT:
            fprintf(stream, row->fmt, row->ivalue);
            break;
        case CSV_LONG:
            fprintf(stream, row->fmt, row->lvalue);
            break;
        case CSV_DOUBLE:
            fprintf(stream, row->fmt, row->dvalue);
            break;
        case CSV_STRING:
        default:
            fprintf(stream, row->fmt, row->svalue);
            break;
   }
}

void
csv_write_row(FILE *stream, struct csv_row *row)
{
    for (;row != NULL && row->next != NULL;row = row->next) {
        _write_field(stream, row);
        fprintf(stream, "%c", DELIMITER);
    }
    if (row != NULL)
        _write_field(stream, row);
    fprintf(stream, "\n");
}

struct csv_header *
csv_find_header(struct csv_header *header, const char *key)
{
    struct csv_header *needle;

    for (needle = header;needle != NULL;needle = needle->next)
        if (strcmp(needle->name, key) == 0)
            return needle;
    return NULL;
}

struct csv_row *
csv_find_row(struct csv_row *row, const char *key)
{
    struct csv_row *needle;

    for (needle = row;needle != NULL;needle = needle->next)
        if (strcmp(needle->name, key) == 0)
            return needle;
    return NULL;
}

void
csv_destory_header(struct csv_header *header)
{
    struct csv_header *p;

    while (header != NULL) {
        p = header->next;
        free(header->name);
        free(header);
        header = p;
    }
}

void
csv_destory_row(struct csv_row *row)
{
    struct csv_row *p;

    while (row != NULL) {
        p = row->next;
        if (row->type == CSV_STRING)
            free(row->svalue);
        free(row);
        row = p;
    }
}
