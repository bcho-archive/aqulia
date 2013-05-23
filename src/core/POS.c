#include "consume_record.h"
#include "../utils/debug.h"

#include <stdlib.h>
#include <string.h>

struct POS *
POS_get(struct consume_record *record)
{
    struct POS *head, *prev, *p;

    head = NULL;
    for (;record != NULL;record = record->next) {
         p = POS_query_by_name(record->pos->name, head);
         if (p == NULL) {   /* not found then create one */
             p = POS_dup(record->pos);

            /* create link list */
            if (head == NULL)
                head = p;
            else
                prev->next = p;
            prev = p;
         } else {           /* update transcation */
             if (p->transcation < record->pos->transcation)
                 p->transcation = record->pos->transcation;
         }
    }

    return head;
}

struct POS *
POS_query_by_name(const char *name, struct POS *pos)
{
    for (;pos != NULL && strcmp(pos->name, name) != 0;pos = pos->next)
        ;
    return pos;
}

struct POS *
POS_create(const char *name, int transcation)
{
    struct POS *new;

    if ((new = malloc(sizeof(struct POS))) == NULL)
        ERROR("malloc");
    new->name = strdup(name);
    new->transcation = transcation;
    new->next = NULL;
    
    return new;
}

struct POS *
POS_dup(struct POS *pos)
{
    return POS_create(pos->name, pos->transcation);
}

void
POS_destory(struct POS *pos)
{
    struct POS *prev;

    while (pos != NULL) {
        prev = pos->next;
        free(pos->name);
        free(pos);
        pos = prev;
    }
}
