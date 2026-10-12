/* An else-if chain of floating-point equality tests against +inf and -inf, with a distinct body per test.
   clang --target=aarch64-linux-gnu -O1 -ffreestanding -fno-asynchronous-unwind-tables -c -o fcmp_inf_aarch64.o fcmp_inf.c */

extern void on_pos_inf(void);
extern void on_neg_inf(void);
extern void on_finite(double x);
extern void on_pos_inf_f(void);
extern void on_neg_inf_f(void);
extern void on_finite_f(float x);

void dispatch_inf(double x)
{
    if (x == __builtin_inf())
        on_pos_inf();
    else if (x == -__builtin_inf())
        on_neg_inf();
    else
        on_finite(x);
}

void dispatch_inf_f(float x)
{
    if (x == __builtin_inff())
        on_pos_inf_f();
    else if (x == -__builtin_inff())
        on_neg_inf_f();
    else
        on_finite_f(x);
}
