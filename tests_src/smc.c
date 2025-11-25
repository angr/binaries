// gcc -O1 -o ../tests/x86_64/smc smc.c
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define ALIGN_UP(v, align) (((v)+(align)-1)&~((align)-1))

// we can't symbolically execute through memalign in native glibc yet
static inline
void *memalign_(size_t page_size, size_t buf_size)
{
    char *buf = malloc(buf_size+page_size);
    return (char*)ALIGN_UP((ptrdiff_t)buf, page_size);
}

// mov eax, 0xdeadbeef; ret
const char *code_bytes = "\xb8\xef\xbe\xad\xde\xc3";
const size_t code_len = 6;

int main(int argc, char **argv) {
    size_t page_size = sysconf(_SC_PAGE_SIZE);
    assert(page_size != -1);

    size_t buf_size = ALIGN_UP(code_len, page_size);
    void *buf = memalign_(page_size, buf_size);
    assert(buf);
    memcpy(buf, code_bytes, code_len); 

    int status = mprotect(buf, buf_size, PROT_EXEC | PROT_READ);
    if (status) {
        perror("mprotect");
	return 1;
    }

    int v = ( (int(*)(void)) buf )();
    printf("v = 0x%x\n", v);

    return 0;
}
