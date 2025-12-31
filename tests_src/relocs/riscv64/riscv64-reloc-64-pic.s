# https://github.com/llvm/llvm-project/blob/523f999acf6faa94d497ca1e81cb676d0a6063d5/lld/test/ELF/riscv64-reloc-64-pic.s

# REQUIRES: riscv
# RUN: llvm-mc -filetype=obj -triple=riscv64 %s -o %t.o
# RUN: ld.lld -shared %t.o -o %t.so
# RUN: llvm-nm %t.so | FileCheck --check-prefix=NM %s
# RUN: llvm-readobj -r %t.so | FileCheck --check-prefix=RELOC %s

## R_RISCV_64 is an absolute relocation type.
## In PIC mode, it creates a relative relocation if the symbol is non-preemptable.

# NM: 0000000000003350 d b

# RELOC:      .rela.dyn {
# RELOC-NEXT:   0x3350 R_RISCV_RELATIVE - 0x3350
# RELOC-NEXT:   0x3348 R_RISCV_64 a 0
# RELOC-NEXT: }

.globl a, b
.hidden b

.data
.quad a
b:
.quad b