// #include <stdio.h>
// #include <stdint.h>
// riscv64-linux-gnu-gcc -shared -fPIC -g -o sim_args_flatten_riscv64.so sim_args_flatten_riscv64.c

struct Flatten1 {
    float f;
    int i;
};

struct Flatten2 {
    int x;
    int y;
};

struct Flatten3 {
    double a;
    float c;
    int d;
};

void complex_func(struct Flatten1 a, struct Flatten2 b, struct Flatten3 c) {
}

/*
int main() {
    struct Flatten1 a = {1.0f, 2};
    struct Flatten2 b = {10, 20};
    struct Flatten3 c = {101.3, 102.3, 60};
    complex_func(a, b, c);
    return 0;
}
*/
