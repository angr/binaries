#include <stdio.h>
#include <stdlib.h>

typedef struct elem_ {
    int val;
    struct elem_* next;
} elem;

int sum(elem* linked_list)
{
    elem* p = linked_list;
    int n = 0;

    while (p != NULL) {
        n += p->val;
        p = p->next;
    }

    return n;
}

elem* alloc()
{
    elem* elem_0 = (elem*)malloc(sizeof(elem));
    elem_0->val = 0;
    elem_0->next = NULL;

    return elem_0;
}

int main()
{
    elem* linked_list = alloc();

    // Allocate some elements
    elem* elem_0 = alloc();
    elem* elem_1 = alloc();
    elem* elem_2 = alloc();

    elem_0->val = 1;
    elem_1->val = 2;
    elem_2->val = 3;

    // Link them together!
    linked_list->next = elem_0;
    elem_0->next = elem_1;
    elem_1->next = elem_2;

    printf("The sum is %d.\n", sum(linked_list));
}
