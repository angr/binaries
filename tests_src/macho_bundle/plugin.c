/* A loadable bundle that imports from the executable that loads it. */
#include <stdio.h>

extern int host_value;
int host_callback(int x);

int plugin_run(int x)
{
    int result = host_callback(x) + host_value;
    printf("plugin_run(%d) = %d\n", x, result);
    return result;
}
