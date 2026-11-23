/* The executable the bundles are loaded into. Its globals are exported, so a
 * bundle can bind to them either through -bundle_loader or by flat lookup. */
#include <dlfcn.h>
#include <stdio.h>

int host_value = 42;

int host_callback(int x)
{
    return x + host_value;
}

int main(int argc, char **argv)
{
    void *handle = dlopen(argc > 1 ? argv[1] : "plugin.bundle", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }
    int (*plugin_run)(int) = (int (*)(int))dlsym(handle, "plugin_run");
    printf("%d\n", plugin_run(1));
    return 0;
}
