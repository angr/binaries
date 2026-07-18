/*
 * Test binary for angr's KnownPatternFinder: the Linux kernel list_head
 * doubly-linked-list idioms. list_head {next, prev} has the same two-pointer
 * layout as Windows LIST_ENTRY {Flink, Blink}, so the same KnownPatterns match
 * both. The inline helpers reproduce the kernel <linux/list.h> bodies; each is
 * exercised through a noinline wrapper.
 *
 * Build: make -f Makefile.known_patterns
 * gcc -O2 -fno-tree-slp-vectorize -> Linux ELF (SLP off so the two-pointer
 * writes stay scalar stores rather than a single 16-byte SSE store).
 */
#include <stddef.h>

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

static inline void list_del(struct list_head *entry)
{
    struct list_head *prev = entry->prev;
    struct list_head *next = entry->next;
    next->prev = prev;
    prev->next = next;
}

static inline void list_add_tail(struct list_head *entry, struct list_head *head)
{
    struct list_head *prev = head->prev;
    entry->next = head;
    entry->prev = prev;
    prev->next = entry;
    head->prev = entry;
}

__attribute__((noinline)) void lx_init(struct list_head *head)
{
    INIT_LIST_HEAD(head);
}

__attribute__((noinline)) int lx_is_empty(const struct list_head *head)
{
    return list_empty(head);
}

/* returns int 0 so the inlined helper's temporaries do not leak into the
 * return register (the region has no live-out value) */
__attribute__((noinline)) int lx_del(struct list_head *entry)
{
    list_del(entry);
    return 0;
}

__attribute__((noinline)) void lx_add_tail(struct list_head *entry, struct list_head *head)
{
    list_add_tail(entry, head);
}

int main(void)
{
    struct list_head head, a, b;
    lx_init(&head);
    lx_add_tail(&a, &head);
    lx_add_tail(&b, &head);
    lx_del(&a);
    return lx_is_empty(&head);
}
