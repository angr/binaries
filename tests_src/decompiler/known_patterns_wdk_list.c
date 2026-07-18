/*
 * Test binary for angr's KnownPatternFinder: the Windows LIST_ENTRY doubly-
 * linked-list idioms as used throughout WDK drivers. The inline helpers below
 * reproduce the standard WDK/ntddk definitions verbatim (mingw's winnt.h ships
 * LIST_ENTRY and CONTAINING_RECORD but not these helpers). Each is exercised
 * through a noinline wrapper so the inlined shape materializes.
 *
 * Build: make -f Makefile.known_patterns
 * x86_64-w64-mingw32-gcc -O2 -fno-tree-slp-vectorize -> Windows PE.
 * (SLP vectorization off so the two-pointer writes stay scalar stores rather
 * than a single 16-byte SSE store.)
 */
#include <windows.h>

FORCEINLINE void InitializeListHead(PLIST_ENTRY head)
{
    head->Flink = head->Blink = head;
}

FORCEINLINE BOOLEAN IsListEmpty(const LIST_ENTRY *head)
{
    return (BOOLEAN)(head->Flink == head);
}

FORCEINLINE void RemoveEntryList(PLIST_ENTRY entry)
{
    PLIST_ENTRY blink = entry->Blink;
    PLIST_ENTRY flink = entry->Flink;
    blink->Flink = flink;
    flink->Blink = blink;
}

FORCEINLINE void InsertTailList(PLIST_ENTRY head, PLIST_ENTRY entry)
{
    PLIST_ENTRY blink = head->Blink;
    entry->Flink = head;
    entry->Blink = blink;
    blink->Flink = entry;
    head->Blink = entry;
}

__attribute__((noinline)) void wdk_init(PLIST_ENTRY head)
{
    InitializeListHead(head);
}

__attribute__((noinline)) int wdk_is_empty(const LIST_ENTRY *head)
{
    return IsListEmpty(head);
}

/* returns int 0 so the inlined helper's temporaries do not leak into the
 * return register (the region has no live-out value) */
__attribute__((noinline)) int wdk_remove(PLIST_ENTRY entry)
{
    RemoveEntryList(entry);
    return 0;
}

__attribute__((noinline)) void wdk_insert_tail(PLIST_ENTRY head, PLIST_ENTRY entry)
{
    InsertTailList(head, entry);
}

int main(void)
{
    LIST_ENTRY head, a, b;
    wdk_init(&head);
    wdk_insert_tail(&head, &a);
    wdk_insert_tail(&head, &b);
    wdk_remove(&a);
    return wdk_is_empty(&head);
}
