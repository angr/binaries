#include <stdarg.h>
#include <stdint.h>
#include <xmmintrin.h>

typedef struct {long a,b;} i2;
typedef struct {long a,b,c,d;} i4;
typedef struct {long a,b,c,d,e,f,g,h;} i8;

typedef struct {double a; long b;} f1i1;
typedef struct {char a[9]; char b;} c9c1;
typedef struct {float a; int b;} f1i1_32;
//typedef struct {long a; __m128 b; } i1x1;

i2 test_small_struct_return() {
	return (i2) { 1,2 };
}

i4 test_medium_struct_return() {
	return (i4) { 1,2,3,4 };
}

i8 test_large_struct_return() {
	return (i8) { 1,2,3,4,5,6,7,8 };
}

long test_multiple_struct_args(i2 a, i2 b) {
	return a.a + a.b + b.a + b.b;
}

long test_medium_struct_arg(i4 a) {
	return a.a + a.b + a.c + a.d;
}

long test_large_struct_arg(i8 a) {
	return a.a + a.b + a.c + a.d + a.e + a.f + a.g + a.h;
}

long test_offset_struct_arg(long a, i2 b) {
	return a + b.a + b.b;
}

long test_register_spilling(i2 a, i2 b, i2 c, i2 d) {
	return a.a + a.b + b.a + b.b + c.a + c.b + d.a + d.b;
}

f1i1 test_mixed_struct(f1i1 a) {
	return a;
}

long test_large_then_small(i8 a, i2 b) {
	return a.a + a.b + a.c + a.d + a.e + a.f + a.g + a.h + b.a * 2 + b.b * 2;
}

c9c1 test_misaligned_chars(c9c1 a) {
	return a;
}

f1i1_32 test_mixed_struct_packing(f1i1_32 a) {
	return a;
}

//long test_variadic(int count, ...) {
//	va_list va;
//	va_start(va, count);
//
//	long result = 0;
//	while (count --> 0) {
//		if (count % 2 == 0) {
//			result += va_arg(va, long);
//		} else {
//			i2 item = va_arg(va, i2);
//			result += item.a + item.b;
//		}
//	}
//	va_end(va);
//	return result;
//}
//
//long uses_variadic(i2 a) {
//	return test_variadic(1, a, 4);
//}

//i1x1 test_vacant_qword(i1x1 a) {
//	return (i1x1) { .a = a.a, .b = a.a };
//}
//
//void uses_vacant_qword() {
//	test_vacant_qword((i1x1) { .a = 1, .b = 2 });
//}
