/* gcc -O2 -fno-inline -fstack-protector-strong -c -o alloca_slot_amd64.o alloca_slot.c
 * rsp before the first alloca is &slot (the lowest fixed-frame slot); slot is written only later. */
extern void use(char *a, long *p);
extern long get(void);

long alloca_slot(int n, long k)
{
    long slot = 0;
    while (k-- > 0) {
        char *buf = __builtin_alloca(n);
        use(buf, 0);
        if (get())
            slot = get();
    }
    use(0, &slot);
    return slot;
}
