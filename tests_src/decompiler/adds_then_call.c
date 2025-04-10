// gcc -c -o adds_then_call.o -O0 adds_then_call.c
void g(int x);

void f(void)
{
    int x;

    x = 10;
    if (x <= 1000) {
        x += 1;
        x += 2;
        x += 3;
        x += 4;
        x += 5;
        x += 6;
        x += 7;
        x += 8;
        x += 9;
    }
    g(x);
}
