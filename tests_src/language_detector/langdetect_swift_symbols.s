/* Assembly file that defines Swift-like symbols for testing.
 * These global symbols mimic what a real Swift binary would export.
 */

    .section .swift5_typeref,"",@progbits
    .byte 0

    .section .swift5_fieldmd,"",@progbits
    .byte 0

    .section .swift5_proto,"",@progbits
    .byte 0

    .text
    .globl swift_bridgeObjectRelease
    .type swift_bridgeObjectRelease, @function
swift_bridgeObjectRelease:
    ret
    .size swift_bridgeObjectRelease, .-swift_bridgeObjectRelease

    .globl swift_bridgeObjectRetain
    .type swift_bridgeObjectRetain, @function
swift_bridgeObjectRetain:
    ret
    .size swift_bridgeObjectRetain, .-swift_bridgeObjectRetain

    .globl swift_release
    .type swift_release, @function
swift_release:
    ret
    .size swift_release, .-swift_release

    .globl swift_retain
    .type swift_retain, @function
swift_retain:
    ret
    .size swift_retain, .-swift_retain

    .globl swift_allocObject
    .type swift_allocObject, @function
swift_allocObject:
    ret
    .size swift_allocObject, .-swift_allocObject
