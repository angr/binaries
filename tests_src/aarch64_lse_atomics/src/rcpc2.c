/* ARMv8.4-A RCpc2 (LDAPUR / STLUR): acquire load and release store using an
 * unscaled immediate offset.  Built with -march=armv8.4-a. */

#include "fixture.h"

#define FN __attribute__((noinline, used))

FN unsigned long rcpc2_ldapur_x(unsigned char *base)
{ return __atomic_load_n((unsigned long *)(base + 4), __ATOMIC_ACQUIRE); }
FN unsigned int rcpc2_ldapur_w(unsigned char *base)
{ return __atomic_load_n((unsigned int *)(base + 2), __ATOMIC_ACQUIRE); }
FN void rcpc2_stlur_x(unsigned char *base, unsigned long v)
{ __atomic_store_n((unsigned long *)(base + 4), v, __ATOMIC_RELEASE); }
FN void rcpc2_stlur_w(unsigned char *base, unsigned int v)
{ __atomic_store_n((unsigned int *)(base + 2), v, __ATOMIC_RELEASE); }
FN unsigned char rcpc2_ldapurb(unsigned char *base)
{ return __atomic_load_n((unsigned char *)(base - 3), __ATOMIC_ACQUIRE); }
FN unsigned short rcpc2_ldapurh(unsigned char *base)
{ return __atomic_load_n((unsigned short *)(base - 4), __ATOMIC_ACQUIRE); }

/* GCC emits STLUR from a release store at an unscaled offset, but folds the
 * acquire load into `add` + LDAPR rather than LDAPUR, so the LDAPUR forms are
 * written as inline asm.  Still compiled and assembled by the cross toolchain. */
FN unsigned long rcpc2_ldapur_x_asm(unsigned char *base)
{
    unsigned long v;
    __asm__ __volatile__("ldapur %0, [%1, #4]" : "=r"(v) : "r"(base) : "memory");
    return v;
}
FN unsigned int rcpc2_ldapur_w_asm(unsigned char *base)
{
    unsigned int v;
    __asm__ __volatile__("ldapur %w0, [%1, #-8]" : "=r"(v) : "r"(base) : "memory");
    return v;
}
FN unsigned char rcpc2_ldapurb_asm(unsigned char *base)
{
    unsigned int v;
    __asm__ __volatile__("ldapurb %w0, [%1, #3]" : "=r"(v) : "r"(base) : "memory");
    return (unsigned char)v;
}
FN unsigned short rcpc2_ldapurh_asm(unsigned char *base)
{
    unsigned int v;
    __asm__ __volatile__("ldapurh %w0, [%1, #6]" : "=r"(v) : "r"(base) : "memory");
    return (unsigned short)v;
}
FN void rcpc2_stlurb_asm(unsigned char *base, unsigned char v)
{
    __asm__ __volatile__("stlurb %w0, [%1, #1]" :: "r"((unsigned int)v), "r"(base) : "memory");
}
FN void rcpc2_stlurh_asm(unsigned char *base, unsigned short v)
{
    __asm__ __volatile__("stlurh %w0, [%1, #2]" :: "r"((unsigned int)v), "r"(base) : "memory");
}
