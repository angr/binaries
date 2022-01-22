#include <immintrin.h>

__m128i global_var;

int main () {
    volatile __m128i local_var = _mm_loadu_si128(&global_var);
    return 0;
}
