/* gcc -m32 -mfpmath=387 -march=i386 -O1: fcomp; fnstsw ax; test ah, 0x45 / and ah, 0x45; cmp ah, 0x40 */
int lt_f64(double a, double b) { return a < b; }
int le_f64(double a, double b) { return a <= b; }
int gt_f64(double a, double b) { return a > b; }
int ge_f64(double a, double b) { return a >= b; }
int eq_f64(double a, double b) { return a == b; }
int ne_f64(double a, double b) { return a != b; }
int br_lt_f64(double a, double b) { if (a < b) return 1; return 2; }
int br_eq_f64(double a, double b) { if (a == b) return 1; return 2; }
