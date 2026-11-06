#if DWARF_VERSION < 5
.file 1 "main.c"
.file 2 "include/header.h"
#define MAIN_FILE 1
#define HEADER_FILE 2
#else
#if ABSOLUTE_DIRS
.file 0 "/source" "main.c"
.file 1 "/include/header.h"
#else
.file 0 "source" "main.c"
.file 1 "include/header.h"
#endif
#define MAIN_FILE 0
#define HEADER_FILE 1
#endif

.text
.globl _start
.type _start, @function
_start:
.cfi_startproc
.loc MAIN_FILE 11 0
nop
.loc HEADER_FILE 23 0
nop
.loc MAIN_FILE 12 0
mov $60, %eax
xor %edi, %edi
syscall
.cfi_endproc
.size _start, . - _start

.section .note.GNU-stack,"",@progbits
