#include "common.h"

int src_func(int i, int x, int y) {
    if (i < 4) {
        return table[i](x, y);
    } else {
        return 0;
    }
}
