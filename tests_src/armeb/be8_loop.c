volatile int counter;

static int add(int a, int b) { return a + b; }

int compute(int n) {
    int total = 0;
    for (int i = 0; i < n; i++) total = add(total, i);
    return total;
}

void _start(void) {
    counter = compute(10);
    for (;;) {}
}
