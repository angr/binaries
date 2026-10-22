    .text
    .globl brcth_displacements
    .type brcth_displacements,@function
brcth_displacements:
    brcth %r1, .+0x20000
    brcth %r1, .+6
    brcth %r1, .-2
    brcth %r1, .-0x20000
    .size brcth_displacements, .-brcth_displacements
