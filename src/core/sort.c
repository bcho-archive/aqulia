#include "sort.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

static void *find_mid(void *list, void *(*next)(void *))
{
    void *slow, *fast;

    if (list == NULL)
        return NULL;

    slow = list; fast = list;
    while (next(fast) != NULL && next(next(fast)) != NULL) {
        slow = next(slow);
        fast = next(next(fast));
    }

    return slow;
}

static void *merge(void *left, void *right, void *(*next)(void *),
                   void (*set_next)(void *, void *),
                   int (*cmp)(void *, void *))
{
    void *head, *prev, *p;

    head = NULL;
    while (left != NULL && right != NULL) {
        if (cmp(left, right) > 0) {
            p = left;
            left = next(left);
        } else {
            p = right;
            right = next(right);
        }

        if (head == NULL)
            head = p;
        else
            set_next(prev, p);
        prev = p;
    }
    if (left == NULL)
        set_next(p, right);
    else
        set_next(p, left);

    return head;
}

inline static void *_sort(void *list, void *(*next)(void *),
                          void (*set_next)(void *, void *),
                          int (*cmp)(void *, void *))
{
    void *mid, *left, *right;

    if (list == NULL || next(list) == NULL)
        return list;

    mid = find_mid(list, next);
    left = list;
    right = next(mid);
    set_next(mid, NULL);

    return merge(_sort(left, next, set_next, cmp),
                 _sort(right, next, set_next, cmp),
                 next, set_next, cmp);
}

void sort(void **list, void *(*next)(void *), void (*set_next)(void *, void *),
          int (*cmp)(void *, void *))
{
    *list = _sort(*list, next, set_next, cmp);
}
