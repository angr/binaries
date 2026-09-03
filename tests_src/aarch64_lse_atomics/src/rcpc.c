/* ARMv8.3-A RCpc: an acquire load compiles to LDAPR / LDAPRB / LDAPRH.
 * Built with -march=armv8.3-a -mno-outline-atomics. */

#include "fixture.h"

#define FN __attribute__((noinline, used))

FN unsigned int   rcpc_ldapr_w(unsigned int *p)
{ return __atomic_load_n(p, __ATOMIC_ACQUIRE); }
FN unsigned long  rcpc_ldapr_x(unsigned long *p)
{ return __atomic_load_n(p, __ATOMIC_ACQUIRE); }
FN unsigned char  rcpc_ldaprb(unsigned char *p)
{ return __atomic_load_n(p, __ATOMIC_ACQUIRE); }
FN unsigned short rcpc_ldaprh(unsigned short *p)
{ return __atomic_load_n(p, __ATOMIC_ACQUIRE); }

/* seq_cst load stays LDAR; kept so the fixture has both side by side */
FN unsigned int rcpc_ldar_w(unsigned int *p)
{ return __atomic_load_n(p, __ATOMIC_SEQ_CST); }
