// #include <stdio.h>
// riscv64-linux-gnu-gcc -shared -fPIC -g -o sim_args_riscv64.so sim_args_riscv64.c

struct Flatten1 {
    float f;
    int i;
};

struct Flatten2 {
    double x;
    double y;
    double z;
};

void complex_func(int a, struct Flatten1 s1, double b, struct Flatten2 s2, 
                  long r4, long r5, long r6, long r7, 
                  long r8, double fp1, int r9, int r10, float fp2) {
}

/*
int main() {
    struct Flatten1 s1 = {1.0f, 2};
    struct Flatten2 s2 = {10.0, 20.0, 30.0};
    complex_func(100, s1, 3.0, s2, 4, 5, 6, 7, 8, 9.0, 10, 11, 12.0f);
    return 0;
}
*/
