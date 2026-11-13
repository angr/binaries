/*
 * Internal functions that MSVC's whole-program optimization (/GL with /LTCG)
 * gives custom conventions: a double argument at position i arrives in
 * xmm<i>, and a double result comes back in xmm0.
 *
 * Built with MSVC 14.44.35207 (Visual Studio Build Tools, Hostx64/x86), linking
 * no Microsoft runtime:
 *   cl /nologo /c /TC /X /GS- /Zl /Brepro /O2 /GL msvc_ltcg_xmm_args.c
 *   link /nologo /nodefaultlib /Brepro /LTCG /incremental:no /opt:ref /opt:icf
 *        /machine:X86 /subsystem:console /entry:entry msvc_ltcg_xmm_args.obj
 */

int _fltused;

typedef struct {
    double sum;
    double sumsq;
    int count;
} stats_t;

volatile double g_in[4] = {1.5, 2.25, 3.0, 4.75};
volatile int g_k = 3;
double g_out[8];
stats_t g_stats;
volatile int g_floor;

static __declspec(noinline) void stats_add(stats_t *s, double v)
{
    s->sum += v;
    s->sumsq += v * v;
    s->count++;
}

static __declspec(noinline) double lerp(double a, double b, double t)
{
    return a + (b - a) * t;
}

static __declspec(noinline) void store_scaled(int index, int stride, double scale, double bias)
{
    g_out[(index * stride) & 7] = g_in[index & 3] * scale + bias;
}

static __declspec(noinline) double mean(const stats_t *s)
{
    return s->count ? s->sum / s->count : 0.0;
}

static __declspec(noinline) double spread(const stats_t *s)
{
    double m = mean(s);
    return s->sumsq / s->count - m * m;
}

static __declspec(noinline) int floor_clamped(float f)
{
    int i;

    if (f < -2147483648.0f)
        return (int)0x80000000;
    if (f >= 2147483648.0f)
        return 0x7fffffff;
    i = (int)f;
    return (float)i > f ? i - 1 : i;
}

/* Called through a pointer, so it keeps the standard __cdecl convention. */
static __declspec(noinline) void __cdecl add_pair(int a, int b)
{
    stats_add(&g_stats, (double)(a * b));
    g_out[2] = g_stats.sum;
}

void (__cdecl *volatile g_add_pair)(int, int) = add_pair;

int __cdecl entry(void)
{
    stats_t s = {0.0, 0.0, 0};
    int i;

    for (i = 0; i < g_k; i++)
        stats_add(&s, g_in[i & 3]);
    store_scaled(g_k, 2, lerp(g_in[0], g_in[1], g_in[2]), mean(&s));
    g_out[0] = mean(&s) * 2.0;
    g_out[1] = spread(&s);
    g_add_pair(g_k, g_k + 1);
    g_floor = floor_clamped((float)g_in[3]);
    return (int)g_out[0];
}
