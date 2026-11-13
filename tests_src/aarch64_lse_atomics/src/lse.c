/* ARMv8.1-A LSE atomics, emitted by the compiler from C11/GCC atomic builtins.
 * Built with -march=armv8.1-a -mno-outline-atomics so the atomics are inlined
 * rather than routed through libgcc's outline-atomics helpers. */

#include "fixture.h"

#define FN __attribute__((noinline, used))

/* ---- LDADD family: __atomic_fetch_add, result used ---- */
FN unsigned int  lse_ldadd_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_add(p, v, __ATOMIC_RELAXED); }
FN unsigned int  lse_ldadda_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_add(p, v, __ATOMIC_ACQUIRE); }
FN unsigned int  lse_ldaddl_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_add(p, v, __ATOMIC_RELEASE); }
FN unsigned int  lse_ldaddal_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_add(p, v, __ATOMIC_ACQ_REL); }
FN unsigned long lse_ldadd_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_add(p, v, __ATOMIC_RELAXED); }
FN unsigned long lse_ldadda_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_add(p, v, __ATOMIC_ACQUIRE); }
FN unsigned long lse_ldaddl_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_add(p, v, __ATOMIC_RELEASE); }
FN unsigned long lse_ldaddal_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_add(p, v, __ATOMIC_ACQ_REL); }
FN unsigned char lse_ldaddalb(unsigned char *p, unsigned char v)
{ return __atomic_fetch_add(p, v, __ATOMIC_ACQ_REL); }
FN unsigned short lse_ldaddalh(unsigned short *p, unsigned short v)
{ return __atomic_fetch_add(p, v, __ATOMIC_ACQ_REL); }

/* ---- LDCLR family: __atomic_fetch_and (compiler inverts the operand) ---- */
FN unsigned int  lse_ldclr_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_and(p, v, __ATOMIC_RELAXED); }
FN unsigned int  lse_ldclral_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_and(p, v, __ATOMIC_ACQ_REL); }
FN unsigned long lse_ldclr_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_and(p, v, __ATOMIC_RELAXED); }
FN unsigned long lse_ldclral_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_and(p, v, __ATOMIC_ACQ_REL); }

/* ---- LDEOR family: __atomic_fetch_xor ---- */
FN unsigned int  lse_ldeor_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_xor(p, v, __ATOMIC_RELAXED); }
FN unsigned int  lse_ldeoral_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_xor(p, v, __ATOMIC_ACQ_REL); }
FN unsigned long lse_ldeor_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_xor(p, v, __ATOMIC_RELAXED); }
FN unsigned long lse_ldeoral_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_xor(p, v, __ATOMIC_ACQ_REL); }

/* ---- LDSET family: __atomic_fetch_or ---- */
FN unsigned int  lse_ldset_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_or(p, v, __ATOMIC_RELAXED); }
FN unsigned int  lse_ldseta_w(unsigned int *p, unsigned int v)
{ return __atomic_fetch_or(p, v, __ATOMIC_ACQUIRE); }
FN unsigned long lse_ldsetl_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_or(p, v, __ATOMIC_RELEASE); }
FN unsigned long lse_ldsetal_x(unsigned long *p, unsigned long v)
{ return __atomic_fetch_or(p, v, __ATOMIC_ACQ_REL); }

/* ---- SWP family: __atomic_exchange_n ---- */
FN unsigned int  lse_swp_w(unsigned int *p, unsigned int v)
{ return __atomic_exchange_n(p, v, __ATOMIC_RELAXED); }
FN unsigned int  lse_swpa_w(unsigned int *p, unsigned int v)
{ return __atomic_exchange_n(p, v, __ATOMIC_ACQUIRE); }
FN unsigned int  lse_swpl_w(unsigned int *p, unsigned int v)
{ return __atomic_exchange_n(p, v, __ATOMIC_RELEASE); }
FN unsigned int  lse_swpal_w(unsigned int *p, unsigned int v)
{ return __atomic_exchange_n(p, v, __ATOMIC_ACQ_REL); }
FN unsigned long lse_swp_x(unsigned long *p, unsigned long v)
{ return __atomic_exchange_n(p, v, __ATOMIC_RELAXED); }
FN unsigned long lse_swpa_x(unsigned long *p, unsigned long v)
{ return __atomic_exchange_n(p, v, __ATOMIC_ACQUIRE); }
FN unsigned long lse_swpl_x(unsigned long *p, unsigned long v)
{ return __atomic_exchange_n(p, v, __ATOMIC_RELEASE); }
FN unsigned long lse_swpal_x(unsigned long *p, unsigned long v)
{ return __atomic_exchange_n(p, v, __ATOMIC_ACQ_REL); }
FN unsigned char lse_swpb(unsigned char *p, unsigned char v)
{ return __atomic_exchange_n(p, v, __ATOMIC_RELAXED); }
FN unsigned short lse_swph(unsigned short *p, unsigned short v)
{ return __atomic_exchange_n(p, v, __ATOMIC_RELAXED); }

/* ---- CAS family: __atomic_compare_exchange_n ---- */
FN int lse_cas_w(unsigned int *p, unsigned int *e, unsigned int d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); }
FN int lse_casa_w(unsigned int *p, unsigned int *e, unsigned int d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); }
FN int lse_casl_w(unsigned int *p, unsigned int *e, unsigned int d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_RELEASE, __ATOMIC_RELAXED); }
FN int lse_casal_w(unsigned int *p, unsigned int *e, unsigned int d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE); }
FN int lse_cas_x(unsigned long *p, unsigned long *e, unsigned long d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_RELAXED, __ATOMIC_RELAXED); }
FN int lse_casa_x(unsigned long *p, unsigned long *e, unsigned long d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); }
FN int lse_casl_x(unsigned long *p, unsigned long *e, unsigned long d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_RELEASE, __ATOMIC_RELAXED); }
FN int lse_casal_x(unsigned long *p, unsigned long *e, unsigned long d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE); }
FN int lse_casalb(unsigned char *p, unsigned char *e, unsigned char d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE); }
FN int lse_casalh(unsigned short *p, unsigned short *e, unsigned short d)
{ return __atomic_compare_exchange_n(p, e, d, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE); }

/* NOTE: a 128-bit __atomic_compare_exchange_n is NOT here: GCC 15.3 lowers it
 * to a call to libatomic's __atomic_compare_exchange_16 rather than to CASP,
 * so the CASP forms live in lse_asm.c as inline asm. */
