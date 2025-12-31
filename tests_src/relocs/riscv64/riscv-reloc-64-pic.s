# https://github.com/llvm/llvm-project/blob/523f999acf6faa94d497ca1e81cb676d0a6063d5/lld/test/ELF/riscv-reloc-64-pic.s

# REQUIRES: riscv
# RUN: llvm-mc -filetype=obj -triple=riscv64 %s -o %t.o
# RUN: not ld.lld -shared %t.o -o %t.so 2>&1 | FileCheck %s

# CHECK: error: relocation R_RISCV_32 cannot be used against symbol a

.globl a

.data
.long a