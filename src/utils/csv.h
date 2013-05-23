#ifndef CSV_H
#define CSV_H

#include <stdio.h>

typedef enum {
   CSV_INT,
   CSV_LONG,
   CSV_DOUBLE,
   CSV_STRING
} field_type_t;

struct csv_header {
    char *name;
    field_type_t type;
    char fmt[10];

    struct csv_header *next;
};

struct csv_row {
    char *name;
    field_type_t type;
    char fmt[10];
    union {
        int ivalue;
        long lvalue;
        double dvalue;
        char *svalue;
    };

    struct csv_row *next;
};

struct csv_header *csv_read_header_from_string(char *buf);
struct csv_header *csv_read_header(FILE *stream);
struct csv_row *csv_read_row(FILE *stream, struct csv_header *header);
struct csv_row *csv_create_row(struct csv_header *header);
void csv_write_header(FILE *stream, struct csv_header *header);
void csv_write_row(FILE *stream, struct csv_row *row);
struct csv_header *csv_find_header(struct csv_header *header, const char *key);
struct csv_row *csv_find_row(struct csv_row *row, const char *key);
void csv_destory_header(struct csv_header *header);
void csv_destory_row(struct csv_row *row);

#endif
