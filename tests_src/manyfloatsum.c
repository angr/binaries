#include <stdio.h>
// WELCOME TO HELL

float sum_floats(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p, float q, float r, float s) {
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r + s;
}

float sum_combo(int a, float b, int c, float d, int e, float f, int g, float h, int i, float j, int k, float l, int m, float n, int o, float p, int q, float r, int s) {
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r + s;
}

float sum_segregated(int a, int b, int c, int d, int e, int f, int g, int h, int i, float j, float k, float l, float m, float n, float o, float p, float q, float r, int s) {
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r + s;
}

double sum_doubles(double a, double b, double c, double d, double e, double f, double g, double h, double i, double j, double k, double l, double m, double n, double o, double p, double q, double r, double s) {
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r + s;
}

double sum_combo_doubles(int a, double b, int c, double d, int e, double f, int g, double h, int i, double j, int k, double l, int m, double n, int o, double p, int q, double r, int s) {
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r + s;
}

double sum_segregated_doubles(int a, int b, int c, int d, int e, int f, int g, int h, int i, double j, double k, double l, double m, double n, double o, double p, double q, double r, int s) {
    return a + b + c + d + e + f + g + h + i + j + k + l + m + n + o + p + q + r + s;
}

double sum_hell(float a1, double a2, long a3, int a4, float a5, long a6, double a7, int a8, double a9, long a10, long a11, long a12, long a13, int a14, float a15, double a16, double a17, int a18, float a19, float a20, float a21, int a22, float a23, long a24, int a25, double a26, int a27, long a28, double a29, float a30, long a31, int a32, long a33, int a34, int a35, long a36, long a37, long a38, long a39, float a40, float a41, double a42, int a43, long a44, long a45, long a46, int a47, long a48, int a49, long a50, int a51, long a52, float a53, int a54, double a55, double a56, int a57, int a58, float a59, int a60, long a61, int a62, float a63, long a64, double a65, int a66, long a67, double a68, float a69, long a70, long a71, double a72, double a73, int a74, long a75, long a76, float a77, float a78, float a79, int a80, double a81, float a82, double a83, float a84, float a85, int a86, float a87, int a88, long a89, long a90, float a91, long a92, long a93, double a94, int a95, float a96, float a97, long a98, int a99, int a100) {
    return a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9 + a10 + a11 + a12 + a13 + a14 + a15 + a16 + a17 + a18 + a19 + a20 + a21 + a22 + a23 + a24 + a25 + a26 + a27 + a28 + a29 + a30 + a31 + a32 + a33 + a34 + a35 + a36 + a37 + a38 + a39 + a40 + a41 + a42 + a43 + a44 + a45 + a46 + a47 + a48 + a49 + a50 + a51 + a52 + a53 + a54 + a55 + a56 + a57 + a58 + a59 + a60 + a61 + a62 + a63 + a64 + a65 + a66 + a67 + a68 + a69 + a70 + a71 + a72 + a73 + a74 + a75 + a76 + a77 + a78 + a79 + a80 + a81 + a82 + a83 + a84 + a85 + a86 + a87 + a88 + a89 + a90 + a91 + a92 + a93 + a94 + a95 + a96 + a97 + a98 + a99 + a100;
}

int main() {
    printf("%lf\n", sum_floats(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) + sum_combo(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) + sum_segregated(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) +
           sum_doubles(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) + sum_combo_doubles(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) + sum_segregated_doubles(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19) +
           sum_hell(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100));
}
