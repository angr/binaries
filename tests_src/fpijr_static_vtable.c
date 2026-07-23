/* A chained dereference through statically initialized read-only structures: dev->drv->read. Resolving it requires
 * following which region each stored pointer points to, using the initialized contents of the binary. */
#include <stdio.h>

struct driver {
    long kind;
    int (*read)(int);
};

struct device {
    long id;
    const struct driver *drv;
};

__attribute__((noinline)) int r0(int x) { return x + 10; }
__attribute__((noinline)) int r1(int x) { return x * 3; }

static const struct driver drv0 = {0, r0};
static const struct driver drv1 = {1, r1};
static const struct device dev0 = {0, &drv0};
static const struct device dev1 = {1, &drv1};
static const struct device *const devs[2] = {&dev0, &dev1};

__attribute__((noinline)) int dispatch(int i, int x) {
    const struct device *d = devs[i & 1];
    return d->drv->read(x);
}

int main(int argc, char **argv) {
    (void)argv;
    printf("%d\n", dispatch(argc, argc));
    return 0;
}
