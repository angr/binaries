/*
 * Test binary for angr's KnownPatternFinder: the single-expression glibc/POSIX
 * macros that the preprocessor inlines into every caller and that therefore
 * survive into decompiled output as bare bit arithmetic:
 *
 *   <sys/stat.h>       S_ISREG/S_ISDIR/S_ISCHR/S_ISBLK/S_ISFIFO/S_ISLNK/S_ISSOCK
 *                      -> ((m) & 0170000) == <S_IFxxx>
 *   <sys/wait.h>       WIFEXITED/WIFSIGNALED/WIFSTOPPED/WEXITSTATUS/WTERMSIG/WSTOPSIG
 *                      -> (s & 0x7f) == 0, ((signed char)(((s & 0x7f) + 1) >> 1)) > 0,
 *                         (s & 0xff) == 0x7f, (s & 0xff00) >> 8, s & 0x7f
 *   <sys/sysmacros.h>  major/minor/makedev -> gnu_dev_* extern-inline bodies
 *
 * The real system headers are included on purpose so the expansions are the
 * REAL glibc ones, not a hand-written approximation. Each macro is exercised
 * through its own noinline wrapper whose whole body is the macro, so the
 * matched pattern spans the entire function.
 *
 * Build: make -f Makefile.known_patterns
 *        gcc -O2 -o known_patterns_glibc_macros known_patterns_glibc_macros.c
 */
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/types.h>
#include <sys/wait.h>

/* ---------------- <sys/stat.h> file-type predicates ---------------- */

__attribute__((noinline)) int chk_isreg(mode_t m)
{
    return S_ISREG(m);
}

__attribute__((noinline)) int chk_isdir(mode_t m)
{
    return S_ISDIR(m);
}

__attribute__((noinline)) int chk_ischr(mode_t m)
{
    return S_ISCHR(m);
}

__attribute__((noinline)) int chk_isblk(mode_t m)
{
    return S_ISBLK(m);
}

__attribute__((noinline)) int chk_isfifo(mode_t m)
{
    return S_ISFIFO(m);
}

__attribute__((noinline)) int chk_islnk(mode_t m)
{
    return S_ISLNK(m);
}

__attribute__((noinline)) int chk_issock(mode_t m)
{
    return S_ISSOCK(m);
}

/* ---------------- <sys/wait.h> status decoders ---------------- */

__attribute__((noinline)) int st_ifexited(int s)
{
    return WIFEXITED(s);
}

__attribute__((noinline)) int st_ifsignaled(int s)
{
    return WIFSIGNALED(s);
}

__attribute__((noinline)) int st_ifstopped(int s)
{
    return WIFSTOPPED(s);
}

__attribute__((noinline)) int st_exitstatus(int s)
{
    return WEXITSTATUS(s);
}

__attribute__((noinline)) int st_termsig(int s)
{
    return WTERMSIG(s);
}

__attribute__((noinline)) int st_stopsig(int s)
{
    return WSTOPSIG(s);
}

/* ---------------- <sys/sysmacros.h> device-number packing ---------------- */

__attribute__((noinline)) unsigned int dev_major(dev_t d)
{
    return major(d);
}

__attribute__((noinline)) unsigned int dev_minor(dev_t d)
{
    return minor(d);
}

__attribute__((noinline)) dev_t dev_makedev(unsigned int maj, unsigned int min)
{
    return makedev(maj, min);
}

int main(void)
{
    int acc = 0;
    mode_t m = (mode_t)(unsigned long)&acc;
    int s = acc;
    dev_t d = (dev_t)(unsigned long)&acc;

    acc += chk_isreg(m);
    acc += chk_isdir(m);
    acc += chk_ischr(m);
    acc += chk_isblk(m);
    acc += chk_isfifo(m);
    acc += chk_islnk(m);
    acc += chk_issock(m);

    acc += st_ifexited(s);
    acc += st_ifsignaled(s);
    acc += st_ifstopped(s);
    acc += st_exitstatus(s);
    acc += st_termsig(s);
    acc += st_stopsig(s);

    acc += (int)dev_major(d);
    acc += (int)dev_minor(d);
    acc += (int)dev_makedev((unsigned int)acc, (unsigned int)acc);
    return acc;
}
