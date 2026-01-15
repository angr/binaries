/* result be put into tests/<arch>/test_return_type_<arch>.elf
 * # RISC-V 64
 * riscv64-linux-gnu-gcc -O2 -no-pie test_return_type.c -o test_return_type_riscv64.elf
 * # x86-64
 * gcc -O2 -no-pie test_return_type.c -o test_return_type_x86_64.elf
 * # ARM64
 * aarch64-linux-gnu-gcc -O2 -no-pie test_return_type.c -o test_return_type_aarch64.elf
 */
#include <stdint.h>

int ret_add_int(int a, int b) { return a + b; }
int ret_sub_int(int a, int b) { return a - b; }
int ret_mul_int(int a, int b) { return a * b; }
int ret_or_int(int a, int b)  { return a | b; }
int ret_xor_int(int a, int b) { return a ^ b; }
int ret_cast_int(long long a) { return (int)a; }

long long ret_add_long(long long a, long long b) { return a + b; }
void* ret_ptr(void* p, int offset) { return (char*)p + offset; }
long long ret_or_long(long long a, long long b) { return a | b; }

int main() {
    return ret_add_int(1, 2) + ret_add_long(1, 2);
}
