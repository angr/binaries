; movmskpd / movmskps gather the sign bit of every lane; VEX lifts them lane by lane.
;
; int mask_pd(const double *p)   -- movupd + movmskpd: a genuine 2-lane vector  -> _mm_movemask_pd(v)
; int mask_ps(const float *p)    -- movups + movmskps: a genuine 4-lane vector  -> _mm_movemask_ps(v)
; int sign_d(double x)           -- movmskpd + and 1: lane 0 of a scalar        -> signbit(x)
; int sign_f(float x)            -- movmskps + and 1: lane 0 of a scalar        -> signbit(x)

bits 64

section .text
global mask_pd
global mask_ps
global sign_d
global sign_f

mask_pd:
    movupd   xmm0, [rdi]
    movmskpd eax, xmm0
    ret

mask_ps:
    movups   xmm0, [rdi]
    movmskps eax, xmm0
    ret

sign_d:
    movmskpd eax, xmm0
    and      eax, 1
    ret

sign_f:
    movmskps eax, xmm0
    and      eax, 1
    ret
