/* Test source for FullProgramIndirectJumpResolution: stack struct with a
 * function pointer initialized in one function and called in another.
 * Compile: gcc -O2 -no-pie -g -o fpijr_interproc fpijr_interproc.c
 */

__attribute__((noinline)) int h1(int x) { return x + 10; }
__attribute__((noinline)) int h2(int x) { return x * 3; }

struct ops {
    long id;
    int (*handler)(int);
};

__attribute__((noinline)) void init_ops(struct ops *o, int which)
{
    o->id = 1;
    o->handler = which ? h1 : h2;
}

__attribute__((noinline)) int run_ops(struct ops *o, int x)
{
    /* The +1 prevents gcc from turning the indirect call into a tail
     * jump, so run_ops contains a genuine `call *`. */
    return o->handler(x) + 1;
}

int main(int argc, char **argv)
{
    (void)argv;
    struct ops o;
    init_ops(&o, argc & 1);
    return run_ops(&o, argc);
}
