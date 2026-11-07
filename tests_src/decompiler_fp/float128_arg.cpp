// __float128 passed by value: the SysV x86-64 ABI classifies it SSE, SSEUP and passes it in one xmm register. The
// mangled name of accumulate carries the parameter type (`g`), so a decompiler learns the prototype from the symbol.
// scale has a C name, so recovering its calling convention analyzes its call site in accumulate.
__float128 seed;
__float128 total;
double last;

extern "C" __attribute__((noinline)) double scale(double x)
{
    return x * 3.0;
}

__attribute__((noinline)) void accumulate(__float128 q, double a)
{
    total = q;
    last = scale(a);
}

void drive(int n)
{
    for (int i = 0; i < n; i++)
        accumulate(seed, 0.5 * i);
}
