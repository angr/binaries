/*
 * Real-binary reproducer for angr RegionIdentifier's repeated has_path work.
 *
 * From https://github.com/angr/angr/issues/6661, contributed by plowsec
 * (https://github.com/plowsec). See pathological_loop.py for the driver and
 * the exact build command of tests/aarch64/decompiler/pathological_loop.
 *
 * Build example (native AArch64 GNU toolchain):
 *
 *   gcc -nostdlib -no-pie -Wl,-e,region_slice_target \
 *       -Wl,--build-id=none -DSIDE_DIAMONDS=100 -DSPINES=450 \
 *       -DLATCHES=450 \
 *       -o pathological_loop.elf pathological_loop.S
 *
 * The defaults keep the binary quick to inspect.  The Python driver selects a
 * larger, seconds-scale size unless told otherwise.
 */

#ifndef SIDE_DIAMONDS
# define SIDE_DIAMONDS 100
#endif

#ifndef LATCHES
# define LATCHES 450
#endif

#ifndef SPINES
# define SPINES 450
#endif

    .text
    .p2align 2
    .global region_slice_target
    .type region_slice_target, %function
region_slice_target:
    mov     w9, w0
    mov     w10, w1

/*
 * DFS enters this side region before .Linner_head.  Each diamond has two
 * statically visible paths, so Clinic/RegionIdentifier cannot merge the whole
 * region into one straight-line block.
 */
.Lside_start:
    .rept SIDE_DIAMONDS
    tbz     w9, #1, 1f
    nop
1:
    .endr
    b       .Linner_head

/*
 * Every block on the live spine has a second edge back into the already-active
 * side region.  Once RegionIdentifier removes incoming edges to
 * .Linner_head, none of those side edges can reach any latch.  Stock angr
 * therefore repeats the same negative side-region traversal SPINES*LATCHES
 * times.  The fallthrough path enters a chain of independent latches, so DFS
 * sees .Linner_head as an inner loop header and every conditional edge below
 * as one of its latches.
 */
.Linner_head:
    .rept SPINES
    tbnz    w9, #0, .Lside_start
    .endr
    b       .Llatch_start

.Llatch_start:
    .rept LATCHES
    cbz     w10, .Linner_head
    .endr

    add     w0, w9, w10
    ret
    .size region_slice_target, .-region_slice_target

    .section .note.GNU-stack,"",%progbits
