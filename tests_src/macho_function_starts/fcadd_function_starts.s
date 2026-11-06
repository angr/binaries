// Built by build.sh into tests/aarch64/fcadd_function_starts.macho.
// _start calls _cmul_rot, whose loop body holds FCADD (Armv8.3 FEAT_FCMA, complex add). A lifter that does not
// implement FCADD stops _cmul_rot's first block there, before the loop's branch and the function's ret.
// _cmul_rot is a local symbol and linking with -x drops it, so LC_FUNCTION_STARTS is the only record in the
// file that a function begins at _cmul_rot.
        .text
        .p2align 2
        .globl _start
_start:
        sub     sp, sp, #48
        stp     x29, x30, [sp, #32]
        add     x29, sp, #32
        str     x0, [sp, #8]
        mov     w1, #8
        bl      _cmul_rot
        ldr     x0, [sp, #8]
        mov     w1, #8
        bl      _square_first
        ldp     x29, x30, [sp, #32]
        add     sp, sp, #48
        ret

// x0: pairs of 4-float vectors, w1: number of pairs (at least one)
        .p2align 2
_cmul_rot:
        mov     x2, x0
1:
        ldr     q0, [x2]
        ldr     q1, [x2, #16]
        fcadd   v2.4s, v0.4s, v1.4s, #270
        str     q2, [x2], #32
        subs    w1, w1, #1
        b.ne    1b
        ret

        .p2align 2
_square_first:
        ldr     s0, [x0]
        fmul    s0, s0, s0
        str     s0, [x0]
        ret
