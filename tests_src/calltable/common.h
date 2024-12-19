#include <stdlib.h>

int dst_func_0(int x, int y) { return x + y; }
int dst_func_1(int x, int y) { return x - y; }
int dst_func_2(int x, int y) { return x * y; }
int dst_func_3(int x, int y) { return x / y; }

typedef int (*calltable_entry_t)(int x, int y);
calltable_entry_t table[] = {dst_func_0, dst_func_1, dst_func_2, dst_func_3};

int src_func(int i, int x, int y);

int main(int argc, char *argv[]) {
    return src_func(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
}
