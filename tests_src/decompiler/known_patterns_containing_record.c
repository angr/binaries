/*
 * Test binary for angr's KnownPatternFinder: the CONTAINING_RECORD macro as
 * used in Windows driver code (LIST_ENTRY walk). CONTAINING_RECORD compiles to
 * a pointer-minus-constant, which the decompiler should recognize and outline
 * back into a CONTAINING_RECORD(...) call.
 *
 * Built as a Windows PE with mingw-w64 so the real winnt.h definitions of
 * LIST_ENTRY and CONTAINING_RECORD are used.
 *
 * Build: make -f Makefile.known_patterns
 */
#include <windows.h>

typedef struct _DEVICE_REC {
    int a;
    int pad;
    LIST_ENTRY entry; /* offsetof(DEVICE_REC, entry) == 8 */
    int b;
} DEVICE_REC;

/* noinline consumer so the CONTAINING_RECORD result materializes as a
 * pointer-minus-constant instead of being folded into field displacements. */
__attribute__((noinline)) int consume(DEVICE_REC *r)
{
    return r->a + r->b;
}

__attribute__((noinline)) int sum_list(LIST_ENTRY *head)
{
    int s = 0;
    LIST_ENTRY *e;
    for (e = head->Flink; e != head; e = e->Flink) {
        DEVICE_REC *r = CONTAINING_RECORD(e, DEVICE_REC, entry);
        s += consume(r);
    }
    return s;
}

int main(void)
{
    LIST_ENTRY head = { &head, &head };
    return sum_list(&head);
}
