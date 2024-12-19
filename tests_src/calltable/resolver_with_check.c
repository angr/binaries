#include "common.h"

 int src_func(int i, int x, int y) {
    return ( (i < 4) ? table[i](x, y) : 0 ) & 0xff;
}
