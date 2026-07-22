/*
 * Test binary for angr's KnownPatternFinder: the glibc <ctype.h> macros, plus
 * errno (which is not a pattern -- see below).
 *
 * These are not range comparisons. glibc classifies a character by indexing a
 * per-locale table:
 *
 *   #define __isctype(c, type) ((*__ctype_b_loc ())[(int) (c)] & (unsigned short int) type)
 *
 * so every predicate is a call to __ctype_b_loc, a load of the table pointer it
 * returns, a 16-bit indexed load, and a mask -- one shape with twelve constants.
 * tolower/toupper are the same against __ctype_tolower_loc/__ctype_toupper_loc
 * with 4-byte signed entries.
 *
 * The real system header is included on purpose, so the expansions are the real
 * glibc ones. Two things the fixture is built to exercise:
 *
 *  - the table entry is loaded ONCE and masked by every predicate in the
 *    function, so the mask does not apply to a Load by the time patterns run;
 *  - glibc's _ISbit puts eight masks in the high byte of the entry and four in
 *    the low one, and gcc tests a high one with `test $0x20,%ah` -- an 8-bit
 *    slice at byte offset 1, not a truncation.
 *
 * Build: make -f Makefile.known_patterns
 *        gcc -O2 -o known_patterns_ctype known_patterns_ctype.c
 */
#include <ctype.h>
#include <errno.h>
#include <stddef.h>

/* Results are stored through a pointer rather than returned: angr's return-value
 * liveness gives up on some of these shapes and would DCE the whole body, which
 * would make the fixture test nothing. */

/* one predicate on its own, in the branch form that is how ctype is used */
__attribute__((noinline)) void one_space(int c, int *out) { if (isspace(c)) *out = 1; }

/* a high-byte mask and a low-byte mask on the same entry. gcc folds the two
 * into a single `& 0x2008` test, which no per-predicate pattern can name -- it
 * is here to document that, not to be matched. */
__attribute__((noinline)) void space_or_alnum(int c, int *out) { if (isspace(c) || isalnum(c)) *out = 1; }

/* the whole family over one buffer: the table pointer is hoisted out of the
 * loop and the entry is loaded once per character */
__attribute__((noinline)) void classify(const char *p, int *out)
{
    int n = 0;
    for (; *p; p++) {
        unsigned char c = (unsigned char)*p;
        if (isspace(c)) n += 1;
        else if (isdigit(c)) n += 2;
        else if (isalpha(c)) n += 3;
        else if (isupper(c)) n += 4;
        else if (isxdigit(c)) n += 5;
        else if (isalnum(c)) n += 6;
        else if (ispunct(c)) n += 7;
        else if (iscntrl(c)) n += 8;
    }
    *out = n;
}

__attribute__((noinline)) void lower_all(char *p, size_t n)
{
    for (size_t i = 0; i < n; i++) p[i] = (char)tolower((unsigned char)p[i]);
}

__attribute__((noinline)) void upper_all(char *p, size_t n)
{
    for (size_t i = 0; i < n; i++) p[i] = (char)toupper((unsigned char)p[i]);
}

/*
 * errno is a glibc macro too, but it is not a KnownPattern: it expands to
 * *(__errno_location()), which puts it in a Store's *address* position, where
 * no value-producing call can go. It is a codegen rendering rule instead, and
 * has to follow the pointer through the register the compiler kept it in --
 * the macro expands per use, but the call happens once.
 */
__attribute__((noinline)) int errno_rw(int fd)
{
    if (fd < 0) {
        errno = EINVAL;
        return -1;
    }
    return errno;
}

int main(void) { int n = 0; classify("a1 b2", &n); return n + errno_rw(1); }
