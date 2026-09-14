__attribute__((noinline)) long absval(long x)
{
    if (x < 0)
        return -x;
    return x + 1;
}
int main(int argc, char **argv) { return (int)absval((long)argc - 5); }
