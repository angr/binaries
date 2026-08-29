/* LSE encodings that GCC will not emit from portable C.
 *
 * GCC has no atomic min/max builtin (LDSMAX/LDSMIN/LDUMAX/LDUMIN have no C11
 * or __atomic_* spelling), and it lowers a 128-bit __atomic_compare_exchange
 * to a libatomic call rather than to CASP.  These are therefore written as
 * inline asm inside ordinary C functions; the file is still compiled and
 * assembled by the cross toolchain like any other translation unit.
 *
 * Built with -march=armv8.1-a. */

#include "fixture.h"

#define FN __attribute__((noinline, used))

#define MINMAX_W(name, insn)                                                  \
    FN unsigned int name(unsigned int *p, unsigned int v)                     \
    {                                                                         \
        unsigned int old;                                                     \
        __asm__ __volatile__(insn " %w2, %w0, [%1]"                           \
                             : "=r"(old) : "r"(p), "r"(v) : "memory");        \
        return old;                                                           \
    }

#define MINMAX_X(name, insn)                                                  \
    FN unsigned long name(unsigned long *p, unsigned long v)                  \
    {                                                                         \
        unsigned long old;                                                    \
        __asm__ __volatile__(insn " %2, %0, [%1]"                             \
                             : "=r"(old) : "r"(p), "r"(v) : "memory");        \
        return old;                                                           \
    }

MINMAX_W(lse_ldsmax_w,   "ldsmax")
MINMAX_W(lse_ldsmaxa_w,  "ldsmaxa")
MINMAX_W(lse_ldsmaxl_w,  "ldsmaxl")
MINMAX_W(lse_ldsmaxal_w, "ldsmaxal")
MINMAX_X(lse_ldsmax_x,   "ldsmax")
MINMAX_X(lse_ldsmaxal_x, "ldsmaxal")

MINMAX_W(lse_ldsmin_w,   "ldsmin")
MINMAX_W(lse_ldsmina_w,  "ldsmina")
MINMAX_W(lse_ldsminl_w,  "ldsminl")
MINMAX_W(lse_ldsminal_w, "ldsminal")
MINMAX_X(lse_ldsmin_x,   "ldsmin")
MINMAX_X(lse_ldsminal_x, "ldsminal")

MINMAX_W(lse_ldumax_w,   "ldumax")
MINMAX_W(lse_ldumaxa_w,  "ldumaxa")
MINMAX_W(lse_ldumaxl_w,  "ldumaxl")
MINMAX_W(lse_ldumaxal_w, "ldumaxal")
MINMAX_X(lse_ldumax_x,   "ldumax")
MINMAX_X(lse_ldumaxal_x, "ldumaxal")

MINMAX_W(lse_ldumin_w,   "ldumin")
MINMAX_W(lse_ldumina_w,  "ldumina")
MINMAX_W(lse_lduminl_w,  "lduminl")
MINMAX_W(lse_lduminal_w, "lduminal")
MINMAX_X(lse_ldumin_x,   "ldumin")
MINMAX_X(lse_lduminal_x, "lduminal")

/* byte and halfword variants of one min/max form */
FN unsigned char lse_ldsmaxb(unsigned char *p, unsigned char v)
{
    unsigned int old;
    __asm__ __volatile__("ldsmaxb %w2, %w0, [%1]"
                         : "=r"(old) : "r"(p), "r"(v) : "memory");
    return (unsigned char)old;
}
FN unsigned short lse_lduminh(unsigned short *p, unsigned short v)
{
    unsigned int old;
    __asm__ __volatile__("lduminh %w2, %w0, [%1]"
                         : "=r"(old) : "r"(p), "r"(v) : "memory");
    return (unsigned short)old;
}

/* ---- CASP: 64-bit pair (X regs) and 32-bit pair (W regs) ---- */
#define CASP_X(name, insn)                                                    \
    FN unsigned long name(unsigned long *p, unsigned long elo,                \
                          unsigned long ehi, unsigned long nlo,               \
                          unsigned long nhi)                                  \
    {                                                                         \
        register unsigned long r0 __asm__("x0") = elo;                        \
        register unsigned long r1 __asm__("x1") = ehi;                        \
        register unsigned long r2 __asm__("x2") = nlo;                        \
        register unsigned long r3 __asm__("x3") = nhi;                        \
        __asm__ __volatile__(insn " %0, %1, %2, %3, [%4]"                     \
                             : "+r"(r0), "+r"(r1)                             \
                             : "r"(r2), "r"(r3), "r"(p) : "memory");          \
        return r0 ^ r1;                                                       \
    }

#define CASP_W(name, insn)                                                    \
    FN unsigned int name(unsigned int *p, unsigned int elo, unsigned int ehi, \
                         unsigned int nlo, unsigned int nhi)                  \
    {                                                                         \
        register unsigned int r0 __asm__("x0") = elo;                         \
        register unsigned int r1 __asm__("x1") = ehi;                         \
        register unsigned int r2 __asm__("x2") = nlo;                         \
        register unsigned int r3 __asm__("x3") = nhi;                         \
        __asm__ __volatile__(insn " %w0, %w1, %w2, %w3, [%4]"                 \
                             : "+r"(r0), "+r"(r1)                             \
                             : "r"(r2), "r"(r3), "r"(p) : "memory");          \
        return r0 ^ r1;                                                       \
    }

CASP_X(lse_casp_x,   "casp")
CASP_X(lse_caspa_x,  "caspa")
CASP_X(lse_caspl_x,  "caspl")
CASP_X(lse_caspal_x, "caspal")
CASP_W(lse_casp_w,   "casp")
CASP_W(lse_caspa_w,  "caspa")
CASP_W(lse_caspl_w,  "caspl")
CASP_W(lse_caspal_w, "caspal")

/* ---- LSE store-form aliases (STADD & co are LDADD with Rt == xzr) ---- */
FN void lse_stadd_w(unsigned int *p, unsigned int v)
{ __atomic_fetch_add(p, v, __ATOMIC_RELAXED); }
FN void lse_staddl_x(unsigned long *p, unsigned long v)
{ __atomic_fetch_add(p, v, __ATOMIC_RELEASE); }
FN void lse_stset_w(unsigned int *p, unsigned int v)
{ __atomic_fetch_or(p, v, __ATOMIC_RELAXED); }
FN void lse_stclr_x(unsigned long *p, unsigned long v)
{ __atomic_fetch_and(p, ~v, __ATOMIC_RELAXED); }
FN void lse_steor_w(unsigned int *p, unsigned int v)
{ __atomic_fetch_xor(p, v, __ATOMIC_RELAXED); }
