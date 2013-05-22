#ifndef CONSUME_RECORD_H
#define CONSUME_RECORD_H

#define DATETIME_SIZE 9

/* consume type */
typedef enum {
    CONSUME_POS
} consume_type_t;

/* pos machine information */
struct pos_info {
    char name[150];
    int transcation;

    struct pos_info *next;
};

/* consume record */
struct consume_record {
    char consumed[DATETIME_SIZE], received[DATETIME_SIZE];
    double sum;
    double balance;
    consume_type_t consume_type;
    struct pos_info *pos;

    struct consume_record *next;
};

struct consume_record *consume_record_read(char *fname);
void consume_record_save(char *fname, struct consume_record **rec);
void consume_record_destory(struct consume_record *record);

struct consume_record *consume_record_create(const char *consumed,
        const char *received, double sum, double balance,
        consume_type_t consume_type, const char *pos,
        struct consume_record *record);

/* pos info */

/* get all pos machine infomations from consume record */
struct pos_info *pos_infos_get(struct consume_record *record);
struct pos_info *pos_infos_query_by_name(const char *name,
                                         struct pos_info *infos);
struct pos_info *pos_infos_create(const char *name, int transcation);
struct pos_info *pos_infos_dup(struct pos_info *info);
void pos_infos_destory(struct pos_info *infos);

#endif
