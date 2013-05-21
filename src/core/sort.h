#ifndef SORT_H
#define SORT_H

void sort(void **list, void *(*next)(void *), void (*set_next)(void *, void *),
          int (*cmp)(void *, void *));

#endif
