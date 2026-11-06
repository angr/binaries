/*
 * A loop that reads the previous iteration's value of a vector before overwriting it.
 *
 * Build from the repository root with GCC 15.3.0 (x86_64-linux-gnu):
 * gcc -O2 -fno-inline -fno-pie -no-pie -fno-stack-protector -nostdlib \
 *     -Wl,--build-id=none,-e,sliding_diff \
 *     -o tests/x86_64/decompiler/loop_carried_vector_fold \
 *     tests_src/decompiler/loop_carried_vector_fold.c
 *
 * This is an analysis fixture, not a native executable to run.
 *
 * gcc keeps `prev` in xmm2 and builds `both` in xmm1 from a copy of it (movaps, movlhps). It computes the next `prev`
 * in xmm4 before the store that uses `both` and copies it to xmm2 after the store. Once that copy is propagated, the
 * next `prev` is defined before the store, so a decompiler that folds `both` into the store reads the new `prev`
 * instead of the old one.
 */
#include <emmintrin.h>
#include <xmmintrin.h>

void sliding_diff(__m128i *dst, const unsigned char *src, long n)
{
    const __m128i zero = _mm_setzero_si128();
    __m128i prev = _mm_setzero_si128();
    for (; n >= 8; n -= 8, src += 8, dst++) {
        __m128i cur = _mm_unpacklo_epi8(_mm_loadl_epi64((const __m128i *)src), zero);
        __m128i both = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(prev), _mm_castsi128_ps(cur)));
        prev = _mm_srli_si128(cur, 8);
        *dst = _mm_sub_epi16(both, cur);
    }
}
