/*
 * Test binary for angr's KnownPatternFinder: Linux kernel <linux/err.h>
 * error-pointer macros and the <linux/list.h> insertion / deletion / hlist
 * idioms that are not yet covered by known_patterns_linux_list.c.
 *
 * The inline helpers reproduce the kernel bodies verbatim (READ_ONCE /
 * WRITE_ONCE are dropped: they are volatile accesses that do not change the
 * lifted AIL shape, only the compiler's freedom to reorder). Each idiom is
 * exercised through a noinline wrapper so the inlined shape materializes.
 *
 * Build:
 * gcc -O2 -fno-tree-slp-vectorize -o known_patterns_kernel_macros \
 *     known_patterns_kernel_macros.c
 * (SLP vectorization off so the two-pointer writes stay scalar stores rather
 * than a single 16-byte SSE store.)
 */
#include <stddef.h>

/* --- <linux/err.h> ------------------------------------------------------ */

#define MAX_ERRNO 4095
#define IS_ERR_VALUE(x) ((unsigned long)(void *)(x) >= (unsigned long)-MAX_ERRNO)

static inline void *ERR_PTR(long error)
{
    return (void *)error;
}

static inline long PTR_ERR(const void *ptr)
{
    return (long)ptr;
}

static inline int IS_ERR(const void *ptr)
{
    return IS_ERR_VALUE((unsigned long)ptr);
}

static inline int IS_ERR_OR_NULL(const void *ptr)
{
    return !ptr || IS_ERR_VALUE((unsigned long)ptr);
}

static inline int PTR_ERR_OR_ZERO(const void *ptr)
{
    if (IS_ERR(ptr))
        return PTR_ERR(ptr);
    return 0;
}

/* --- <linux/poison.h> --------------------------------------------------- */

#define POISON_POINTER_DELTA 0xdead000000000000UL
#define LIST_POISON1 ((void *)(0x100UL + POISON_POINTER_DELTA))
#define LIST_POISON2 ((void *)(0x122UL + POISON_POINTER_DELTA))

/* --- <linux/list.h> ----------------------------------------------------- */

struct list_head {
    struct list_head *next;
    struct list_head *prev;
};

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;
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

static inline void __list_add(struct list_head *new, struct list_head *prev, struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void __list_del_entry(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

static inline void list_del(struct list_head *entry)
{
    __list_del_entry(entry);
    entry->next = LIST_POISON1;
    entry->prev = LIST_POISON2;
}

static inline void list_del_init(struct list_head *entry)
{
    __list_del_entry(entry);
    INIT_LIST_HEAD(entry);
}

static inline void list_move(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add(list, head);
}

static inline void list_move_tail(struct list_head *list, struct list_head *head)
{
    __list_del_entry(list);
    list_add_tail(list, head);
}

static inline void __list_splice(const struct list_head *list, struct list_head *prev, struct list_head *next)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

static inline void list_splice(const struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head, head->next);
}

static inline int hlist_empty(const struct hlist_head *h)
{
    return !h->first;
}

static inline void __hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;

    *pprev = next;
    if (next)
        next->pprev = pprev;
}

static inline void hlist_del(struct hlist_node *n)
{
    __hlist_del(n);
    n->next = LIST_POISON1;
    n->pprev = LIST_POISON2;
}

static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_node *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    h->first = n;
    n->pprev = &h->first;
}

/* --- err.h wrappers ----------------------------------------------------- */

__attribute__((noinline)) int k_is_err(const void *p)
{
    return IS_ERR(p);
}

__attribute__((noinline)) int k_is_err_or_null(const void *p)
{
    return IS_ERR_OR_NULL(p);
}

__attribute__((noinline)) long k_ptr_err(const void *p)
{
    return PTR_ERR(p);
}

__attribute__((noinline)) void *k_err_ptr(long e)
{
    return ERR_PTR(e);
}

__attribute__((noinline)) int k_ptr_err_or_zero(const void *p)
{
    return PTR_ERR_OR_ZERO(p);
}

/* IS_ERR used as a branch condition rather than a returned value */
__attribute__((noinline)) long k_is_err_branch(const void *p, long fallback)
{
    if (IS_ERR(p))
        return PTR_ERR(p);
    return fallback;
}

/* --- list.h wrappers ---------------------------------------------------- */

/* the void-returning wrappers return int 0 where the region would otherwise
 * leak a temporary into the return register (see known_patterns_linux_list.c) */

__attribute__((noinline)) int k_list_add(struct list_head *entry, struct list_head *head)
{
    list_add(entry, head);
    return 0;
}

__attribute__((noinline)) int k_list_add_tail(struct list_head *entry, struct list_head *head)
{
    list_add_tail(entry, head);
    return 0;
}

__attribute__((noinline)) int k_list_del_poison(struct list_head *entry)
{
    list_del(entry);
    return 0;
}

__attribute__((noinline)) int k_list_del_init(struct list_head *entry)
{
    list_del_init(entry);
    return 0;
}

__attribute__((noinline)) int k_list_move(struct list_head *entry, struct list_head *head)
{
    list_move(entry, head);
    return 0;
}

__attribute__((noinline)) int k_list_move_tail(struct list_head *entry, struct list_head *head)
{
    list_move_tail(entry, head);
    return 0;
}

__attribute__((noinline)) int k_list_splice(const struct list_head *list, struct list_head *head)
{
    list_splice(list, head);
    return 0;
}

/* --- hlist wrappers ----------------------------------------------------- */

__attribute__((noinline)) int k_hlist_empty(const struct hlist_head *h)
{
    return hlist_empty(h);
}

__attribute__((noinline)) int k_hlist_del(struct hlist_node *n)
{
    hlist_del(n);
    return 0;
}

__attribute__((noinline)) int k_hlist_del_plain(struct hlist_node *n)
{
    __hlist_del(n);
    return 0;
}

__attribute__((noinline)) int k_hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    hlist_add_head(n, h);
    return 0;
}

int main(void)
{
    struct list_head head, other, a, b;
    struct hlist_head hh = {NULL};
    struct hlist_node hn, hn2;
    int rc = 0;

    INIT_LIST_HEAD(&head);
    INIT_LIST_HEAD(&other);
    rc += k_list_add(&a, &head);
    rc += k_list_add_tail(&b, &head);
    rc += k_list_move(&a, &other);
    rc += k_list_move_tail(&b, &other);
    rc += k_list_splice(&other, &head);
    rc += k_list_del_init(&a);
    rc += k_list_del_poison(&b);

    rc += k_hlist_add_head(&hn, &hh);
    rc += k_hlist_add_head(&hn2, &hh);
    rc += k_hlist_empty(&hh);
    rc += k_hlist_del_plain(&hn2);
    rc += k_hlist_del(&hn);

    rc += k_is_err(&head);
    rc += k_is_err_or_null(&head);
    rc += (int)k_ptr_err(&head);
    rc += k_err_ptr(rc) != NULL;
    rc += k_ptr_err_or_zero(&head);
    rc += (int)k_is_err_branch(&head, 7);
    return rc;
}
